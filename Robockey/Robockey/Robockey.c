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
#include "m_num.h"
#include "m_wii.h"
#include "m_wireless_variables.h" //define CHANNEL, ADDRESS, PACKET_LENGTH in this header (# of variables to send = (PACKET_LENGTH-1))

void rotate(int dir) {
	OCR1B = OCR1A;
	OCR0B = OCR0A;
	if (dir == LEFT) {
		set(DDRB,2);
		clear(DDRB,3);
	}
	else if (dir == RIGHT) {
		clear(DDRB,2);
		set(DDRB,6);
	}
}

void turn(int dir) {
	//TODO: Implement code
}

void findPuck() {
	//TODO: Implement code
}

void drive_to_puck() {
	//TODO: Implement code
}

void drive_to_goal() {
	//TODO: Implement code
}

void shoot() {
	//TODO: Implement code
}

void game_stop() {
	//TODO: Implement code
}

int main(void)
{
	//wireless stuffs
	m_bus_init();
	m_num_init();
	m_rf_open(CHANNEL, ADDRESS, PACKET_LENGTH);
	int counter = 0;
	//
	
	m_clockdivide(0);
	
	//TIMER 0: For Controlling the solenoid
	
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
	
	//TIMER 1: For Controlling the left wheel
	
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
	
	//TIMER 3: For Controlling the right wheel
	
	set(TCCR3B, WGM33);
	set(TCCR3B, WGM32);
	set(TCCR3A, WGM31);
	clear(TCCR3A, WGM30);
	
	set(TCCR3A, COM3A1);
	clear(TCCR3A, COM3A0);
	
	clear(TCCR3B, CS32);
	clear(TCCR3B, CS31);
	set(TCCR3B, CS30);
	
	ICR3 = 0xFFFF;
	OCR1A = 0;
	
	//Pins for controlling speed of left and right wheel
	set(DDRB,6);
	set(DDRC,6);
	
	//Pins for determining direction of wheels
	set(DDRB,2);
	set(DDRB,3);
	
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
	set(DIDR2, ADC8D);	//Disable digital input for D4
	set(DIDR2, ADC9D);	//Disable digital input for D6
		
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
	
	set(ADCSRB, MUX5);//Set analog input (D4)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
		
	set(ADCSRB, MUX5);//Set analog input (D6)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	set(ADCSRA, ADIF);	//Enable reading results
	
	

	int state = -1; //state variable
	int count = 0;
	
    while(1)
    {
		//wireless stuffs
		
        //manually say what each buffer[i] will be (corresponds to a state, variable output, etc.)

        //e.g.
        buffer[0] = 50;
        buffer[1] = 2;
        
        if (counter > 30000) {
	        m_rf_send(ADDRESS,buffer,PACKET_LENGTH);
	        m_green(TOGGLE);

	        counter = 0;
        }
        
        counter++;
		
		
		//switch states
        switch (state) {
			
			case -1:
				m_wait(500);
				disp(count % 10);
				count++;
			break;
			
			case 0:
			rotate(LEFT);
			rotate(RIGHT);
			break;
			
			case 1:
			findPuck();
			break;
			
			case 2:
			drive_to_puck();
			break;
			
			case 3:
			drive_to_goal();
			break;
			
			case 4:
			shoot();
			break;
			
			case 5:
			game_stop();
			break;
			
			default:
			rotate(LEFT);
			break;
		}
        
    }
}