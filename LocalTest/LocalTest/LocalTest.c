/*
 * Robockey.c
 *
 * Created: 11/15/2013 11:15:13 PM
 *  Author: Sean, Klyde
 */ 

#define F_CPU 16000000UL
#define RIGHT 0
#define LEFT 1
#define CHANNEL 1
#define RXADDRESS 0x08
#define GOALBX 115
#define GOALBY 0
#define GOALAX -115
#define GOALAY 0

#include <avr/io.h>
#include <stdarg.h>
#include "m_general.h"
#include "m_rf.h"
//#include "m_port.h"
//#include "m_num.h"
#include "m_wii.h"
#include "m_wireless_variables.h" //define CHANNEL, ADDRESS, PACKET_LENGTH in this header (# of variables to send = (PACKET_LENGTH-1))
#include "m_usb.h"
#include "m_loc.h"


volatile char changedState = 0;
char rx_buffer;
char yes;
char buffer[10];

// subroutines
void set_ADC(void);
void update_ADC(void);

// global variables
#define DEBUG 1
#define CLOCK 0

float puckdistance;
int state = 0;

int flag = 0;

int i = 0;
int ADCarr[7] = {0, 0, 0, 0, 0, 0, 0};
	
	int index = 0;
	int maxval = 0;
	int diff = 0;
	float deg = 0.0;

void chooseInput(int i) {
	switch (i) {
		case 0:
		clear(ADCSRB, MUX5);//Set analog input (F0)
		clear(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		clear(ADMUX, MUX0);	//^

		break;
		case 1:
		clear(ADCSRB, MUX5);//Set analog input (F1)
		clear(ADMUX, MUX2);	//^
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

		clear(ADCSRB, MUX5);//Set analog input (F5)
		set(ADMUX, MUX2);	//^
		clear(ADMUX, MUX1);	//^
		set(ADMUX, MUX0);	//^
		break;
		case 4:
		clear(ADCSRB, MUX5);//Set analog input (F6)
		set(ADMUX, MUX2);	//^
		set(ADMUX, MUX1);	//^
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

void getADC() {
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
			clear(ADCSRA, ADEN);	//Enable/Start conversion
			clear(ADCSRA, ADSC);	//^
			clear(ADCSRA, ADATE);
			clear(ADCSRA, ADIF);
			if (flag >= 0 && flag < 7) {
				flag = (flag + 1) % 7;
				chooseInput(flag);
			}
			set(ADCSRA, ADATE);	//Set trigger to free-running mode
			set(ADCSRA, ADEN);	//Enable/Start conversion
			set(ADCSRA, ADSC);	//^
			
			set(ADCSRA, ADIF);	//Enable reading results
			conversion = 0;
	}

	ADCarr[0] = ADC0;
	ADCarr[1] = ADC1;
	ADCarr[2] = ADC2;
	ADCarr[3] = ADC3;
	ADCarr[4] = ADC4;
	ADCarr[5] = ADC5;
	ADCarr[6] = ADC6;
	
	
	
}

void reportADC() {

	m_red(ON);
	m_green(ON);

	m_usb_init(); // connect usb
	while(!m_usb_isconnected());  //wait for connection

	m_red(OFF);
	m_green(OFF);

	char rx_buffer; //computer interactions
	int index = 0;
	int maxval = 0;
	float deg = 0.0;
	float diff = 0.0;

	
	

	getADC();
	index = 0;
	maxval = 0;
	//m_green(TOGGLE);
	for(int i = 0; i < 7; i++) {
		if (ADCarr[i] > maxval) {
			index = i;
			maxval = ADCarr[i];
		}
	}
	switch (index) {
		case 0:
		diff = ADCarr[0] - ADCarr[6];
		deg = exp(-1.0*fabs(((float)diff))/400.0);
		m_green(ON);
		m_red(OFF);
		break;
		case 6:
		diff = ADCarr[6] - ADCarr[0];
		deg = exp(-1.0*fabs(((float)diff))/400.0);
		m_green(OFF);
		m_red(ON);
		break;
		default:
		m_green(ON);
		m_red(ON);
	}
	//while(!m_usb_rx_available());  	//wait for an indication from the computer
		//rx_buffer = m_usb_rx_char();  	//grab the computer packet

		//m_usb_rx_flush();  				//clear buffer

		//if(rx_buffer == 1) {  			//computer wants ir buffer
			//write ir buffer as concatenated hex:  i.e. f0f1f4f5
			m_usb_tx_int(ADCarr[0]);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADCarr[1]);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADCarr[2]);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADCarr[3]);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADCarr[4]);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADCarr[5]);
			m_usb_tx_char('\t');
			m_usb_tx_int(ADCarr[6]);
			m_usb_tx_char('\t');
			m_usb_tx_int(index);
			m_usb_tx_char('\t');
			m_usb_tx_int((int)(deg*100));
			m_usb_tx_char('\t');

		//}
		m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
		//}
}

void rotate(int dir, float speed) {
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

void turn(int dir,float speed, float degree) {
	set(PORTB,2);
	set(PORTB,3);
	if (dir == LEFT) {
		OCR3A = (unsigned int) ((float)ICR3 * speed);
		OCR1B = (unsigned int) (degree * speed * OCR1A);
	}
	
		if (dir == RIGHT) {
			OCR3A = (unsigned int) (degree * speed * ICR3);
			OCR1B = (unsigned int) ((float) OCR1A * speed);
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
		getADC();
		index = 0;
		maxval = 0;
		for(int i = 0; i < 7; i++) {
			if (ADCarr[i] > maxval) {
				index = i;
				maxval = ADCarr[i];
			}
		}
		switch (index) {
			case 0: 
				puckdistance = (log(((double) ADCarr[0])) * -1.0 * 89.64) + 664.58;
				diff = ADCarr[0] - ADCarr[6];
				deg = exp(-1.0*(fabs((float)diff))/70.0);
				turn(RIGHT,1.0,deg);
				m_green(ON);
				m_red(OFF);
				break;
			case 1:
				turn(RIGHT,1.0,0.15);
				break;
			case 2:
				turn(RIGHT,1.0,0);
				break;
			case 3:
				if (ADCarr[2] > ADCarr[4]) {
					rotate(RIGHT, 1);
				}
				else {
					rotate(LEFT, 1);
				}
				break;
			case 4:
				turn(LEFT,1.0,0);
				break;
			case 5:
				turn(LEFT,1.0,0.15);
				break;
			case 6:
				puckdistance = (log(((double) ADCarr[0])) * -1.0 * 89.64) + 664.58;
				diff = ADCarr[6] - ADCarr[0];
				deg = exp(-1.0*(fabs((float)diff))/70.0);
				turn(LEFT,1.0,deg);
				m_green(OFF);
				m_red(ON);
				break;
			default:
				m_red(ON);
				m_green(ON);
				break;
		}
	//}
}

	float speed_cap = 0.5;
	float angle_dif = 0.0;
	float distance = 0.0;
	float spd = 0.0;
	int dir = 0;

void drive_to_point2(int x, int y) {
	m_green(ON);
	//m_wait(500);

	//while(1) {
		localize(data);
		
		//Set angle difference to be between -180 and 180
		angle_dif = (((int) (((data[2] + 90.0) * -1.0) - (float)(atan2((double)y-data[1],(double)x-data[0]))*180.0/3.14)) + 900) % 360 - 180;
		if (angle_dif > 0) {
			dir = RIGHT;
		}
		else {
			dir = LEFT;
		}
		
		//Angle of 180 corresponds to deg of 0; angle of 0 corresponds to deg of 1
		deg = exp(-1.0* ((double)fabs(angle_dif))/30.0);
		//Set distance
		distance = (int) sqrt(((double)y - data[1])*((double)y - data[1])+((double)x - data[0])*((double)x - data[0]));
		spd = ((float)distance)/70.0;
		//*****************************************************************************
			rx_buffer = m_usb_rx_char();  	//grab the computer packet

			m_usb_rx_flush();  				//clear buffer

			if(rx_buffer == 1) {  			//computer wants ir buffer
				//write ir buffer as concatenated hex:  i.e. f0f1f4f5
				data[3] = angle_dif;
				data[4] = deg*100.0;
				data[5] = distance;
				data[6] = spd*100.0;
				for (int i = 0 ; i < 7 ; i++){
					m_usb_tx_int((int)data[i]);
					m_usb_tx_char('\t');

				}

				m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
			}
			
		//*********************************************************************************
		
		if (spd > speed_cap) spd = speed_cap;
		turn(dir, spd, deg);
		
	//}
	if (distance < 10) {
		m_red(ON);
		reverse();
		OCR1B = 0;
		OCR3A = 0;
		state = 0;
	}
}

void drive_to_goalA() {
	drive_to_point2(GOALBX,GOALBY);
}

void drive_to_goalB() {
	drive_to_point2(GOALAX,GOALAY);
}

void shoot() {
	m_wait(1000);
	m_red(ON);
	set(PORTB,7);
	m_wait(100);
	clear(PORTB,7);
	m_red(OFF);
}

void game_pause() {
	OCR1B = 0;
	OCR3A = 0;
	//clear(PORTB,2);
	//clear(PORTB,3);
}

void game_resume() {
	//set(PORTB,2);
	//set(PORTB,3);
}

void comm_test() {
	set(PORTD,5);
	m_wait(500);
	clear(PORTD,5);
	m_wait(500);
	state = 0;
}



int MATLAB_test(int count, ...) {
	char rx_buffer;
	//see wikipedia article on variadic functions******
			va_list ap;
			int array[count];
			va_start(ap, count);
			for (int j= 0; j < count; j++) {
				array[j] = va_arg(ap, int);
			}
			va_end(ap);
		//*************
			//char rx_buffer;
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
	set(DDRD,5);
	//wireless stuffs
	m_bus_init();
	
	m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH);

	int counter = 0;
	//
	
	//m_num_init();
	int flag;
	
	m_clockdivide(0);
	
	m_disableJTAG();
	
	//TIMER 0: For Controlling the solenoid
// 	
// 	set(TCCR0B, WGM02);
// 	set(TCCR0A, WGM01);
// 	set(TCCR0A, WGM01);
// 	
// 	set(TCCR0A, COM0B1);
// 	clear(TCCR0A, COM0B0);
// 	
// 	set(TCCR0B, CS02);
// 	set(TCCR0B, CS01);
// 	set(TCCR0B, CS00);
// 	
	set(DDRB,7);
	
// 	OCR0A = 0xFF;
// 	OCR0B = 0xff;
// 	
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
	set(DDRB,2);
	set(DDRB,3);
	
	//Blue LED for Comm Test
	//set(DDRB,5);
	
	//ADC's
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
	
	chooseInput(0);
	
	set(ADCSRA, ADEN);	//Enable/Start conversion
	set(ADCSRA, ADSC);	//^
	
	set(ADCSRA, ADIF);	//Enable reading results
	
	
	//Limit Switch stuffs
	clear(DDRB,0); //set to input, RIGHT LIMIT SWITCH
	clear(DDRB,1); //set to input, LEFT LIMIT SWITCH
	
	clear(PORTB,0); //disable internal pull up resistor
	clear(PORTB,1); //disable internal pull up resistor
	
	 
	//int state; // state variable
	state = 4; //set state
	long count = 0;
	

	//m_bus_init();
	m_wii_open();
	m_usb_init();
	//local_init();

    while(1)
    {
		changedState = 0;
		//getADC();
		

		//localize(data);
		
// 		if (check(PINB,0)) {
// 			set(PORTD,5);
// 			state = 0x1A;
// 		} else {
// 			clear(PORTD,5);
// 			state = 2;
// 		}
	

		//switch states
        switch (state) {
			
			case -2:
			getADC();
			if (ADCarr[0] > 500) {
				m_green(ON);
			}
			else {m_green(OFF)}
			break;
			
			case -1: //test Limit switches
				//m_green(ON);
				if (check(PINB,1)) {
					
					m_green(ON);
				}
				
				else if (check(PINB,0)) {
					
					m_red(ON);
				}
				
				else {
					m_red(OFF);
					m_green(OFF);
				}
				
			break;

			case 0:
			//drive_to_point2(-100,0);
			game_pause();
			break;
			
			case 1:
			findPuck();
			break;
			
			case 2:
			//m_red(ON);
			drive_to_puck();
			break;
			
			case 3:
			drive_to_goalA();
			break;
			
			case 4:
			shoot();
			state =  0;
			break;	
			
			case 0xA4:
			game_pause();
			break;
			
			case 0xA0:
			comm_test();
			break;
			
			case 0xA1:
			drive_to_goalA();
			break;
			
			case 0xA2:
			drive_to_goalA();
			break;
			
			case 0xA3:
			drive_to_goalB();
			break;
			
			case 69:
				set(PORTB,2);
				set(PORTB,3);
				OCR1B = OCR1A;
				OCR3A = ICR3;
				break;
				
			case 70:
				reportADC();
				break;
			
			default:
			game_pause();
			break;
		}
        
    }
}


ISR(INT2_vect)  {
	cli();
	m_rf_read(buffer,PACKET_LENGTH);
	sei();
	state=buffer[0];
	changedState = 1;
}

ISR(ADC_vect) {
	cli();
	conversion = 1;
	sei();
}