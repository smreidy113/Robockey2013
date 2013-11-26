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

int main(void)
{
	
	m_port_init(ADDR);
	m_port_set(ADDR,DDRG,3);
	int flag = 0;
	
    while(1)
    {
        m_wait(500);
		if (flag == 0) {
			m_port_set(ADDR,PORTG,3);
			flag = 0;
		} else {
			m_port_clear(ADDR,PORTG,3);
			flag = 1;
		}
    }
}