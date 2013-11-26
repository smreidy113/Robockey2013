/*
* PT_test.c
*
* Created: 11/25/2013 11:23:30 PM
*  Author: Klyde
*/


#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#define F_CPU 16000000UL

volatile int flag = 0;

int main(void)
{
	
	m_clockdivide(0);
	
	clear(ADCSRB, MUX5);//Set analog input (F6) PHOTOTRANSISTOR 1
	set(ADMUX, MUX2);	//^
	set(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	//ADC's
	sei();					//Set up interrupts
	set(ADCSRA,ADIE);
	
	clear(ADMUX, REFS1);	//Voltage reference is AR pin (5V)
	clear(ADMUX, REFS0);	//^
	
	set(ADCSRA, ADPS2);	//Set scale to /128
	set(ADCSRA, ADPS1);	//^
	set(ADCSRA, ADPS0);	//^
	
	set(DIDR0, ADC0D);	//Disable digital input for F0
	set(DIDR0, ADC1D);	//Disable digital input for F1
	set(DIDR0, ADC4D);	//Disable digital input for F4
	set(DIDR0, ADC5D);	//Disable digital input for F5
	set(DIDR0, ADC6D);	//Disable digital input for F6
	set(DIDR2, ADC8D);	//Disable digital input for D4
	set(DIDR2, ADC9D);	//Disable digital input for D6
	
	set(ADCSRA, ADATE);	//Set trigger to free-running mode
	
	set(ADCSRA, ADIF);	//Enable reading results
	
	//MATLAB PLOTTING STUFFS*****************************************************************
	m_clockdivide(0);
	
	
	m_red(ON);
	m_green(ON);

	m_usb_init(); // connect usb
	while(!m_usb_isconnected()){};  //wait for connection

	m_red(OFF);
	m_green(OFF);

	char rx_buffer; //computer interactions
	
	int i;

	while(1){

		m_red(ON);
		m_green(OFF);
		m_red(OFF);
		m_green(ON);

		while(!m_usb_rx_available());  	//wait for an indication from the computer
		rx_buffer = m_usb_rx_char();  	//grab the computer packet

		m_usb_rx_flush();  				//clear buffer

		if(rx_buffer == 1) {  			//computer wants ir buffer
			//write ir buffer as concatenated hex:  i.e. f0f1f4f5

			
				m_usb_tx_int(ADC);
				m_usb_tx_char('\t');


			m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
		}
	}
	
}

