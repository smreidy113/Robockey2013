/*
 * Robockey.c
 *
 * Created: 11/15/2013 11:15:13 PM
 *  Author: Sean, Klyde
 */ 

#define F_CPU 16000000UL
#define RIGHT 1
#define LEFT 0

#include <avr/io.h>
#include "m_general.h"
#include "m_rf.h"
#include "m_port.h"
#include "m_wii.h"

void rotate(int dir) {
	OCR1B = OCR1A;
	OCR0B = OCR0A;
	if (dir == LEFT) {
		set(DDRD,5);
		clear(DDRD,6);
	}
	else if (dir == RIGHT) {
		clear(DDRD,5);
		set(DDRD,6);
	}
}

void turn(int dir) {
	//TODO: Implement code
}

void findPuck() {
	//TODO: Implement code
}

void shoot() {
	//TODO: Implement code
}

int main(void)
{
	
	m_clockdivide(0);
	
	//TIMER 0: For Controlling the left wheel
	
	set(TCCR0B, WGM02);
	set(TCCR0A, WGM01);
	set(TCCR0A, WGM01);
	
	set(TCCR0A, COM0B1);
	clear(TCCR0A, COM0B0);
	
	set(TCCR0B, CS02);
	set(TCCR0B, CS01);
	set(TCCR0B, CS00);
	
	OCR1A = 0xFF;
	OCR1B = 0;
	
	//TIMER 1: For Controlling the right wheel
	
	set(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	set(TCCR1A, WGM10);
	
	set(TCCR1A, COM1B1);
	clear(TCCR1A, COM1B0);
	
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);
	
	OCR1A = 0xFFFF;
	OCR1B = 0;
	
	//Pins for controlling speed of left and right wheel
	set(DDRB,7);
	set(DDRB,6);
	
	//Pins for determining direction of wheels
	set(DDRD,5);
	set(DDRD,6);
	
	//ADC's
	sei();					//Set up interrupts
	
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
	set(DIDR0, ADC7D);	//Disable digital input for F7
	set(DIDR2, ADC8D);	//Disable digital input for D4
	
	set(ADCSRA, ADATE);	//Set trigger to free-running mode
	
	clear(ADCSRB, MUX5);//Set analog input (F0)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	clear(ADCSRB, MUX5);//Set analog input (F1)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^
	
	clear(ADCSRB, MUX5);//Set analog input (F4)
	set(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	clear(ADCSRB, MUX5);//Set analog input (F5)
	set(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^
	
	clear(ADCSRB, MUX5);//Set analog input (F6)
	set(ADMUX, MUX2);	//^
	set(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	clear(ADCSRB, MUX5);//Set analog input (F7)
	set(ADMUX, MUX2);	//^
	set(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^
	
	set(ADCSRB, MUX5);//Set analog input (D4)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	set(ADCSRA, ADIF);	//Enable reading results
	
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}