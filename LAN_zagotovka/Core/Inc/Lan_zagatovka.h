/*
 * Lan_zagatovka.h
 *
 *  Created on: 14 авг. 2022 г.
 *      Author: Denis
 */

#ifndef INC_LAN_ZAGATOVKA_H_
#define INC_LAN_ZAGATOVKA_H_
#include <stdint.h>
#include "main.h"
#define numpin 58 // колличество пинов 75
#define numpintopin 70 // колличество pin to pin

void http_server_init (void);

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

struct time_table { // Создали структуру с необходимым набором типов элиментов для таймера.
	/* Variable of TIMERS */
	short id;          // Для получения данных из другой странички
	char pins[5];      // Для получения данных из другой странички
	short imm_ly;      // Immediately (1|0)
	int s_dt;          // Start DATE and TIME in UNIX format.
	int e_dt;          // End DATE and TIME in UNIX format.
	int t_repeat;      // Repeat for timers TIME in UNIX format.
	short t_action;    // Action, 0 - OFF; 1 - ON; 2 - TOGGLE for timers
	char t_group[10];  // Groups for timers
	int sunrise;       // Time of sunrise in UNIX format.
	int sunset;        // Time of sunset in UNIX format.
	char t_info[15];   // Info for timers
	short t_onoff;     // On | Off for timers
	short t_rept1;     // Для отображения числа в поле Repeat на страничке edit_timers.shtml
	short t_rept2;     // Для отображения выбранной величины времени (сек. мин...) в поле Repeat на страничке edit_timers.shtml
};



struct Str_table { // Создали структуру для страницы settings
double lon_de;    // Longitude / Долгота
double lat_de;    // Latitude / Широта
// Настройки MQTT
int mqtt_prt;        // Your MQTT broker port (default port is set to 1883)
char mqtt_clt[10];   // Device's unique identifier.
char mqtt_usr[10];   // Имя пользователя для авторизации
char mqt_pswd[15];  // Пароль для авторизации
char mqtt_tpc[15];   // Unique identifying topic for your device (kitchen-light) It is recommended to use a single word for the topic.
char mqt_ftpc[15];   // Полный топик for example lights/%prefix%/%topic%/
// Настройки IP адреса
short ip_addr0; // IP адрес
short ip_addr1; // IP адрес
short ip_addr2; // IP адрес
short ip_addr3; // IP адрес
short sb_mask0;// Маска сети
short sb_mask1;// Маска сети
short sb_mask2;// Маска сети
short sb_mask3;// Маска сети
short gateway0; // Шлюз
short gateway1; // Шлюз
short gateway2; // Шлюз
short gateway3; // Шлюз
short mqt_hst0; // Your MQTT broker address or IP
short mqt_hst1; // Your MQTT broker address or IP
short mqt_hst2; // Your MQTT broker address or IP
short mqt_hst3; // Your MQTT broker address or IP
};


struct tablepins { // Создали структуру с необходимым набором типов элиментов для PIN's.
	char pins[5];
	char port[2];
	int number;
	uint16_t hal_pin;
	//uint32_t gpio_name; -> он говорит, что ты пытаешься в поле uint32_t запихать указатель на тип структуры.
	GPIO_TypeDef* gpio_name;
};

extern struct tablepins tp[numpin];
struct table T[numpin];
struct Str_table Settings; // для странички "Settings".
struct time_table Tt[50]; // Для странички "edit_timers.shtml"

struct intoout TO[numpintopin];


#endif /* INC_LAN_ZAGATOVKA_H_ */
