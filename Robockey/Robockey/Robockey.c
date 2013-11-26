/*
 * Robockey.c
 *
 * Created: 11/15/2013 11:15:13 PM
 *  Author: Sean, Klyde
 */ 

#define F_CPU 16000000UL
#define RIGHT 0
#define LEFT 1

#include <avr/io.h>
#include "m_general.h"
#include "m_rf.h"
#include "m_port.h"
#include "m_num.h"
#include "m_wii.h"
#include "m_wireless_variables.h" //define CHANNEL, ADDRESS, PACKET_LENGTH in this header (# of variables to send = (PACKET_LENGTH-1))
#include "m_usb.h"

void rotate(int dir) {
	OCR1B = OCR1A;
	ICR3 = OCR3A;
	if (dir == RIGHT) {
		set(PORTB,2);
		clear(PORTB,3);
	}
	else if (dir == LEFT) {
		clear(PORTB,2);
		set(PORTB,3);
	}
}

void turn(int dir) {
	//TODO: Implement code
}

void reverse(){
	OCR1B = OCR1A;
	ICR3 = OCR3A;
	clear(PORTB,2);
	clear(PORTB,3);
}

void forward() {
			OCR1B = OCR1A;
			ICR3 = OCR3A;
			set(PORTB,2);
			set(PORTB,3);
}

void PT_cycle() {
	//cycles ADC input clockwise

	clear(ADCSRB, MUX5);//Set analog input (F6) PHOTOTRANSISTOR 1
	set(ADMUX, MUX2);	//^
	set(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	/*
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
	
	clear(ADCSRB, MUX5);//Set analog input (F5)
	set(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^

	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
		
	clear(ADCSRB, MUX5);//Set analog input (F4)
	set(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^

	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
	
	clear(ADCSRB, MUX5);//Set analog input (F1)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
	
	clear(ADCSRB, MUX5);//Set analog input (F0)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^

	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
	
	set(ADCSRB, MUX5);//Set analog input (D4)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	clear(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
	
	set(ADCSRB, MUX5);//Set analog input (D6)
	clear(ADMUX, MUX2);	//^
	clear(ADMUX, MUX1);	//^
	set(ADMUX, MUX0);	//^
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	clear(ADCSRA, ADEN);	//stop conversion
	clear(ADCSRA, ADSC);
	*/
	
}

void findPuck() {
	//TODO: Implement code
}

void drive_to_puck() {

}



void drive_to_goal() {
	//TODO: Implement code
}

void shoot() {
		set(PORTB,7);
		m_wait(300);
		clear(PORTB,7);
}

void game_pause() {
	OCR1B= 0;
	ICR3 = 0;
	clear(DDRB,6);
	clear(DDRC,6);
	clear(DDRB,2);
	clear(DDRB,3);
}

void game_resume() {
	OCR1B = OCR1A;
	ICR3 = OCR3A;
	set(DDRB,6);
	set(DDRC,6);
	set(DDRB,2);
	set(DDRB,3);
}

int main(void)
{
	/*/wireless stuffs
	m_bus_init();
	
	m_rf_open(CHANNEL, ADDRESS, PACKET_LENGTH);
	int counter = 0;
	/*/
	
	m_num_init();
	int flag;
	
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
	
	set(DDRB,7);
	
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
	
	set(DDRB,6);
	
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
	
	//Pin for controlling solenoid pulse
	set(DDRB,7);
	
	//Pins for controlling speed of left and right wheel
	set(DDRB,6);
	set(DDRC,6);
	
	//Pins for determining direction of wheels
	set(DDRB,2);
	set(DDRB,3);
	
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
	
	//Limit Switch stuffs
	clear(DDRB,0); //set to input, RIGHT LIMIT SWITCH
	clear(DDRB,1); //set to input, LEFT LIMIT SWITCH
	
	clear(PORTB,0); //disable internal pull up resistor
	clear(PORTB,1); //disable internal pull up resistor
	
	 
	int state; // state variable
	state = -1; //set state
	int count = 0;
	
	m_usb_init();
	
    while(1)
    {
		//wireless stuffs
		
        //manually say what each buffer[i] will be (corresponds to a state, variable output, etc.)

        //e.g.
        buffer[0] = 50;
        buffer[1] = 2;
        
		/*
        if (counter > 30000) {
	        m_rf_send(ADDRESS,buffer,PACKET_LENGTH);
	        m_green(TOGGLE);

	        counter = 0;
        }
        
        counter++;
		*/
		
		
		//switch states
        switch (state) {
			
			case -4: //test phototransistors
				PT_cycle();
				if (count > 10000) {
				m_usb_tx_string("\tADC: ");
				m_usb_tx_int(ADC);
				count = 0;
				}
				count++;
			break;
			
			case -3: //test Limit switches
				if (check(PINB,0)) {
					m_green(ON);
				}
				
				else {
					m_green(OFF);
				}
				
				if (check(PINB,1)) {
					m_red(ON);
				}
				
				else {
					m_red(OFF);
				}
			break;
			
			case -2: //test motor & shooting
				//m_wait(500);
				//rotate(RIGHT);
				//m_wait(2000);
				//rotate(RIGHT);
				//m_wait(2000);
				//drive_to_puck();
				//m_wait(1000);
				//game_pause();
				//m_wait(500);
				forward();
				m_wait(500);
				reverse();
				m_wait(500);
				game_pause();
				m_wait(500);
				game_resume();
			break;
			
				
			case -1: //testing electronics/LED number display
				m_wait(500);
				disp(8);
				//count++;
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
			game_pause();
			break;
			
			default:
			rotate(LEFT);
			break;
		}
        
    }
}

ISR(ADC_vect) {
	if (ADC  > 500) {
		m_green(ON);
	}
	
	else {
		m_green(OFF);
	}
}