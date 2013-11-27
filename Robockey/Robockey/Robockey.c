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
#include <stdarg.h>
#include "m_general.h"
#include "m_rf.h"
#include "m_port.h"
#include "m_num.h"
#include "m_wii.h"
#include "m_wireless_variables.h" //define CHANNEL, ADDRESS, PACKET_LENGTH in this header (# of variables to send = (PACKET_LENGTH-1))
#include "m_usb.h"
#include "m_loc.h"

void rotate(int dir) {
	OCR1B = OCR1A;
	OCR3A = ICR3;
	if (dir == RIGHT) {
		set(PORTB,2);
		clear(PORTB,3);
	}
	else if (dir == LEFT) {
		clear(PORTB,2);
		set(PORTB,3);
	}
}

void turn(int dir,int degree) {
	set(PORTB,2);
	set(PORTB,3);
	if (dir == LEFT) {
		OCR3A = ICR3;
		OCR1B = degree;
	}
	
		if (dir == RIGHT) {
			OCR3A = degree;
			OCR1B = OCR1A;
		}
}

void reverse(){
	OCR1B = OCR1A;
	OCR3A = ICR3;
	clear(PORTB,2);
	clear(PORTB,3);
}

void forward() {
			OCR1B = OCR1A;
			OCR3A = ICR3;
			set(PORTB,2);
			set(PORTB,3);
}

void PT_cycle() {

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
		m_wait(100);
		clear(PORTB,7);
}

void game_pause() {
	clear(DDRB,6);
	clear(DDRC,6);
	clear(DDRB,2);
	clear(DDRB,3);
}

void game_resume() {
	set(DDRB,6);
	set(DDRC,6);
	set(DDRB,2);
	set(DDRB,3);
}

int MATLAB_test(int count, ...) {
	//see wikipedia article on variadic functions******
			va_list ap;
			int array[count];
			va_start(ap, count);
			for (int j= 0; j < count; j++) {
				array[j] = va_arg(ap, int);
			}
			va_end(ap);
		//*************
			char rx_buffer;
			while(!m_usb_rx_available());  	//wait for an indication from the computer
			rx_buffer = m_usb_rx_char();  	//grab the computer packet

			m_usb_rx_flush();  				//clear buffer

			if(rx_buffer == 1) {  			//computer wants ir buffer
				//write ir buffer as concatenated hex:  i.e. f0f1f4f5

				for (int i = 0; i < count; i++) {
				m_usb_tx_uint(array[i]);
				m_usb_tx_char('\t');
				}
		

				m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
			}
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
	
	OCR0A = 0xFF;
	OCR0B = 0;
	
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
	//up to ICR3, clear at OCR3A & set at rollover
	
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
	OCR3A = 0;
	
	//Pin for controlling solenoid pulse
	set(DDRB,7);
	
	//Pins for controlling speed of left and right wheel
	set(DDRB,6);
	set(DDRC,6);
	
	//Pins for determining direction of wheels
	clear(DDRB,2);
	clear(DDRB,3);
	
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
	state = -3; //set state
	long count = 0;
	
	char yes;
	m_usb_init();
	m_bus_init();
	m_wii_open();
	local_init();

    while(1)
    {
		//wireless stuffs
		
        //manually say what each buffer[i] will be (corresponds to a state, variable output, etc.)

        //e.g.
        //buffer[0] = 50;
        //buffer[1] = 2;
        
		/*
        if (counter > 30000) {
	        m_rf_send(ADDRESS,buffer,PACKET_LENGTH);
	        m_green(TOGGLE);

	        counter = 0;
        }
        
        counter++;
		*/
		
		//constant localization
		yes = m_wii_open();
		m_wii_read(blobs);
		
					if (yes) {
						MATLAB_test(13, 1, (int) blobs[0], (int)blobs[1], (int)blobs[2], (int)blobs[3], (int)blobs[4], (int)blobs[5], (int)blobs[6], (int)blobs[7], (int)blobs[8], (int)blobs[9], (int)blobs[10], (int)blobs[11]);
					}
					
					else {m_red(ON);}
					
		
		//switch states
        switch (state) {
			
			case -4:
			m_wait(1000);
				shoot();
				state = -3;
			break;
			
			case -3: //test Limit switches
				if (check(PINB,1)) {
					
					rotate(LEFT);
				}
				
				//else if (!check(PINB,1)) {
					//OCR1B = 0;
					//OCR3A = 0;
				//}
				
				else if (check(PINB,0)) {
					
					rotate(RIGHT);
				}
				
				else {
					OCR1B = 0;
					OCR3A = 0;
				}
			break;
			
			case -2: //test turning n driving n stuff
			turn(LEFT,OCR1A/4);
			m_wait(1000);
			turn(RIGHT, ICR3/5);
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