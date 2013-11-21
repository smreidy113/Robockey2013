 
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


 int main(void){

	 //m_red(ON);
	 //m_green(ON);

	 m_bus_init();
	 
	 m_usb_init(); // connect usb
	 //while(!m_usb_isconnected()){};  //wait for connection

	 //m_red(OFF);
	 //m_green(OFF);

	 char rx_buffer; //computer interactions
	 
	 
	 m_wii_open();

	 

	 
	 int i;
	 int counter = 0;
	 while(1){
		 
		 
		 if (counter == 10000) {
			 //while(!m_usb_rx_available());  	//wait for an indication from the computer
			 //rx_buffer = m_usb_rx_char();  	//grab the computer packet
			 
			 //m_usb_rx_flush();  				//clear buffer
			 
			 localize(data);
			 m_red(ON);



			 //if(rx_buffer != -1) {  			//computer wants ir data
			 //write ir data as concatenated hex:  i.e. f0f1f4f5
			 //m_green(TOGGLE);
			 
			 
			 //for printing to Terminal.exe	
			 //so far: rotating does not work -- it changes x & y; angle calculation does not work either--it changes when in quadrants 1 & 2, but it stuck at either 0 or 45 in quadrants 3 & 4
			 //also check the orientation of the mWii on the wiki -- we need to invert the x direction again	
			 for (i = 0 ; i < 3 ; i++){
				 if (i == 2) {
					 m_usb_tx_int((int) (data[i] * 180.0 / 3.14));
					 m_usb_tx_string("\t");
				 }
				 
				 else {
					 m_usb_tx_int((int) data[i]);
					 m_usb_tx_string("\t");
				 }

			 }

			 m_usb_tx_string("\tx[top]: ");
			 m_usb_tx_int(data[3]);
			 m_usb_tx_string("\tx[bottom]: ");
			 m_usb_tx_int(data[4]);
			 m_usb_tx_string("\n");
			 			 m_usb_tx_string("\ty[top]: ");
			 			 m_usb_tx_int(data[5]);
			 			 m_usb_tx_string("\ty[bottom]: ");
			 			 m_usb_tx_int(data[6]);
			 			 m_usb_tx_string("\n");
			 m_green(TOGGLE);
			 
			 counter = 0;

		 }
		 
		 counter++;

		 m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
		 //}
	 }
 }
