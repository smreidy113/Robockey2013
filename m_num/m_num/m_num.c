/*
 * m_num.c
 *
 * Created: 11/19/2013 1:28:28 AM
 *  Author: Sean
 */ 

#define F_CPU 16000000UL
#define LOWERLEFT1 PORTE
#define LOWERLEFT2 6
#define MIDDLE1 PORTB
#define MIDDLE2 4
#define LOWERRIGHT1 PORTC
#define LOWERRIGHT2 7
#define BOTTOM1 PORTF
#define BOTTOM2 1
#define UPPERLEFT1 PORTB
#define UPPERLEFT2 5
#define TOP1 PORTB
#define TOP2 6
#define UPPERRIGHT1 PORTC
#define UPPERRIGHT2 6

#include <avr/io.h>
#include "m_general.h"

void disp(int num) {
	switch (num) {
		case 1:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			clear(TOP1,TOP2);
			clear(UPPERLEFT1,UPPERLEFT2);
			clear(LOWERLEFT1,LOWERLEFT2);
			clear(BOTTOM1,BOTTOM2);
			clear(MIDDLE1,MIDDLE2);
			break;
		case 2:
			clear(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			clear(UPPERLEFT1,UPPERLEFT2);
			set(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
			break;
		case 3:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			clear(UPPERLEFT1,UPPERLEFT2);
			clear(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
			break;
		case 4:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			clear(TOP1,TOP2);
			set(UPPERLEFT1,UPPERLEFT2);
			clear(LOWERLEFT1,LOWERLEFT2);
			clear(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
			break;
		case 5:
			set(LOWERRIGHT1,LOWERRIGHT2);
			clear(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			set(UPPERLEFT1,UPPERLEFT2);
			clear(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
		break;
		case 6:
			set(LOWERRIGHT1,LOWERRIGHT2);
			clear(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			set(UPPERLEFT1,UPPERLEFT2);
			set(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
			break;
		case 7:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			clear(UPPERLEFT1,UPPERLEFT2);
			clear(LOWERLEFT1,LOWERLEFT2);
			clear(BOTTOM1,BOTTOM2);
			clear(MIDDLE1,MIDDLE2);
			break;
		case 8:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			set(UPPERLEFT1,UPPERLEFT2);
			set(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
			break;
		case 9:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			set(UPPERLEFT1,UPPERLEFT2);
			clear(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			set(MIDDLE1,MIDDLE2);
			break;
		case 0:
			set(LOWERRIGHT1,LOWERRIGHT2);
			set(UPPERRIGHT1,UPPERRIGHT2);
			set(TOP1,TOP2);
			set(UPPERLEFT1,UPPERLEFT2);
			set(LOWERLEFT1,LOWERLEFT2);
			set(BOTTOM1,BOTTOM2);
			clear(MIDDLE1,MIDDLE2);
			break;
	}
}

int main(void)
{
	m_clockdivide(0);	
	
	sei();					//Set up interrupts
	set(TIMSK1, OCIE1A);	//Set up timer interrupt
	
	clear(ADMUX, REFS1);	//Voltage reference is AR pin (5V)
	clear(ADMUX, REFS0);	//^
	
	set(ADCSRA, ADPS2);	//Set scale to /128
	set(ADCSRA, ADPS1);	//^
	set(ADCSRA, ADPS0);	//^
	
	set(DIDR0, ADC0D);	//Disable digital input for F0
	
	set(ADCSRA, ADATE);	//Set trigger to free-running mode
	
	clear(ADCSRB, MUX5);//Set analog input (F0)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	set(ADCSRA, ADIF);	//Enable reading results
	
	set(DDRB,4);
	set(DDRB,5);
	set(DDRB,6);
	set(DDRC,6);
	set(DDRC,7);
	set(DDRE,6);
	set(DDRF,1);
	
	int n = 0;
	
    while(1)
    {
        /*for (int i = 0; i < 10; i++) {
			disp(i);
			m_wait(500);
		}*/
		n = ADC / 102;
		disp(n);
    }
}