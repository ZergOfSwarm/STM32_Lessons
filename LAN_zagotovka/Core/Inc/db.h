/*
 * db.h
 *
 *  Created on: 3 мая 2022 г.
 *      Author: denis
 */

#ifndef INC_DB_H_
#define INC_DB_H_

#define numpin 58 // колличество пинов
#define numpintopin 70 // колличество связей pin to pin

struct intoout {
	int idin;
	int idout;
	int flag;
};



struct table {    // Создали структуру с необходимым набором типов элиментов.
	int topin;		// Type of pins: 0 - ON; 1 - buttons; 2 - relay
	int pwm;		// PWM frequency
	int on;			// ON
	int istate;		// Invert state
	int dvalue;		// Dimmer value
	int ponr;		// Power on restore
	char ptype[20]; // Pullup type
	int binter; 	// Bounce interval
	int hinter; 	// Hold interval
	int repeat; 	// Repeat
	int rinter; 	// Repeat interval
	int dcinter;	// Double-click interval
	int pclick; 	// Prevent Click
	char info[30];	// Info
	int onoff;		// On | Off
};

struct tablepins { // Создали структуру с необходимым набором типов элиментов для PIN's.
	char pins[5];
	char port[2];
	int number;
	uint32_t hal_pin;
	uint32_t gpio_name;
};


extern struct tablepins tp[numpin];

struct table T[numpin];

struct intoout TO[numpintopin];

#endif /* INC_DB_H_ */
