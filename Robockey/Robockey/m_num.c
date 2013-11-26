/*
 * m_num.c
 *
 * Created: 11/19/2013 1:28:28 AM
 *  Author: Sean
 */ 

#define F_CPU 16000000UL
#define LOWERLEFT1 PORTG
#define LOWERLEFT2 4
#define MIDDLE1 PORTG
#define MIDDLE2 0
#define LOWERRIGHT1 PORTG
#define LOWERRIGHT2 6
#define BOTTOM1 PORTG
#define BOTTOM2 5
#define UPPERLEFT1 PORTG
#define UPPERLEFT2 1
#define TOP1 PORTG
#define TOP2 2
#define UPPERRIGHT1 PORTG
#define UPPERRIGHT2 3

#include <avr/io.h>
#include "m_general.h"
#include "m_port.h"

#define ADDR 0x20

void disp(int num) {
	switch (num) {
		case 1:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_clear(ADDR,TOP1,TOP2);
			m_port_clear(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_clear(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_clear(ADDR,BOTTOM1,BOTTOM2);
			m_port_clear(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 2:
			m_port_clear(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_clear(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_set(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 3:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_clear(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_clear(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 4:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_clear(ADDR,TOP1,TOP2);
			m_port_set(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_clear(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_clear(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 5:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_clear(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_set(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_clear(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
		break;
		case 6:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_clear(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_set(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_set(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 7:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_clear(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_clear(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_clear(ADDR,BOTTOM1,BOTTOM2);
			m_port_clear(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 8:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_set(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_set(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 9:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_set(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_clear(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_set(ADDR,MIDDLE1,MIDDLE2);
			break;
		case 0:
			m_port_set(ADDR,LOWERRIGHT1,LOWERRIGHT2);
			m_port_set(ADDR,UPPERRIGHT1,UPPERRIGHT2);
			m_port_set(ADDR,TOP1,TOP2);
			m_port_set(ADDR,UPPERLEFT1,UPPERLEFT2);
			m_port_set(ADDR,LOWERLEFT1,LOWERLEFT2);
			m_port_set(ADDR,BOTTOM1,BOTTOM2);
			m_port_clear(ADDR,MIDDLE1,MIDDLE2);
			break;
	}
}

void m_num_init() {
	m_bus_init();
	m_port_init(ADDR);
	m_port_set(ADDR,DDRG,0);
	m_port_set(ADDR,DDRG,1);
	m_port_set(ADDR,DDRG,2);
	m_port_set(ADDR,DDRG,3);
	m_port_set(ADDR,DDRG,4);
	m_port_set(ADDR,DDRG,5);
	m_port_set(ADDR,DDRG,6);
}