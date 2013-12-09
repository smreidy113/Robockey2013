/************************************************************************/
/* L6.c
   Authors: Sean Reidy, Klyde Breitton, Nick LaBarbera
  
   Source code for balancing acrobat robot.  Uses complementary filter
   to accurately sense angle without noise, and proportional, integral,
   and derivative (PID) control to determine the amount of torque to
   apply to the wheels.                                                 
                                                                        */
/************************************************************************/

// Constants
#define F_CPU 16000000UL
#define SAMPLERATE 1000
#define BETA 0.97
#define PI 3.14

#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_imu.h"
#include "m_usb.h"

// Initialize variables for PID control
float Kp = 2.5; //

float Ki = 2.0;

float Kd = 10.0;

// Initialize array for packet reading from mRF
int buffer[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
// {a_x, a_y, a_z, g_x, g_y, g_z, m_x, m_y, m_z)

// Initialize variable containing value (1 or 0) 
// returned by IMU for whether it is reading something
unsigned char yes = 0;

// Multiplier for dt: If interrupt is called when code is still
// calculating torque, wait and record number of times to correctly
// adjust dt
int dtmultiplier = 0;
int num_times_skipped = 0;

// Variables for PID control
float P = 0.0;
float I = 0.0;
float I_error = 0.0;
float D = 0.0;
float D_prev_error = 0.0;
long intermediate = 0;

// Flag to signal loop to compute torque
int control = 0;

int main(void) {
	
	m_usb_init();
	
	// Timer will generate PWM to change motor speed: IMU reading
	// --> change OCR1B --> change duty cycle/motor speed
	// if motor needs to reverse, change output pins -- voltage
	// applied to REV || clear output pin -- voltage no longer applied to FWD
	// set pin B4, clear pin B5 -- FWD; clear in B4, set pin B5 -- REV
	
	// Enable interrupts
	sei();
	
	m_clockdivide(0);

	// TIMER 1: This is used to control the speed of the motors

	// Set to Timer 1 to mode 15
	set(TCCR1B,WGM13);
	set(TCCR1B,WGM12);
	set(TCCR1A,WGM11);
	set(TCCR1A,WGM10);
	
	// Clear at OCR1B, set at rollover
	set(TCCR1A,COM1B1);
	clear(TCCR1A,COM1B0);
	
	// Set system clock prescale to 1
	clear(TCCR1B,CS12);
	clear(TCCR1B,CS11);
	set(TCCR1B,CS10);
	
	// This OCR1A setting will give us a frequency high enough to
	// accurately control PWM
	OCR1A = 65535; 
	OCR1B = 1;
	
	// TIMER 0: This is used to control the sampling rate from
	// the IMU for constant dt.

	// Set TCNT0 to go up to OCR0A
	clear(TCCR0B, WGM02);
	set	(TCCR0A, WGM01);
	clear(TCCR0A, WGM00);
	
	// Don't change pin upon hitting OCR0B
	clear(TCCR0A, COM0A1);
	clear(TCCR0A, COM0A0);
	
	// Set clock scale to /1024
	set(TCCR0B, CS02);
	clear(TCCR0B, CS01);
	set(TCCR0B, CS00);
	
	// Set Frequency of readings to 1/SAMPLERATE; dt = 1/SAMPLERATE
	OCR0A = (unsigned int) ((float) F_CPU / 1024 / SAMPLERATE);
	OCR0B = 1;
	
	// Set up interrupt for reading values at sampling rate
	set(TIMSK0, OCIE0A);
	
	// Set output for pin B6, C6
	set(DDRB,6);
	set(DDRC,6);
	
	// Initially set high for C6
	set(PORTC,6);
	clear(PORTC,6);
	
	// Initialize ports B4 and B0
	set(DDRB,4);
	set(DDRB,0);
	
	//**********************************************************************
	// Initialize IMU
	m_imu_init(0, 2); // +/- 1g, +/- 500 degrees per second

	// Clear pins for IMU input & disable internal pull up resistors
	clear(DDRD,0);
	clear(DDRD,1);
	clear(DDRD,2);

	// Disable pullup resistors to enable digital input
	clear(PORTD,0);
	clear(PORTD,1);
	clear(PORTD,2);

	// Get Initial Values for readings
	yes = m_imu_raw(buffer);
	float theta_0;
	
	if (buffer[2] != 0) {
		
		theta_0 = (float) (atan2(buffer[2],buffer[0]));
		
	} 
	
	else {
		
		theta_0 = 3.14/2;
		
	}
	
	float integral = theta_0;
	float integrali1 = 0.0;
	float high_pass = integral;
	float high_passi1 = 0.0;
	float accel = -1* ((float) buffer[0]) / 8192;
	float low_pass = accel;
	float low_passi1 =  0.0;
	float overall_angle = 0.0;
	unsigned int final_angle = 0;
	float final_angle_float = 0.0;
	float dtheta = 0;
	
	int counter = 0;

	// Loop
	while(1) {
		
		// Calculates angle, if control (flag) raised by sampling timer
		if (control == 1) {
			
			// Toggle B4 (determine actual sampling rate)
			toggle(PORTB,4);
			
			// Obtain values from IMU
			yes = m_imu_raw(buffer);
			
			integrali1 = integral;
			
			// Determine change in angle using the integral
			dtheta = ((float) buffer[4]) / 32768 * 500 * 3.14 / 180
			/ SAMPLERATE * (dtmultiplier+1);
			integral = integral + dtheta;
			
			// Use high pass filtering to filter out drift
			high_passi1 = high_pass;
			high_pass = (integral - integrali1) * BETA + high_passi1 * BETA;
			
			// Use low pass filter to filter out noise from translational motion
			low_passi1 = low_pass;
			if (buffer[2] != 0) {
				accel = (float) (-1 * atan2(buffer[2],buffer[0]));
			}
			else {
				accel = -3.14/2;
			}
			low_pass = -1 * (1.0 - BETA) * (accel) + low_passi1 * BETA;
			
			// Determine overall angle with the complementary filter
			// varies from negative pi/2 to positive pi/2
			overall_angle = (high_pass + low_pass) - (PI / 2.0); 
			
			// convert abs(overall angle) in radians to unsigned int
			// for OCR1B calculation
			final_angle = (unsigned int) (labs((long) 
			((overall_angle / (PI/2.0)) * 65535.0)));
			final_angle_float = (float) final_angle;
			
			// P, I, D controls
			P = final_angle_float * Kp;
			I_error = final_angle_float + I_error ;
			I = I_error * Ki;
			D = (final_angle_float - D_prev_error) * Kd;
			D_prev_error = final_angle_float;
			
			// Combine to find final control value
			intermediate = ((long) (P + I + D)) + ((long) (OCR1A * 2/3));

			// Set limit for control to not go over 100%
			if (intermediate > OCR1A) {
				intermediate = (long) OCR1A;
			}
			
			// Set a limit for how far the acrobat tips before stopping
			if (final_angle > 19000 ) {
				intermediate = 0;
			}
			
			// Control value sets the value for OCR1B (duty cycle)
			OCR1B = (unsigned int) intermediate;
			
			// Reset flag
			control = 0;
			
		}
		
		// Directional control: switch direction of motors depending on angle
		
		// If tilting forward (positive angle), set pin C6 to High (motor forward)
		if (overall_angle >= 0.0) {
			
			m_green(ON);
			m_red(OFF);
			set(PORTC,6);
			
		}
		
		// Reverse direction
		else if (overall_angle < 0.0) {
			
			m_green(OFF);
			m_red(ON);
			clear(PORTC,6);

		}
		
		counter++;
		
	}
	
}


//Interrupt for Timer 0: gets called once every 1/SAMPLERATE seconds

ISR(TIMER0_COMPA_vect) {
	if (control == 0) {
		
		dtmultiplier = num_times_skipped;
		num_times_skipped = 0;
		control = 1;
		
	}
	
	else {
		
		num_times_skipped++;
		
	}
	
}