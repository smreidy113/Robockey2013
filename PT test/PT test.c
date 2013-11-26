/*
* PT_test.c
*
* Created: 11/25/2013 11:23:30 PM
*  Author: Klyde
*/


#define F_CPU 16000000UL
#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"

volatile int flag = 0;

int i = 0;
int ADCarr[7] = {0, 0, 0, 0, 0, 0, 0};

void chooseInput(int i) {
	switch (i) {
		case 0:
			clear(ADCSRB, MUX5);//Set analog input (F0)
			clear(ADMUX, MUX2);	//^
			clear(ADMUX, MUX1);	//^
			clear(ADMUX, MUX0);	//^
			break;
		case 1:
			set(ADCSRB, MUX5);//Set analog input (D4)
			clear(ADMUX, MUX2);	//^
			clear(ADMUX, MUX1);	//^
			clear(ADMUX, MUX0);	//^
			break;
		case 2:
			set(ADCSRB, MUX5);//Set analog input (D6)
			clear(ADMUX, MUX2);	//^
			clear(ADMUX, MUX1);	//^
			set(ADMUX, MUX0);	//^
			break;
		case 3:
			clear(ADCSRB, MUX5);//Set analog input (F6) PHOTOTRANSISTOR 1
			set(ADMUX, MUX2);	//^
			set(ADMUX, MUX1);	//^
			clear(ADMUX, MUX0);	//^
			break;
		case 4:
			clear(ADCSRB, MUX5);//Set analog input (F5)
			set(ADMUX, MUX2);	//^
			clear(ADMUX, MUX1);	//^
			set(ADMUX, MUX0);	//^
			break;
		case 5:
			clear(ADCSRB, MUX5);//Set analog input (F4)
			set(ADMUX, MUX2);	//^
			clear(ADMUX, MUX1);	//^
			clear(ADMUX, MUX0);	//^
			break;
		case 6:
			clear(ADCSRB, MUX5);//Set analog input (F1)
			clear(ADMUX, MUX2);	//^
			clear(ADMUX, MUX1);	//^
			set(ADMUX, MUX0);	//^
			i = -1;
			break;
	}
}


int main(void)
{
	
	m_clockdivide(0);
	
		
	sei();					//Set up interrupts
	set(ADCSRA, ADIE);
	
	clear(ADMUX, REFS1);	//Voltage reference is AR pin (5V)
	clear(ADMUX, REFS0);	//^
	
	set(ADCSRA, ADPS2);	//Set scale to /128
	set(ADCSRA, ADPS1);	//^
	set(ADCSRA, ADPS0);	//^
	
	set(DIDR0, ADC0D);	//Disable digital input for F0
	
	set(ADCSRA, ADATE);	//Set trigger to free-running mode
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	set(ADCSRA, ADIF);	//Enable reading results
	
	//MATLAB PLOTTING STUFFS*****************************************************************
	m_clockdivide(0);
	
	
	m_red(ON);
	m_green(ON);

	m_usb_init(); // connect usb
	while(!m_usb_isconnected());  //wait for connection

	m_red(OFF);
	m_green(OFF);

	//char rx_buffer; //computer interactions
	//m_usb_tx_string("FUCK");
	
	//int i;
	
	long counter = 0;

	while(1){
		m_red(TOGGLE);
		m_green(TOGGLE);

		/*while(!m_usb_rx_available());  	//wait for an indication from the computer
		rx_buffer = m_usb_rx_char();  	//grab the computer packet

		m_usb_rx_flush();  				//clear buffer*/

		
			//write ir buffer as concatenated hex:  i.e. f0f1f4f5
		if (counter == 300000) {
			for (int j = 0; j < 7; j++) {
				m_usb_tx_string("ADC");
				m_usb_tx_int(j);
				m_usb_tx_string(": ");
				m_usb_tx_int(ADCarr[j]);
				m_usb_tx_string("\t");
			}
			m_usb_tx_string("\n");
			counter = 0;
			m_usb_
		}
		counter++;
	}
	
}

ISR(ADC_vect) {
	if (i < 7 && i >= 0) {
		ADCarr[i] = ADC;
	}
	clear(ADCSRA, ADEN);	//Enable/Start conversion
	clear(ADCSRA, ADSC);	//^
	chooseInput(i);
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	i++;
}