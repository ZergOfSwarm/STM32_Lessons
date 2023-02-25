/*
 * db.h
 *
 *  Created on: Dec 14, 2022
 *      Author: anton
 */

#ifndef INC_DB_H_
#define INC_DB_H_

#define NUMPIN 89 // колличество пинов
#define NUMPINTOPIN 89 // колличество pin to pin


#include "stdio.h"
#include "stm32f7xx_hal.h"

struct dbPinsConf {    // Создали структуру с необходимым набором типов элиментов.
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
	short event;     // Event (StateChanged - 0, Pressed -1, Released - 2, Repeat - 3, Hold - 4, LongClick - 5, Click - 6, DoubleClick - 7)
	short act;       // Action (No action - 0, On - 1,  Off - 2, Toggle - 3, Value - 4, IncValue - 5)
	short parametr;  // Parameter value for dimmer (0-255), value for IncValue (-255 to 255).
	int timeout;     // Timeout (ms)
	char condit[30]; // Condition

};

struct dbPinsInfo { // Создали структуру с необходимым набором типов элиментов для PIN's.
	char pins[5];
	char port[2];
	int number;
	uint16_t hal_pin;
	GPIO_TypeDef* gpio_name;
};




#endif /* INC_DB_H_ */
