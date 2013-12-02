/*
 * WirelessTest.c
 *
 * Created: 12/2/2013 12:21:33 AM
 *  Author: Sean
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"

#define CHANNEL 1
#define RXADDRESS 0x08
#define PACKET_LENGTH 10

char buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int main(void)
{
	m_clockdivide(0);
	sei();
	m_bus_init();
	char yes = m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH);
	
	if (yes == 1) {
		m_red(ON);
	}
	
    while(1)
    {
        /*if (buffer[0] == 160) {
			m_green(TOGGLE);
		}*/
    }
}

ISR(INT2_vect) {
	//cli();
	m_green(ON);
	m_rf_read(buffer, PACKET_LENGTH);
	//m_green(OFF);
	//sei();
}