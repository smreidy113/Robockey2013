 
/********************** ADC Live Plotting & Logging Code ****************************
	Sends ADC value from pin F0 in hex from the M2 microcontroller [http://medesign.seas.upenn.edu/index.php/Guides/MaEvArM].

	By Nick McGill [nmcgill.com]
************************************************************************************/

#define F_CPU 16000000UL

// header files
#include "m_general.h"
#include "m_usb.h"
#include "m_loc.h"
#include "m_bus.h"

// subroutines
void set_ADC(void);
void update_ADC(void);

// global variables
#define DEBUG 1
#define CLOCK 0
float data[3]={0.0};

int main(void){

	m_red(ON);
	m_green(ON);

	m_bus_init();
	
	m_usb_init(); // connect usb
	while(!m_usb_isconnected()){};  //wait for connection

	m_red(OFF);
	m_green(OFF);

	char rx_buffer; //computer interactions
  
	int i;
	while(1){
	
		

		while(!m_usb_rx_available());  	//wait for an indication from the computer
		rx_buffer = m_usb_rx_char();  	//grab the computer packet
		localize(data);
		m_usb_rx_flush();  				//clear buffer		

		if(rx_buffer == 1) {  			//computer wants ir data
			//write ir data as concatenated hex:  i.e. f0f1f4f5
			m_green(TOGGLE);
			for (i = 0 ; i < 3 ; i++){
				//m_usb_tx_int((int) data[i]);
				m_usb_tx_int(1);
				m_usb_tx_char('\t');

			}
			m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
		}
	}
}
