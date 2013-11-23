/*
 * Localization.c
 *
 * Created: 11/20/2013 1:47:43 AM
 *  Author: Sean
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include "m_wii.h"
#include "m_general.h"

//12-size array for holding Wii data
unsigned int blobs[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int top = 0;
int bottom = 0;
int left = 0;
int right = 0;

float beta = 0;
float theta = 0;

float rcenterx = 512;
float rcentery = 512;

unsigned int x[4] = {0, 0, 0, 0};
unsigned int y[4] = {0, 0, 0, 0};
	
float dist = 0;
float distmax = 0, distmin = 1023;
float dcenter = 0;
int far1 = 0, far2 = 0, close1 = 0, close2 = 0;

int numlost = 0;
int recalibrate = 0;

unsigned char getData(unsigned int* x, unsigned int* y) {
	m_wii_read(blobs);
	x[0] = blobs[0];
	x[1] = blobs[3];
	x[2] = blobs[6];
	x[3] = blobs[9];
	y[0] = blobs[1];
	y[1] = blobs[4];
	y[2] = blobs[7];
	y[3] = blobs[10];
	return 1;
} 

unsigned char determine_blobs() {
	int exit = 0;
	distmax = 0;
	distmin = 30000;
	while (exit == 0) {
		getData(x, y);
		if (x[0] < 1023 && x[1] < 1023 && x[2] < 1023 && x[3] < 1023) {
			exit = 1;
			for (int i = 0; i < 4; i++) {
				for (int j = i; j < 4; j++) {
					if (i == j) continue;
					dist = sqrt(((float)x[i]-(float)x[j])*((float)x[i]-(float)x[j])+((float)y[i]-(float)y[j])*((float)y[i]-(float)y[j]));
					if (dist > distmax) {
						distmax = dist;
						far1 = i;
						far2 = j;
						dcenter = dist / 2.0;
					}
					if (dist < distmin) {
						distmin = dist;
						close1 = i;
						close2 = j;
					}
				}
			}
			if (far1 == close1) {
				top = far1;
				bottom = far2;
				right = close2;
				//left + top + bottom + right = 1 + 2 + 3 + 4 = 10
				left = 9 - top - bottom - right;
			}
			else if (far1 == close2) {
				top = far1;
				bottom = far2;
				right = close1;
				left = 9 - top - bottom - right;
			}
			else if (far2 == close1) {
				top = far2;
				bottom = far1;
				right = close2;
				left = 9 - top - bottom - right;
			}
			else {
				top = far2;
				bottom = far1;
				right = close1;
				left = 9 - top - bottom - right;
			}
		}
	}
	return 1;
}

unsigned char local_init() {
	m_wii_open();
	
	getData(x,y);
	

	//Determine indices of all top, bottom, left, right points
	determine_blobs();
	
	return 1;
}
/*
unsigned char test (float* data) {
	determine_blobs();
	data[0] = blobs[0];
	data[1] = blobs[1];
	data[2] = 1.0;
}
*/

unsigned char localize (float* data) {
	numlost = 0;
	
	//if (recalibrate == 1) {
		determine_blobs();
		//recalibrate = 0;
	//}
	
	float posx = 0;
	float posy = 0;
	float r = 0;
	float phi = 0, beta = 0, angle = 0.0;
	
	float theta_top_left= (float) atan2((double) (14.5-2.483), (double) 10.563);
	float theta_top_right = (float) atan2((double) 11.655, (double) (14.5+8.741));
	float theta_bottom_left = (float) atan2((double)10.563, (double) (14.5+2.483));
	float theta_bottom_right = (float) atan2((double) 11.655, (double) (14.5-8.741));

	//for (int i = 0; i < 4; i++) {
		//if (x[i] >= 1023) numlost++;
	//}
	
	//if (numlost >= 2) {
		//recalibrate = 1;
		//return 0;
	//}
	
	if (x[top] < 1023 && x[bottom] < 1023) {
		//calculate and store pixel-space position and orientation
		posx = (float) ((float)(x[top]+x[bottom]))/2.0;
		posy = (float) ((float)(y[top]+y[bottom]))/2.0;
		
		
		//calculate and store angle
		angle = (float) atan2(((double) ((float)y[top]-(float)y[bottom])),((double) ((float)x[top]-(float)x[bottom])));
		phi = ((float) atan2((double) posy, (double) posx));
		
		r = (float) sqrt((double)((posx-rcenterx)*(posx-rcenterx) + (posy-rcentery)*(posy-rcentery)));
		
		

		//data[0] = rcenterx + r * (float) cos((double) (((3 * 3.14)/2) - angle - phi));
		//data[1] = rcentery + r * (float) sin((double) (((3 * 3.14)/2) - angle - phi));
		data[0] = posx- r*phi;
		data[1] = posy- r*phi;
		data[2] = angle * 180.0 / 3.14;
		data[3] = x[top];
		data[4] = y[top];
		data[5] = x[bottom];
		data[6] = y[bottom];
		data[7] = x[right];
		data[8] = y[right];
		data[9] = x[left];
		data[10] = y[left];
		
		return 1;
	}
	
	/*
	else if (x[top] < 1023 && x[left] < 1023) {
		beta = (float) atan2(((double) ((float)x[top]-(float)x[left])), ((double) ((float)y[top]-(float)y[left])));
		
		//this theta is constant, considering defining it as such
		//to save calculation time
		posx = rcenterx - (128.0/1023.0)*((float)x[top]-dcenter * (float)sin((double) (theta_top_left-beta)));
		posy = rcentery - (96.0/1023.0)*((float)y[top]+0.5*dcenter * (float)cos((double) (theta_top_left-beta)));
		
		angle = theta_top_left + beta;
		phi = -1.0 * (float) atan2((double)posy, (double)posx);
		
		r = (float) sqrt((double)((posx*posx)+(posy*posy)));
		
		data[0] = rcenterx - r * (float)cos((double) (angle-phi));
		data[1] = rcentery - r * (float)sin((double) (angle-phi));
		data[2] = angle;
		return 1;
	}
	
	else if (x[top] < 1023 && x[right] < 1023) {
		beta = (float)atan2((double) ((float)x[top]-(float)x[right]),((double) (float)y[top]-(float)y[right]));

		posx = rcenterx - (128.0/1023.0)*((float)x[top]+0.5*dcenter * (float)sin((double) (theta_top_right-beta)));
		posy = rcentery - (96.0/1023.0)*((float)y[top]-0.5*dcenter * (float)cos((double)  (theta_top_right-beta)));
		
		angle = -1.0 * theta_top_right - beta;
		phi = -1.0 * (float) atan2((double)posy,(double)posx);
		
		r = (float) sqrt((double) ((posx*posx)+(posy*posy)));
		
		data[0] = rcenterx - r * (float)cos((double) (angle - phi));
		data[1] = rcentery - r * (float)sin((double) (angle - phi));
		data[2] = angle;
		return 1;
	}
	
	else if (x[bottom] < 1023 && x[left] < 1023) {
		beta = (float)atan2((double) ((float)x[bottom]-(float)x[left]),(double) ((float)y[bottom]-(float)y[left]));

		posx = rcenterx - (128.0/1023.0)*((float)x[bottom]+dcenter*(float)sin((double) (theta_bottom_left-beta)));
		posy = rcentery - (96.0/1023.0)*((float)y[bottom]-dcenter*(float)cos((double) (theta_bottom_left-beta)));
		
		angle = -1.0 *3.14/2.0 + theta_bottom_left - beta;
		phi = -1.0 * (float)atan2((double)posy,(double)posx);
		
		r = (float)sqrt((double) ((posx*posx)+(posy*posy)));
		
		data[0] = rcenterx - r * (float)cos((double) (angle - phi));
		data[1] = rcentery - r * (float)sin((double) (angle - phi));
		data[2] = angle;
		return 1;
	}
	
	else if (x[bottom] < 1023 && x[right] < 1023) {
		beta = (float)atan2((double) ((float)x[bottom]-(float)x[right]), (double) ((float)y[bottom]-(float)y[right]));

		posx = rcenterx - (128.0/1023.0)*((float)x[bottom]+0.5*dcenter*(float)sin((double) (theta_bottom_right-beta)));
		posy = rcentery - (96.0/1023.0)*(y[bottom]-0.5*dcenter*(float)cos((double) (theta_bottom_right-beta)));
		
		angle = theta_bottom_right - beta;
		phi = -1.0 * (float)atan2((double)posy,(double)posx);
		
		r = (float)sqrt((double) ((posx*posx)+(posy*posy)));
		
		data[0] = rcenterx - r * (float)cos((double)(angle - phi));
		data[1] = rcentery - r * (float)sin((double)(angle - phi));
		data[2] = angle;
		return 1;
	}
	
	return 0;
	*/
}