/*
* Wireless_Communication____Computer.c
*
* Created: 11/18/2013 7:44:04 PM
*  Author: Klyde
*/

//for use with Terminal.exe
#define F_CPU 16000000UL
#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "m_wireless_variables.h"


volatile int flag = 0;

char array[3] = {0, 0, 0};

int main(void)
{

	//m_bus_init();
	m_rf_open(CHANNEL, ADDRESS, PACKET_LENGTH);
	m_usb_init();

	int i;

	char yes;

	int counter = 0;

	char rx_buffer;

	sei();
	
	
	
	while(1)
	{
		if (flag == 1) {
			//m_green(TOGGLE);
			
			//while(!m_usb_rx_available());  	//wait for an indication from the computer
			rx_buffer = m_usb_rx_char();  	//grab the computer packet

			m_usb_rx_flush();  				//clear buffer

			if(rx_buffer == 1) {  			//computer wants ir buffer
				//write ir buffer as concatenated hex:  i.e. f0f1f4f5

				for (int i = 0 ; i < 3 ; i++){
					m_usb_tx_int((int)array[i]);
					m_usb_tx_char('\t');

				}

				m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
			}
			flag = 0;
		}
		
	}
}

ISR(INT2_vect){
	cli();
	m_green(TOGGLE);
	if (flag == 0) {
	m_rf_read(array,3);
	m_red(TOGGLE);
	flag = 1;
	}
	sei();
}

