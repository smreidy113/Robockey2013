/*
 * mPortTest.c
 *
 * Created: 11/25/2013 7:17:37 PM
 *  Author: Sean
 */ 

#define F_CPU 16000000UL
#define ADDR 0x20

#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_port.h"

char yes;

int main(void)
{
	m_clockdivide(0);
	m_bus_init();
	yes = m_port_init(ADDR);
	int flag = 0;
	set(DDRD,7);
	m_port_set(ADDR,DDRH,7);
    while(1)
    {
		if (yes){
			clear(PORTD,7);
		}
		else {
			set(PORTD,7);
		}
        m_wait(500);
		if (flag == 0) {
			m_green(TOGGLE);
			m_port_set(ADDR,PORTH,7);
			set(PORTD,7);
			flag = 1;
		} else {
			m_port_clear(ADDR,PORTH,7);
			clear(PORTD,7);
			flag = 0;
		}
    }
}