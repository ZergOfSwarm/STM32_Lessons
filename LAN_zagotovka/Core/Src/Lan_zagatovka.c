/*
 * Lan_zagatovka.c
 *
 *  Created on: 14 авг. 2022 г.
 *      Author: Denis
 */

#include <Lan_zagatovka.h>
#include <stdlib.h> // для парсинга
#include "string.h"
#include "stdio.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "stm32f4xx_hal.h"

//#define A_PORT GPIOA

/************************ SSI HANDLER ***************************/
int indx1 = 0;
int y;
char pins[5] = "0"; // для странички edit_timers.shtml
short t_id = 0;   // для странички edit_timers.shtml
short tmp_id = 0; // Переменная для временной "ID"

//char json[2500];
char bufervar[10500]; // 12500
char readvar[1000];
//char arrline[400];

struct tablepins tp[numpin] = {
		{"PA0", "A", 0, GPIO_PIN_0,GPIOA},
		{"PA4","A", 4, GPIO_PIN_4, GPIOA},
		{"PA5", "A", 5, GPIO_PIN_5, GPIOA},
		{"PA6", "A", 6, GPIO_PIN_6, GPIOA},
		{"PA8", "A", 8, GPIO_PIN_8, GPIOA},
		{"PA9", "A", 9, GPIO_PIN_9, GPIOA},
		{"PA10", "A", 10, GPIO_PIN_10,GPIOA},
		{"PA11", "A", 11, GPIO_PIN_11, GPIOA},
		{"PA12", "A",12, GPIO_PIN_12, GPIOA},
		{"PA15", "A", 15, GPIO_PIN_15, GPIOA},
		{"PB0", "B", 0, GPIO_PIN_0,GPIOB},
		{"PB1", "B", 1, GPIO_PIN_1, GPIOB},
		{"PB3", "B", 3, GPIO_PIN_3, GPIOB},
		{"PB5", "B", 5, GPIO_PIN_5, GPIOB},
		{"PB6", "B", 6, GPIO_PIN_6, GPIOB},
		{"PB7", "B", 7, GPIO_PIN_7,GPIOB},
		{"PB8", "B", 8, GPIO_PIN_8, GPIOB},
		{"PB9", "B", 9, GPIO_PIN_9, GPIOB},
		{"PB10", "B", 10, GPIO_PIN_10, GPIOB},
		{"PB14", "B", 14, GPIO_PIN_14, GPIOB},
		{"PB15", "B", 15,GPIO_PIN_15, GPIOB},
		{"PC0", "C", 0, GPIO_PIN_0, GPIOC},
		{"PC2", "C", 2, GPIO_PIN_2, GPIOC},
		{"PC3", "C", 3, GPIO_PIN_3,GPIOC},
		{"PC6", "C", 6, GPIO_PIN_6, GPIOC},
		{"PC7", "C", 7, GPIO_PIN_7, GPIOC},
		{"PC13", "C", 13, GPIO_PIN_13, GPIOC},
		{"PD0", "D", 0, GPIO_PIN_0, GPIOD},
		{"PD1", "D", 1, GPIO_PIN_1,GPIOD},
		{"PD3", "D", 3, GPIO_PIN_3, GPIOD},
		{"PD4", "D", 4, GPIO_PIN_4, GPIOD},
		{"PD5", "D", 5, GPIO_PIN_5, GPIOD},
		{"PD6", "D", 6, GPIO_PIN_6, GPIOD},
		{"PD7", "D", 7, GPIO_PIN_7,GPIOD},
		{"PD8", "D", 8, GPIO_PIN_8, GPIOD},
		{"PD9", "D", 9, GPIO_PIN_9, GPIOD},
		{"PD10", "D", 10, GPIO_PIN_10, GPIOD},
		{"PD11", "D", 11, GPIO_PIN_11, GPIOD},
		{"PD12", "D", 12,GPIO_PIN_12, GPIOD},
		{"PD13", "D", 13, GPIO_PIN_13, GPIOD},
		{"PD14", "D", 14, GPIO_PIN_14, GPIOD},
		{"PD15", "D", 15,GPIO_PIN_15, GPIOD},
		{"PE0", "E", 0, GPIO_PIN_0, GPIOE},
		{"PE1", "E", 1, GPIO_PIN_1, GPIOE},
		{"PE2", "E", 2, GPIO_PIN_2,GPIOE},
		{"PE3", "E", 3, GPIO_PIN_3, GPIOE},
		{"PE4", "E", 4, GPIO_PIN_4, GPIOE},
		{"PE5", "E", 5, GPIO_PIN_5, GPIOE},
		{"PE6", "E", 6, GPIO_PIN_6, GPIOE},
		{"PE7", "E", 7, GPIO_PIN_7,GPIOE},
		{"PE8", "E", 8, GPIO_PIN_8, GPIOE},
		{"PE9", "E", 9, GPIO_PIN_9, GPIOE},
		{"PE10", "E", 10, GPIO_PIN_10, GPIOE},
		{"PE11", "E", 11, GPIO_PIN_11, GPIOE},
		{"PE12", "E", 12, GPIO_PIN_12, GPIOE},
		{"PE13", "E", 13, GPIO_PIN_13, GPIOE},
		{"PE14", "E", 14, GPIO_PIN_14, GPIOE},
		{"PE15", "E", 15, GPIO_PIN_15, GPIOE} };

struct intoout TO[numpintopin];
struct table T[numpin];
struct time_table Tt[50];  // Для странички "edit_timers.shtml"
extern struct Str_table Settings;

// По умолчанию "Долгота" и "Широта"
struct Str_table Settings; // для странички "Settings"
//struct Str_table Settings = {46.3417, 42.2678}; // Longitude/Долгота и Latitude/Широта

struct Str_table Settings = {
		46.3417,
		42.2678,
		1883,
		"Mojordomo",
		"Denis",
		"password",
		"light",
		"Full_topic",
		192, 168, 18, 111,
		255, 255, 255, 0,
		192, 168, 18, 1,
		192, 168, 22, 50,
};

//struct time_table Tt= {  // Для странички "edit_timers.shtml"
//0,            // ID
//"",           // Pin
//1,            // "imm_ly" - immediately (1|0)
//1654332076,   // "s_dt" - Start DATE and TIME in UNIX format.
//1432332076,   // "e_dt" - End DATE and TIME in UNIX format.
//1,            // "t_repeat" - Repeat for timers, max is 32767
//0,            // "t_action" - Action, 0 - OFF; 1 - ON; 2 - TOGGLE for timers
//"test_grupe", // "t_group[30]" - Groups for timers
//1432332076,   // "sunrise" - Time of sunrise
//1432552070,   // "sunset" - Time of sunset
//"lights in kitchen",  // "t_info[30]" - Info for timers
//1             // "t_onoff" - On | Off for timers
//};

struct time_table Tt[50];

int variable3 = 0;
int variable4 = 0;
char name3[10];

static int variable = 0;

int jsongo = 0; // GET json type
int id = 0;     // GET tab
int idtab = 0;  // GET id

char jsonrp[300];// было 100

char listaction[400];

/* we will some tags for SSI */
char const *TAGCHAR[] = { "json", "formjson", "menu", "tjson", "tlon_de",
		"tlat_de", "mqtt_prt", "mqtt_clt", "mqtt_usr", "mqt_pswd", "mqtt_tpc",
		"mqt_ftpc", "ip_addr0", "ip_addr1", "ip_addr2", "ip_addr3", "sb_mask0",
		"sb_mask1", "sb_mask2", "sb_mask3", "gateway0", "gateway1", "gateway2",
		"gateway3", "mqt_hst0", "mqt_hst1", "mqt_hst2", "mqt_hst3", "s_dt",
		"e_dt", "id", "pins", "t_repeat", "t_rept1", "t_rept2", "t_action", "imm_ly",
		"sunrise", "sunset", "t_group", "t_info" };// 1. Добавил тэги

char const **TAGS = TAGCHAR;

int f = 0;

//printf("Longitude: %.20f\n\r", Settings.lon_de);
//printf("Latitude: %.20f\n\r",Settings.lat_de);

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen,
		u16_t current_tag_part, u16_t *next_tag_part) {

	//printf("%d \n", jsongo);
	switch (iIndex) {
	case 0:
#if LWIP_HTTPD_SSI_MULTIPART

		if (variable == numpin) {
			//default:
			variable = 0;
			//return strlen(pcInsert);
			break;
		} else {
			// json pin to pin (to buttoms json)
			if (jsongo == 1) {
				while (variable3 <= numpintopin - 1) {
					if (variable3 == 0) {
						strcat(listaction, "{");
					}
					if (variable == TO[variable3].idin
							&& TO[variable3].flag == 1) {
						//sprintf(name3, "\"%s\":[%d,%d],",tp[TO[variable3].idout].pins, TO[variable3].idout, variable3);
						sprintf(name3, "\"%s\":%d,",
								tp[TO[variable3].idout].pins, variable3);
						strcat(listaction, name3);// {"PA5":0,"PA6":1, в поле "Relay connection"
					}
//					int code = (listaction[strlen(listaction) - 1]);
//					//printf("tmp_x %d\r\n",tmp_x ); // 123 -	{ левая фигурная скобка
//					if (variable3 == numpintopin - 1 && strcmp((char*)code, "123") > 0) {
//						listaction[strlen(listaction) - 1] = '\0';
//					}
					if (variable3 == numpintopin - 1 && strcmp(listaction[strlen(listaction) - 1], (char*)'{') > 0) {
						listaction[strlen(listaction) - 1] = '\0';
					}
					if (variable3 == numpintopin - 1) {
						strcat(listaction, "}");
					}
					variable3++;
				}
				memset(name3, '\0', sizeof(name3));
				variable3 = 0;
			}

			if (variable == 0) {
				if (jsongo == 1) {
					// buttoms json
					sprintf(pcInsert,
							"[{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"pinact\":%s,\"info\":\"%s\",\"onoff\":%d},",
							T[variable].topin, variable, tp[variable].pins,
							T[variable].ptype, T[variable].binter,
							T[variable].hinter, T[variable].repeat,
							T[variable].rinter, T[variable].dcinter,
							T[variable].pclick, listaction, T[variable].info,
							T[variable].onoff);
				}
				if (jsongo == 2) {
					// relay json
					sprintf(pcInsert,
							"[{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d},",
							T[variable].topin, variable, tp[variable].pins,
							T[variable].ptype, T[variable].pwm, T[variable].on,
							T[variable].istate, T[variable].dvalue,
							T[variable].ponr, T[variable].info,
							T[variable].onoff);
				}
				if (jsongo == 3) {
					// select json
					sprintf(pcInsert,
							"[{\"id\":%d,\"pins\":\"%s\",\"topin\":%d},",
							variable, tp[variable].pins, T[variable].topin);

				}
			} else if (variable == numpin - 1) {
				if (jsongo == 1) {
					// buttoms json
					sprintf(pcInsert,
							"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"pinact\":%s,\"info\":\"%s\",\"onoff\":%d}]",
							T[variable].topin, variable, tp[variable].pins,
							T[variable].ptype, T[variable].binter,
							T[variable].hinter, T[variable].repeat,
							T[variable].rinter, T[variable].dcinter,
							T[variable].pclick, listaction, T[variable].info,
							T[variable].onoff);
				}
				if (jsongo == 2) {
					// relay json
					sprintf(pcInsert,
							"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d}]",
							T[variable].topin, variable, tp[variable].pins,
							T[variable].ptype, T[variable].pwm, T[variable].on,
							T[variable].istate, T[variable].dvalue,
							T[variable].ponr, T[variable].info,
							T[variable].onoff);
				}
				if (jsongo == 3) {
					// select json
					sprintf(pcInsert,
							"{\"id\":%d,\"pins\":\"%s\",\"topin\":%d}]",
							variable, tp[variable].pins, T[variable].topin);
				}
			} else {
				if (jsongo == 1) {
					// buttoms json
					sprintf(pcInsert,
							"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"pinact\":%s,\"info\":\"%s\",\"onoff\":%d},",
							T[variable].topin, variable, tp[variable].pins,
							T[variable].ptype, T[variable].binter,
							T[variable].hinter, T[variable].repeat,
							T[variable].rinter, T[variable].dcinter,
							T[variable].pclick, listaction, T[variable].info,
							T[variable].onoff);
				}
				if (jsongo == 2) {
					// relay json
					sprintf(pcInsert,
							"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d},",
							T[variable].topin, variable, tp[variable].pins,
							T[variable].ptype, T[variable].pwm, T[variable].on,
							T[variable].istate, T[variable].dvalue,
							T[variable].ponr, T[variable].info,
							T[variable].onoff);
				}
				if (jsongo == 3) {
					// select json
					sprintf(pcInsert,
							"{\"id\":%d,\"pins\":\"%s\",\"topin\":%d},",
							variable, tp[variable].pins, T[variable].topin);
				}
			}
			/***********************************************************************/
			memset(listaction, '\0', sizeof(listaction));
			*next_tag_part = variable;
			//printf("*next_tag_part = %d\n",*next_tag_part);
			variable++;

			//strcat(pcInsert, "]");
			return strlen(pcInsert);
			break;
		}

#else
		printf("LWIP_HTTPD_SSI_MULTIPART disabled /n");
#endif
		break;
	case 1:
		if (idtab == 1) {
			// buttoms
			sprintf(pcInsert,
					"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"info\":\"%s\",\"onoff\":%d}",
					T[id].topin, id, tp[id].pins, T[id].ptype, T[id].binter,
					T[id].hinter, T[id].repeat, T[id].rinter, T[id].dcinter,
					T[id].pclick, T[id].info, T[id].onoff);
		}
		if (idtab == 2) {
			// relay
			sprintf(pcInsert,
					"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d}",
					T[id].topin, id, tp[id].pins, T[id].ptype, T[id].pwm,
					T[id].on, T[id].istate, T[id].dvalue, T[id].ponr,
					T[id].info, T[id].onoff);
		}
		if (idtab == 3) {
			// connection
			sprintf(jsonrp, "{");
			strcat(bufervar, jsonrp);
			while (variable <= numpin - 1) {
				if (T[variable].topin == 2) {
					sprintf(jsonrp, "\"%s\":%d,", tp[variable].pins, variable);
					strcat(bufervar, jsonrp);
					//printf("%s \r\n",  jsonrp);
				}
				variable++;
			}
			bufervar[strlen(bufervar) - 1] = '\0';
			sprintf(jsonrp, "}");
			strcat(bufervar, jsonrp);
			variable = 0;
			sprintf(pcInsert, "{\"id\":%d,\"pins\":\"%s\",\"rpins\":%s}", id,
					tp[id].pins, bufervar);
		}
		memset(jsonrp, '\0', sizeof(jsonrp));
		memset(bufervar, '\0', sizeof(bufervar));
		return strlen(pcInsert);
		break;
	case 2:
		sprintf(pcInsert,
				"<a href=\"index.shtml\">Home</a> | <a href=\"select.shtml\">Select pin</a> | <a href=\"tab1.shtml\">Buttom pin</a> | <a href=\"tab2.shtml\">Relay pin</a> | <a href=\"timers.shtml\">Timers</a> | <a href=\"settings.shtml\">Settings</a>");
		return strlen(pcInsert);
		break;
	case 4: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%.6f", Settings.lon_de);
		return strlen(pcInsert);
		break;
	case 5: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%.6f", Settings.lat_de);
		return strlen(pcInsert);
		break;
	case 6: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.mqtt_prt);
		return strlen(pcInsert);
		break;
	case 7: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Settings.mqtt_clt);
		return strlen(pcInsert);
		break;
	case 8: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Settings.mqtt_usr);
		return strlen(pcInsert);
		break;
	case 9: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Settings.mqt_pswd);
		return strlen(pcInsert);
		break;
	case 10: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Settings.mqtt_tpc);
		return strlen(pcInsert);
		break;
	case 11: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Settings.mqt_ftpc);
		return strlen(pcInsert);
		break;
	case 12: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.ip_addr0);
		//printf("ip_addr0 = \%d\n",Settings.ip_addr0);
		return strlen(pcInsert);
		break;
	case 13: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.ip_addr1);
		return strlen(pcInsert);
		break;
	case 14: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.ip_addr2);
		return strlen(pcInsert);
		break;
	case 15: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.ip_addr3);
		return strlen(pcInsert);
		break;
	case 16: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.sb_mask0);
		//printf("sb_mask0 = \%d\n",Settings.sb_mask0);
		return strlen(pcInsert);
		break;
	case 17: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.sb_mask1);
		//printf("sb_mask1 = \%d\n",Settings.sb_mask1);
		return strlen(pcInsert);
		break;
	case 18: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.sb_mask2);
		return strlen(pcInsert);
		//printf("sb_mask2 = \%d\n",Settings.sb_mask2);
		break;
	case 19: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.sb_mask3);
		//printf("sb_mask3 = \%d\n",Settings.sb_mask3);
		return strlen(pcInsert);
		break;
	case 20: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.gateway0);
		//printf("gateway0 = \%d\n",Settings.gateway0);
		return strlen(pcInsert);
		break;
	case 21: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.gateway1);
		return strlen(pcInsert);
		break;
	case 22: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.gateway2);
		return strlen(pcInsert);
		break;
	case 23: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.gateway3);
		return strlen(pcInsert);
		break;
	case 24: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.mqt_hst0);
		return strlen(pcInsert);
		break;
	case 25: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.mqt_hst1);
		return strlen(pcInsert);
		break;
	case 26: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.mqt_hst2);
		return strlen(pcInsert);
		break;
	case 27: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%d", Settings.mqt_hst3);
		return strlen(pcInsert);
		break;
	case 28: // "SSI" для странички "edit_timers.shtml"
		sprintf(pcInsert, "%d", Tt[tmp_id].s_dt);
		return strlen(pcInsert);
		break;
	case 29: // "SSI" для странички "edit_timers.shtml"
		sprintf(pcInsert, "%d", Tt[tmp_id].e_dt);
		return strlen(pcInsert);
		break;
	case 30: // "SSI" для странички "edit_timers.shtml"
		sprintf(pcInsert, "%d", t_id);
		return strlen(pcInsert);
		break;
	case 31: // "SSI" для странички "edit_timers.shtml"
		sprintf(pcInsert, "%s", pins);
		return strlen(pcInsert);
		break;
	case 32: // "SSI" для странички "edit_timers.shtml"
		//printf("Tt[tmp_id].t_repeat = \%d\n",Tt[tmp_id].t_repeat);
		sprintf(pcInsert, "%d", Tt[tmp_id].t_repeat);
		return strlen(pcInsert);
		break;

	case 33: // Для странички "edit_timers.shtml"
		sprintf(pcInsert,"%d",Tt[tmp_id].t_rept1);
    	return strlen(pcInsert);
		break;
	case 34: // Для странички "edit_timers.shtml"
		if(Tt[tmp_id].t_rept2 == 0){
			strcat(listaction, "<option value=\"0\" selected>NONE</option>"); // Блок "option" генерируем для HTML
		}
		else{
			strcat(listaction, "<option value=\"0\">NONE</option>");
		}
		if (Tt[tmp_id].t_rept2 == 1) {
			strcat(listaction, "<option value=\"1\" selected>Seconds</option>");
		}
		else {
			strcat(listaction, "<option value=\"1\">Seconds</option>");
		}

		if (Tt[tmp_id].t_rept2 == 2) {
			strcat(listaction, "<option value=\"2\" selected>Minutes</option>");
		}
		else {
			strcat(listaction, "<option value=\"2\">Minutes</option>");
		}
		if (Tt[tmp_id].t_rept2 == 3) {
			strcat(listaction, "<option value=\"3\" selected>Hours</option>");
		}
		else {
			strcat(listaction, "<option value=\"3\">Hours</option>");
		}
		if (Tt[tmp_id].t_rept2 == 4) {
			strcat(listaction, "<option value=\"4\" selected>Days</option>");
		}
		else {
			strcat(listaction, "<option value=\"4\">Days</option>");
		}
		if (Tt[tmp_id].t_rept2 == 5) {
			strcat(listaction, "<option value=\"5\" selected>Weeks</option>");
		}
		else {
			strcat(listaction, "<option value=\"5\">Weeks</option>");
		}
		if (Tt[tmp_id].t_rept2 == 6) {
			strcat(listaction, "<option value=\"6\" selected>Months</option>");
		}
		else {
			strcat(listaction, "<option value=\"6\">Months</option>");
		}
		if(Tt[tmp_id].t_rept2 == 7) {
			strcat(listaction, "<option value=\"7\" selected>Years</option>");
		}
		else {
			strcat(listaction, "<option value=\"7\">Years</option>");
		}
		sprintf(pcInsert,"%s",listaction);
		memset(listaction, '\0', sizeof(listaction)); // Обнулим istaction
    	return strlen(pcInsert);
		break;
	case 35: // "SSI" для странички "edit_timers.shtml"
		if(Tt[tmp_id].t_action == 0){
			strcat(listaction, "<option value=\"0\" selected>OFF</option>"); // Блок "option" генерируем для HTML
		}
		else{
			strcat(listaction, "<option value=\"0\">OFF</option>");
		}
		if (Tt[tmp_id].t_action == 1) {
			strcat(listaction, "<option value=\"1\" selected>ON</option>");
		}
		else {
			strcat(listaction, "<option value=\"1\">ON</option>");
		}

		if (Tt[tmp_id].t_action == 2) {
			strcat(listaction, "<option value=\"2\" selected>TOGGLE</option>");
		}
		else {
			strcat(listaction, "<option value=\"2\">TOGGLE</option>");
		}
		sprintf(pcInsert,"%s",listaction);
		memset(listaction, '\0', sizeof(listaction)); // Обнулим istaction
    	return strlen(pcInsert);
		break;

	case 36: // "SSI" для странички "edit_timers.shtml"
		if(Tt[tmp_id].imm_ly == 1){
		sprintf(pcInsert, "%s", "checked");
		}
		else{
			sprintf(pcInsert, "%s", " ");
		}
		return strlen(pcInsert);
		break;

	case 37: // "SSI" для странички "edit_timers.shtml"
		if(Tt[tmp_id].sunrise == 1){
		sprintf(pcInsert, "%s", "checked");
		}
		else{
			sprintf(pcInsert, "%s", " ");
		}
		return strlen(pcInsert);
		break;
	case 38: // "SSI" для странички "edit_timers.shtml"
		if(Tt[tmp_id].sunset == 1){
		sprintf(pcInsert, "%s", "checked");
		}
		else{
			sprintf(pcInsert, "%s", " ");
		}
		return strlen(pcInsert);
		break;
	case 39: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Tt[tmp_id].t_group);
		return strlen(pcInsert);
		break;
	case 40: // "SSI" для странички "Settings"
		sprintf(pcInsert, "%s", Tt[tmp_id].t_info);
		return strlen(pcInsert);
		break;

	default:
		break;
	}
	return 0;
}

/************************ CGI HANDLER for FORM ***************************/
const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
const char* CGIForm_Handler1(int iIndex1, int iNumParams, char *pcParam[],char *pcValue[]);
const char* CGIForm_Handler2(int iIndex1, int iNumParams, char *pcParam[],char *pcValue[]);
const char* CGIForm_Handler3(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]); // Для странички Settings.shtml
const char* CGIForm_Handler4(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]); // Для странички Edit Timers

//const tCGI FORM_CGI = {"/json.shtml", CGIForm_Handler,"/form1.shtml", CGIForm_Handler1,"/form2.shtml", CGIForm_Handler2}; // Создаем структуру CGI

static const tCGI URL_TABLES[] = {
		{ "/json.shtml", CGIForm_Handler },
		{"/form1.shtml", CGIForm_Handler1 },
		{ "/form2.shtml", CGIForm_Handler1 },
		{ "/form3.shtml", CGIForm_Handler1 },
		{ "/tab1.shtml", CGIForm_Handler2 },
		{ "/settings.shtml",CGIForm_Handler3 },
		{ "/edit_timers.shtml", CGIForm_Handler4 }
};

const uint8_t CGI_URL_NUM = (sizeof(URL_TABLES) / sizeof(tCGI));

//tCGI CGI_TAB[3]; // 3. For LED создадим массив для LED CGI

const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	//printf("GET OK 1 \n");
	if (iIndex == 0) {
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "jsongo") == 0) // if the fname string is found
					{
				//memset(name, '\0', 30);  // Очищаем массив перед записью новых данных.
				//strcpy(name, pcValue[i]); // Сохраняем значение в переменную.
				jsongo = atoi(pcValue[i]);
			}
		}
	}
	return "/json.shtml"; //
}

const char* CGIForm_Handler1(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	//printf("GET 1 OK  \n");
	if (iIndex == 1 || iIndex == 2 || iIndex == 3) {
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "id") == 0)  // if the fname string is found
					{
				id = atoi(pcValue[i]);
				//printf("+++++++++ %d \n", id);
			}
			if (strcmp(pcParam[i], "tab") == 0)  // if the fname string is found
					{
				idtab = atoi(pcValue[i]);
				//printf("+++++++++ %d \n", idtab);
			}
		}
	}
	if (iIndex == 1) {
		return "/form1.shtml"; //
	} else if (iIndex == 2) {
		return "/form2.shtml";
	} else if (iIndex == 3) {
		return "/form3.shtml";
	} else {
		return "/404.html";
	}
}

const char* CGIForm_Handler2(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	//printf("GET OK 1 \n");
	if (iIndex == 4) {
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "del") == 0)  // if the fname string is found
					{
				id = atoi(pcValue[i]);
				TO[id].idout = 0;
				TO[id].idin = 0;
				TO[id].flag = 0;
			}
		}
	}
	return "/tab1.shtml"; // Здесь пропишем имя файла из которого берем переменные.
}

const char* CGIForm_Handler3(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	if (iIndex == 5) // для странички settings
			//printf("GET CGI_Handler 5 \n");
			{
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "tlon_de") == 0) // if the "tlat_de" string is found
					{
				//memset(&(Settings.lon_de), 0, sizeof(Settings.lon_de));  // Очищаем элимент структуры перед перед записью новых данных.
				//pcValue[i][8] = 0; // Ограничем переменую количеством символов!

				//printf("pcValue[i] is - %s\n\r", pcValue[i]);
				Settings.lon_de = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Settings.lon_de = atof(pcValue[i]); // Сохраняем значение в переменную.
				//printf("Length of line is - %d\n\r", strlen(pcValue[i]));
				//printf("Longitude - %.6F\n\r",Settings.lon_de);
			}
			if (strcmp(pcParam[i], "tlat_de") == 0) // if the "tlat_de" string is found
					{
				//memset(&(Settings.lat_de), 0, sizeof(Settings.lat_de));  // Очищаем элимент структуры перед перед записью новых данных.
				//pcValue[i][8] = 0; // Ограничем переменую количеством символов!

				//printf("pcValue[i] is - %s\n\r", pcValue[i]);
				Settings.lat_de = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Settings.lat_de = atof(pcValue[i]); // Сохраняем значение в переменную.
				//printf("Length of line is - %d\n\r", strlen(pcValue[i]));
				//printf("Latitude - %.6F\n\r",Settings.lat_de);
			}
			if (strcmp(pcParam[i], "mqtt_prt") == 0)  // if the string is found
					{
				Settings.mqtt_prt = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Settings.mqtt_prt = atoi(pcValue[i]); // Сохраняем значение в переменную.
			}
			if (strcmp(pcParam[i], "mqtt_clt") == 0)  // if the string is found
					{
				//printf("pcParam[i] = '%s'\n", pcParam[i]);
				//printf("pcValue[i] = '%s'\n", pcValue[i]);
				Settings.mqtt_clt[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				//printf("Settings.mqtt_clt = %s\n", Settings.mqtt_clt);
				//Settings.mqtt_clt = pcValue[i]; // Сохраняем значение в переменную.
				//strcpy(Settings.mqtt_clt, pcValue[i]);
				//printf("Settings.mqtt_clt = %s\n", Settings.mqtt_clt);
			}
			if (strcmp(pcParam[i], "mqtt_usr") == 0)  // if the string is found
					{
				Settings.mqtt_usr[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				//Settings.mqtt_usr[10] = pcValue[i]; // Сохраняем значение в переменную.
				strcpy(Settings.mqtt_usr, pcValue[i]);
			}
			if (strcmp(pcParam[i], "mqt_pswd") == 0)  // if the string is found
					{
				Settings.mqt_pswd[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				//Settings.mqt_pswd[15] = pcValue[i]; // Сохраняем значение в переменную.
				strcpy(Settings.mqt_pswd, pcValue[i]);
			}
			if (strcmp(pcParam[i], "mqtt_tpc") == 0)  // if the string is found
					{
				Settings.mqtt_tpc[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				//Settings.mqtt_tpc[15] = pcValue[i]; // Сохраняем значение в переменную.
				strcpy(Settings.mqtt_tpc, pcValue[i]);
			}
			if (strcmp(pcParam[i], "mqt_ftpc") == 0)  // if the string is found
					{
				Settings.mqt_ftpc[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				//Settings.mqt_ftpc[15] = pcValue[i]; // Сохраняем значение в переменную.
				strcpy(Settings.mqt_ftpc, pcValue[i]);
			}
			if (strcmp(pcParam[i], "ip_addr") == 0)  // if the string is found
					{
				//printf("pcParam[i] = '%s'\n", pcParam[i]);
				//printf("pcValue[i] = '%s'\n", pcValue[i]);
				char delim[] = ".";
				int k = 0;
				char *ptr = strtok(pcValue[i], delim);
				while (ptr != NULL) {
					if (k == 0) {
						//printf("k0 - %d\n", k);
						//printf("ptr - %s\n", ptr);
						Settings.ip_addr0 = 0;// Очищаем элимент структуры перед перед записью новых данных.
						Settings.ip_addr0 = atoi(ptr);	// Сохраняю в переменную
						//printf("Settings.ip_addr0 = %d\n", Settings.ip_addr0);
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 1) {
						//printf("k1 - %d\n", k);
						//printf("ptr - %s\n", ptr);
						Settings.ip_addr1 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.ip_addr1 = atoi(ptr); // Сохраняю в переменную
						//printf("Settings.ip_addr1 = %d\n", Settings.ip_addr1);
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 2) {
						//printf("k2 - %d\n", k);
						//printf("ptr - %s\n", ptr);
						Settings.ip_addr2 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.ip_addr2 = atoi(ptr); // Сохраняю в переменную
						//printf("Settings.ip_addr2 = %d\n", Settings.ip_addr2);
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 3) {
						//printf("k3 - %d\n", k);
						//printf("ptr - %s\n", ptr);
						Settings.ip_addr3 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.ip_addr3 = atoi(ptr); // Сохраняю в переменную
						//printf("Settings.ip_addr3 = %d\n", Settings.ip_addr3);
						ptr = strtok(NULL, delim);
						k++;
					}
					ptr = strtok(NULL, delim);
				}
			}
			if (strcmp(pcParam[i], "sb_mask") == 0)  // if the string is found
					{
				char delim[] = ".";
				int k = 0;
				char *ptr = strtok(pcValue[i], delim);

				while (ptr != NULL) {
					if (k == 0) {
						Settings.sb_mask0 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.sb_mask0 = atoi(ptr);  // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 1) {
						Settings.sb_mask1 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.sb_mask1 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 2) {
						Settings.sb_mask2 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.sb_mask2 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 3) {
						Settings.sb_mask3 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.sb_mask3 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					ptr = strtok(NULL, delim);
				}
			}
			if (strcmp(pcParam[i], "gateway") == 0)  // if the string is found
					{
				char delim[] = ".";
				int k = 0;
				char *ptr = strtok(pcValue[i], delim);
				while (ptr != NULL) {
					if (k == 0) {
						Settings.gateway0 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.gateway0 = atoi(ptr);  // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 1) {
						Settings.gateway1 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.gateway1 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 2) {
						Settings.gateway2 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.gateway2 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 3) {
						Settings.gateway3 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.gateway3 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					ptr = strtok(NULL, delim);
				}
			}
			if (strcmp(pcParam[i], "mqt_hst") == 0)  // if the string is found
					{
				char delim[] = ".";
				int k = 0;
				char *ptr = strtok(pcValue[i], delim);
				while (ptr != NULL) {
					if (k == 0) {
						Settings.mqt_hst0 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.mqt_hst0 = atoi(ptr);  // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 1) {
						Settings.mqt_hst1 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.mqt_hst1 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 2) {
						Settings.mqt_hst2 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.mqt_hst2 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					if (k == 3) {
						Settings.mqt_hst3 = 0; // Очищаем элимент структуры перед перед записью новых данных.
						Settings.mqt_hst3 = atoi(ptr); // Сохраняю в переменную
						ptr = strtok(NULL, delim);
						k++;
					}
					ptr = strtok(NULL, delim);
				}
			}
		}
	}
	return "/settings.shtml"; //
}

const char* CGIForm_Handler4(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) // Для странички Edit Timers
{
	//printf("iIndex %d\n", iIndex);
	if (iIndex == 6) // Для странички Edit Timers
			//printf("GET CGI_Handler 6 \n");
			{
		//printf("iNumParams %d\n",iNumParams);
		for (int i = 0; i < iNumParams; i++) {

			//printf("tmp_id %d\n",tmp_id);
			//printf("pcParam[i] = %s pcValue[i] %s\n", pcParam[i], pcValue[i]); // смотрим, что прилитело.
			// От формы GET запрос1
			if (strcmp(pcParam[i], "id_id") == 0) // if the "pins" string is found
					{
				tmp_id = atoi(pcValue[i]);
				//printf("tmp_id = %d\n", tmp_id);
				Tt[tmp_id].id = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].id = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("ID = %d\n", Tt[tmp_id].id);
			}
			// От формы GET запрос2
			if (strcmp(pcParam[i], "id") == 0)  // if the "id" string is found
					{
				tmp_id = atoi(pcValue[i]);
				t_id = 0; // Очищаем элимент структуры перед перед записью новых данных.
				t_id = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("id ID = %d\n", Tt[tmp_id].id);
				//printf("id = %d\n", t_id);
			}
			if (strcmp(pcParam[i], "s_dt") == 0 && tmp_id != 0) // if the "s_dt" string is found
					{
				Tt[tmp_id].s_dt = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].s_dt = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("s_dt ID = %d\n", Tt[tmp_id].id);
				//printf("Tt[tmp_id].s_dt = %d\n", Tt[tmp_id].s_dt);
			}
			if (strcmp(pcParam[i], "e_dt") == 0 && tmp_id != 0) // if the "e_dt" string is found
					{
				Tt[tmp_id].e_dt = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].e_dt = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("e_dt ID = %d\n", Tt[tmp_id].id);
				//printf("Tt[tmp_id].e_dt = %d\n", Tt[tmp_id].e_dt);
			}
			if (strcmp(pcParam[i], "pin_pin") == 0 && tmp_id != 0) // Переход от таблиц к форме
					{
				Tt[tmp_id].pins[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				strcpy(Tt[tmp_id].pins, pcValue[i]); // Сохраняю в переменную
				//printf("pin_pin ID = %d\n", Tt[tmp_id].id);
				//printf("PINS = %s\n", Tt[tmp_id].pins);
			}
			// От таблицы к форме GET запрос

			if (strcmp(pcParam[i], "pin") == 0) {
				//pins[0] = 0; // Очищаем элимент структуры перед перед записью новых данных.
				memset(&pins, '\0', sizeof(pins));
				strcpy(pins, pcValue[i]); // Сохраняю в переменную
				//printf("pin ID = %d\n", Tt[tmp_id].id);
				//printf("pin = %s\n", pins);
			}
			if (strcmp(pcParam[i], "h_repeat") == 0 && tmp_id != 0) // if the "t_repeat" string is found
					{
				Tt[tmp_id].t_repeat = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].t_repeat = atoi(pcValue[i]); // Сохраняю в переменную
//				printf("t_repeat ID = %d\n", Tt[tmp_id].id);
//				printf("Tt[tmp_id].t_repeat = %d\n", Tt[tmp_id].t_repeat);
			}
			if (strcmp(pcParam[i], "set_num") == 0 && tmp_id != 0) // if the "e_dt" string is found
					{
				Tt[tmp_id].t_rept1 = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].t_rept1 = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("t_rept1 ID = %d\n", Tt[tmp_id].id);
				//printf("Tt[tmp_id].t_rept1 = %d\n", Tt[tmp_id].t_rept1);
			}
			if (strcmp(pcParam[i], "repeat") == 0 && tmp_id != 0) // if the "e_dt" string is found
					{
				Tt[tmp_id].t_rept2 = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].t_rept2 = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("t_rept2 ID = %d\n", Tt[tmp_id].id);
				//printf("Tt[tmp_id].t_rept2 = %d\n", Tt[tmp_id].t_rept2);
			}
			if (strcmp(pcParam[i], "action") == 0 && tmp_id != 0) // if the "e_dt" string is found
					{
				Tt[tmp_id].t_action = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].t_action = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("t_action ID = %d\n", Tt[tmp_id].id);
				//printf("Tt[tmp_id].t_action = %d\n", Tt[tmp_id].t_action);
			}
			if (strcmp(pcParam[i], "immediately") == 0 && tmp_id != 0)  // if the "id" string is found
					{
				Tt[tmp_id].imm_ly = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].imm_ly = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("imm_ly ID = %d\n", Tt[tmp_id].id);
				//printf("imm_ly = %d\n", Tt[tmp_id].imm_ly);
			}
			if (strcmp(pcParam[i], "sunrise") == 0 && tmp_id != 0)  // if the "sunrise" string is found
					{
				Tt[tmp_id].sunrise = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].sunrise = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("imm_ly ID = %d\n", Tt[tmp_id].id);
				//printf("sunrise = %d\n", Tt[tmp_id].sunrise);
			}
			if (strcmp(pcParam[i], "sunset") == 0 && tmp_id != 0)  // if the "sunset" string is found
					{
				Tt[tmp_id].sunset = 0; // Очищаем элимент структуры перед перед записью новых данных.
				Tt[tmp_id].sunset = atoi(pcValue[i]); // Сохраняю в переменную
				//printf("sunset ID = %d\n", Tt[tmp_id].id);
				//printf("sunset = %d\n", Tt[tmp_id].sunset);
			}


			if (strcmp(pcParam[i], "t_group") == 0 && tmp_id != 0)  // if the string is found
					{
				//Tt[tmp_id].t_group[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				memset(&Tt[tmp_id].t_group , '\0', sizeof(Tt[tmp_id].t_group ));
				strcpy(Tt[tmp_id].t_group, pcValue[i]);// Сохраняем значение в переменную.
			}
			if (strcmp(pcParam[i], "t_info") == 0 && tmp_id != 0)  // if the string is found
					{
				//Tt[tmp_id].t_info[0] = '\0'; // Очищаем элимент структуры перед перед записью новых данных.
				memset(&Tt[tmp_id].t_info , '\0', sizeof(Tt[tmp_id].t_info ));
				strcpy(Tt[tmp_id].t_info, pcValue[i]);// Сохраняем значение в переменную.
				//printf("Tt[tmp_id].t_info[15] = %s\n", Tt[tmp_id].t_info);
			}
		}
	}
	return "/edit_timers.shtml"; //
}

////////////////////////////// POST START //////////////////////////////////
static void *current_connection;
static void *valid_connection;
int http_post_content_len;
char urls[30] = { 0 };

err_t httpd_post_begin(void *connection, const char *uri,
		const char *http_request, uint16_t http_request_len, int content_len,
		char *response_uri, uint16_t response_uri_len, uint8_t *post_auto_wnd) {

	http_post_content_len = content_len;
	strncpy(urls, uri, 15);

	//printf("uri %s\r\n", uri);//////////////////////////////////

	if (current_connection != connection) {
		current_connection = connection;
		valid_connection = NULL;
		/* default page is "login failed" */
		//snprintf(response_uri, response_uri_len, "/cgi_leds.html");
		/* e.g. for large uploads to slow flash over a fast connection, you should
		 manually update the rx window. That way, a sender can only send a full
		 tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
		 We do not need to throttle upload speed here, so: */
		return ERR_OK;
	}
	return ERR_VAL;
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p) {
	//printf("OK POST 2 \r\n");
	//char data;
	if (current_connection == connection && p != NULL) {
		//data = p->payload;
		//printf("DATA %s \r\n", data);
		//strcat(bufervar, p->payload);
		//strncpy(readvar, p->payload, p->tot_len);
		strncpy(readvar, p->payload, p->len);
		//strcpy(readvar, p->payload);
		//printf("DATA %s \r\n", readvar);
		//printf("num %d \r\n", p->len);
		strcat(bufervar, readvar);
		pbuf_free(p);
		//memset(data, '\0', sizeof(data));
		return ERR_OK;
	}

	if (p != NULL) {
		//printf("Очищаем полученные данные! \r\n");
		pbuf_free(p);
	}
	return ERR_VAL;
}

void httpd_post_finished(void *connection, char *response_uri,
		u16_t response_uri_len) {
	//printf("OK POST 3 \r\n");

	char *token; // "&"
	char *token1;// "="
	char *token2;// "_"
	char *name1; // Для сравнения полученных тегов из uri
	int z = 0;
	int id;
	int variable1 = 0;
	int variable2;
	//char urlline[http_post_content_len];
	char urlline[1000] = { 0 };
	char name0[50] = { 0 };
	char nametoken[50] = { 0 };

	memset(name0, '\0', sizeof(name0));
	//memset(urlline, '\0', sizeof(urlline));
	strncpy(urlline, bufervar, http_post_content_len);
	//printf("%s \r\n", urlline);
//	printf("COUNT %d \r\n", http_post_content_len);

	char *rest = urlline;
	while ((token = strtok_r(rest, "&", &rest))) {
		//printf("%s\n", token);
		char *rest1 = token;
		strcat(name0, token); // решение с пустой строкой
		y = 0;
		while ((token1 = strtok_r(rest1, "=", &rest1))) {
			z = 0;
			if (y == 0) {
				//printf("NAME %s\n", token1);
				// получение id
				char *rest2 = token1;
				while ((token2 = strtok_r(rest2, "_", &rest2))) {
					if (z == 0) {
						name1 = token2;
					}
					if (z == 1) {
						id = atoi(token2);
						//printf("ID %d\n", id);
					}
					z++;
				}
			}
			if (y == 1) {
				// значение переменной
				//printf("VAL %s\n", token1);
				// пишем в структуру
				if (strcmp(name1, "ptype") == 0) {
					strcpy(T[id].ptype, token1);
				} else if (strcmp(name1, "topin") == 0) {
					T[id].topin = atoi(token1);
				} else if (strcmp(name1, "binter") == 0) {
					T[id].binter = atoi(token1);
				} else if (strcmp(name1, "hinter") == 0) {
					T[id].hinter = atoi(token1);
				} else if (strcmp(name1, "repeat") == 0) {
					T[id].repeat = atoi(token1);
				} else if (strcmp(name1, "rinter") == 0) {
					T[id].rinter = atoi(token1);
				} else if (strcmp(name1, "dcinter") == 0) {
					T[id].dcinter = atoi(token1);
				} else if (strcmp(name1, "pclick") == 0) {
					T[id].pclick = atoi(token1);
				} else if (strcmp(name1, "info") == 0) {
					//memset(T[id].info, '\0', sizeof(T[id].info));
					strcpy(T[id].info, token1);
				} else if (strcmp(name1, "onoff") == 0) {
					T[id].onoff = atoi(token1);
				} else if (strcmp(name1, "pwm") == 0) {
					T[id].pwm = atoi(token1);
				} else if (strcmp(name1, "on") == 0) {
					T[id].on = atoi(token1);
				} else if (strcmp(name1, "istate") == 0) {
					T[id].istate = atoi(token1);
				} else if (strcmp(name1, "dvalue") == 0) {
					T[id].dvalue = atoi(token1);
				} else if (strcmp(name1, "ponr") == 0) {
					T[id].ponr = atoi(token1);
				} else if (strcmp(name1, "idin") == 0) {
					//printf("idin------- %d \r\n", atoi(token1));
					while (variable1 <= numpintopin - 1) {
						if (TO[variable1].flag == 0) {
							TO[variable1].idin = atoi(token1);
							variable2 = variable1;
							TO[variable1].flag = 1;
							variable1 = numpintopin;
						}
						variable1++;
					}
					variable1 = 0;
				} else if (strcmp(name1, "idout") == 0) {
					TO[variable2].idout = atoi(token1);
					variable2 = 0;
					//printf("idout------- %d \r\n", atoi(token1));
					//////////////
//							while(variable1 <= 49){
//								printf("TO1 id %d --val %d \r\n",variable1 ,TO[variable1].idin);
//								printf("TO2 id %d --val %d \r\n",variable1 ,TO[variable1].idout);
//								variable1++;
//							}
//							variable1 = 0;
					//////////////
				}
			}
			y++;
		}
		// решение с пустой строкой
		sprintf(nametoken, "infofield_%d=", id); // Если поле пустое то его заполняем "null".
		if (strcmp(name0, nametoken) == 0) {
			strcpy(T[id].info, "null");
			//printf("Yes \n");
		}
		memset(name0, '\0', sizeof(name0));
		memset(nametoken, '\0', sizeof(nametoken));
		z++;
	}

	memset(urlline, '\0', sizeof(urlline));   // Очищаем urlline
	memset(bufervar, '\0', sizeof(bufervar)); // Очищаем bufervar
	//memset(json, '\0', sizeof(json));
	//printf("httpd_post_finished! \r\n");
	/* default page is "login failed" */
	//snprintf(response_uri, response_uri_len, "/tab.shtml");
	snprintf(response_uri, response_uri_len, urls);
	if (current_connection == connection) {
		if (valid_connection == connection) {
			/* login succeeded */
			//snprintf(response_uri, response_uri_len, "/tab.shtml");
			snprintf(response_uri, response_uri_len, urls);
		}
		current_connection = NULL;
		valid_connection = NULL;
	}
	connection = NULL;
}

//////////////////// POST  END ////////////////////////

void http_server_init(void) {
	httpd_init();
	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 41); // 2. Указал точное количество тегов.
	// 4. For LED Перед созданиме CGI_handle нам нужно назначить
	//CGI_TAB[0] = FORM_CGI; // CGI структуру
	//http_set_cgi_handlers (&FORM_CGI, 1); // Это пример когда у нас всего один "Handler" пример из видео.
	http_set_cgi_handlers(URL_TABLES, CGI_URL_NUM); // 5. И теперь мы передадим структуру массива в CGI_handle!
}
