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

int rcenterx = 128/2;
int rcentery = 96/2;

unsigned int x[4] = {0, 0, 0, 0};
unsigned int y[4] = {0, 0, 0, 0};
	
float dist = 0;
float distmax = 0, distmin = 1023;
float dcenter = 0;
int far1 = 0, far2 = 0, close1 = 0, close2 = 0;

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
	while (exit == 0) {
		getData(x, y);
		if (x[0] < 1023 && x[1] < 1023 && x[2] < 1023 && x[3] < 1023) {
			exit = 1;
			for (int i = 0; i < 4; i++) {
				for (int j = i; j < 4; j++) {
					dist = sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
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
				left = 10 - top - bottom - right;
			}
			else if (far1 == close2) {
				top = far1;
				bottom = far2;
				right = close1;
				left = 10 - top - bottom - right;
			}
			else if (far2 == close1) {
				top = far2;
				bottom = far1;
				right = close2;
				left = 10 - top - bottom - right;
			}
			else {
				top = far2;
				bottom = far1;
				right = close1;
				left = 10 - top - bottom - right;
			}
		}
	}
	return 1;
}

unsigned char local_init() {
	m_wii_open();
	
	m_wii_read(blobs);

	//Determine indices of all top, bottom, left, right points
	determine_blobs();
	
	return 1;
}

unsigned char localize (float* data) {
	m_wii_read(blobs);
	determine_blobs();
	float posx = 0;
	float posy = 0;
	float r = 0;
	float phi = 0, beta = 0, angle = 0;
	if (x[top] < 1023 && x[bottom] < 1023) {
		//calculate and store pixel-space position and orientation
		posx = ((128.0/1023.0)*(x[top]+x[bottom])/2-rcenterx);
		posy = ((96.0/1023.0)*(y[top]+y[bottom])/2-rcentery);
		
		//calculate and store angle
		angle = atan2(x[top]-x[bottom],y[top]-y[bottom]);
		phi = -1 * atan2(posy, posx);
		
		r = sqrt((posx*posx + posy*posy));
		
		data[0] = rcenterx - r * cos(angle - phi);
		data[1] = rcentery + r * sin(angle - phi);
		data[2] = angle;
		return 1;
	}
	
	else if (x[top] < 1023 && x[left] < 1023) {
		beta = atan2(x[top]-x[left], y[top]-y[left]);
		
		//this theta is constant, considering defining it as such
		//to save calculation time
		theta=atan2(14.5-2.483,10.563);
		posx = rcenterx - (128.0/1023.0)*(x[top]-dcenter*sin(theta-beta));
		posy = rcentery - (96.0/1023.0)*(y[top]+0.5*dcenter*cos(theta-beta));
		
		angle = theta + beta;
		phi = -1 * atan2(posy, posx);
		
		r = sqrt((posx*posx)+(posy*posy));
		
		data[0] = rcenterx - r * cos(angle-phi);
		data[1] = rcentery - r * sin(angle-phi);
		data[2] = angle;
		return 1;
	}
	
	else if (x[top] < 1023 && x[right] < 1023) {
		beta = atan2(x[top]-x[right],y[top]-y[right]);
		theta = atan2(11.655,14.5+8.741);
		posx = rcenterx - (128.0/1023.0)*(x[top]+0.5*dcenter*sin(theta-beta));
		posy = rcentery - (96.0/1023.0)*(y[top]-0.5*dcenter*cos(theta-beta));
		
		angle = -1 * theta - beta;
		phi = -1 * atan2(posy,posx);
		
		r = sqrt((posx*posx)+(posy*posy));
		
		data[0] = rcenterx - r * cos(angle - phi);
		data[1] = rcentery - r * sin(angle - phi);
		data[2] = angle;
		return 1;
	}
	
	else if (x[bottom] < 1023 && x[left] < 1023) {
		beta = atan2(x[bottom]-x[left],y[bottom]-y[left]);
		theta = atan2(10.563,14.5+2.483);
		posx = rcenterx - (128.0/1023.0)*(x[bottom]+dcenter*sin(theta-beta));
		posy = rcentery - (96.0/1023.0)*(y[bottom]-dcenter*cos(theta-beta));
		
		angle = -1 *3.14/2 + theta - beta;
		phi = -1 * atan2(posy,posx);
		
		r = sqrt((posx*posx)+(posy*posy));
		
		data[0] = rcenterx - r * cos(angle - phi);
		data[1] = rcentery - r * sin(angle - phi);
		data[2] = angle;
		return 1;
	}
	
	else if (x[bottom] < 1023 && x[right] < 1023) {
		beta = atan2(x[bottom]-x[right],y[bottom]-y[right]);
		theta = atan2(11.655,14.5-8.741);
		posx = rcenterx - (128.0/1023.0)*(x[bottom]+0.5*dcenter*sin(theta-beta));
		posy = rcentery - (96.0/1023.0)*(y[bottom]-0.5*dcenter*cos(theta-beta));
		
		angle = theta - beta;
		phi = -1 * atan2(posy,posx);
		
		r = sqrt((posx*posx)+(posy*posy));
		
		data[0] = rcenterx - r * cos(angle - phi);
		data[1] = rcentery - r * sin(angle - phi);
		data[2] = angle;
		return 1;
	}
	return 0;
}