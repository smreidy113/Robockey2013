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

int flag = 0;

int i = 0;
int ADCarr[7] = {0, 0, 0, 0, 0, 0, 0};

void chooseInput(int i) {
	switch (i) {
		case 0:
		clear(ADCSRB, MUX5);//Set analog input (F6) PHOTOTRANSISTOR 1
		set(ADMUX, MUX2);	//^
		set(ADMUX, MUX1);	//^
		clear(ADMUX, MUX0);	//^
		break;
		case 1:
		clear(ADCSRB, MUX5);//Set analog input (F5)
		set(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		set(ADMUX, MUX0);	//^
		break;
		case 2:
		clear(ADCSRB, MUX5);//Set analog input (F4)
		set(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		clear(ADMUX, MUX0);	//^
		break;
		case 3:
		clear(ADCSRB, MUX5);//Set analog input (F1)
		clear(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		set(ADMUX, MUX0);	//^
		break;
		case 4:
		clear(ADCSRB, MUX5);//Set analog input (F0)
		clear(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		clear(ADMUX, MUX0);	//^
		break;
		case 5:
		set(ADCSRB, MUX5);//Set analog input (D4)
		clear(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		clear(ADMUX, MUX0);	//^
		break;
		case 6:
		set(ADCSRB, MUX5);//Set analog input (D6)
		clear(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		set(ADMUX, MUX0);	//^
		i=-1;
		break;
	}
}

int ADC0 = 0;
int ADC1 = 0;
int ADC2 = 0;
int ADC3 = 0;
int ADC4 = 0;
int ADC5 = 0;
int ADC6 = 0;
int conversion = 0;

int main(void)
{
	
	m_clockdivide(0);
	
	m_disableJTAG();
	
	sei();					//Set up interrupts
	set(ADCSRA, ADIE);
	
	clear(ADMUX, REFS1);	//Voltage reference is AR pin (5V)
	clear(ADMUX, REFS0);	//^
	
	set(ADCSRA, ADPS2);	//Set scale to /128
	set(ADCSRA, ADPS1);	//^
	set(ADCSRA, ADPS0);	//^
	
	set(DIDR0, ADC0D);	//Disable digital input for F0
	set(DIDR0, ADC1D),
	set(DIDR0, ADC4D);
	set(DIDR0, ADC5D);
	set(DIDR0, ADC6D);
	set(DIDR2, ADC8D);
	set(DIDR2, ADC9D);
	
	set(ADCSRA, ADATE);	//Set trigger to free-running mode
	
	chooseInput(1);
	
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

	char rx_buffer; //computer interactions
	
	
	long counter = 0;

	while(1){
		

		
		

		while(!m_usb_rx_available());  	//wait for an indication from the computer
		rx_buffer = m_usb_rx_char();  	//grab the computer packet

		m_usb_rx_flush();  				//clear buffer

		
		//write ir buffer as concatenated hex:  i.e. f0f1f4f5
		/*if (counter == 300000) {
		for (int j = 0; j < 7; j++) {
		m_usb_tx_string("ADC");
		m_usb_tx_int(j);
		m_usb_tx_string(": ");
		m_usb_tx_int(ADCarr[j]);
		m_usb_tx_string("\t");
		}
		m_usb_tx_string("\n");
		counter = 0;
		
		}
		counter++;*/
		
		if(rx_buffer == 1) {  			//computer wants ir buffer
			//write ir buffer as concatenated hex:  i.e. f0f1f4f5
			m_usb_tx_int(ADC0);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADC1);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADC2);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADC3);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADC4);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADC5);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADC6);
			/*for (int j = 0 ; j < 7 ; j++){
			m_usb_tx_int(ADCarr[j]);
			m_usb_tx_char('\t');
			}*/
		}
		m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
		
		if (conversion) {
			switch (flag) {
				case 0:
					ADC0 = ADC;
					break;
				case 1:
					ADC1 = ADC;
					break;
				case 2:
					ADC2 = ADC;
					break;
				case 3:
					ADC3 = ADC;
					break;
				case 4:
					ADC4 = ADC;
					break;
				case 5:
					ADC5 = ADC;
					break;
				case 6:
					ADC6 = ADC;
					break;
			}
		}
		clear(ADCSRA, ADEN);	//Enable/Start conversion
		clear(ADCSRA, ADSC);	//^
		clear(ADCSRA, ADATE);
		clear(ADCSRA, ADIF);
		if (flag >= 0 && flag < 7) {
			chooseInput(flag);
			flag = (flag + 1) % 7;
		}
		set(ADCSRA, ADATE);	//Set trigger to free-running mode
		set(ADCSRA, ADEN);	//Enable/Start conversion
		set(ADCSRA, ADSC);	//^
		
		set(ADCSRA, ADIF);	//Enable reading results
		conversion = 0;
	}
	
}




ISR(ADC_vect) {
	conversion = 1;

	
	/*if (i < 7 && i >= 0) {
	ADCarr[i] = (int) ADC;
	}
	else {
	m_green(TOGGLE);
	}
	i++;
	clear(ADCSRA, ADEN);	//Enable/Start conversion
	clear(ADCSRA, ADSC);	//^
	chooseInput(i);
	set(ADCSRA, ADATE);	//Set trigger to free-running mode
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	set(ADCSRA, ADIF);	//Enable reading results
	//m_wait(500);
	m_red(TOGGLE);
	flag = 1;*/
}