/*
 * Wireless_Communication____Computer.c
 *
 * Created: 11/18/2013 7:44:04 PM
 *  Author: Klyde
 */ 

//for use with Terminal.exe

#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "m_wireless_variables.h"
#define F_CPU 16000000UL

volatile int flag;


int main(void)
{

m_bus_init();
m_rf_open(CHANNEL, ADDRESS, PACKET_LENGTH);
m_usb_init();

int i;

char yes;

int counter = 0;

sei();
				
    while(1)
    {
		if (counter > 30000) {
		
		if (flag == 1) {
			yes = m_rf_read(buffer,PACKET_LENGTH);
			if (yes == 1) {m_red(TOGGLE);}

						for (i = 0; i <= (PACKET_LENGTH-1); i++) {
							m_usb_tx_string("\tState ");
							m_usb_tx_int(i);
							m_usb_tx_string(": ");
							m_usb_tx_int(buffer[i]);
							
						}
						m_usb_tx_string("\n");
				flag ==0;
		}
		
		counter = 0;
		}
		
		counter++;
    }
}

ISR(INT2_vect){
	flag = 1;
}
