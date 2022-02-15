/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */


#include"http_ssi.h"
#include "string.h"
#include "stdio.h"

#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"

#include "stm32f4xx_hal.h"
/************************ SSI HANDLER ***************************/
int indx = 0;
/* we will use character "x", "y","z" as tag for SSI */
char const* TAGCHAR[]={"x", "y", "z"};
char const** TAGS=TAGCHAR;

uint16_t ssi_handler (int iIndex, char *pcInsert, int iInsertLen)
{
	switch (iIndex) {
		case 0:
			indx+=1;
			sprintf(pcInsert, "%d", indx);
			return strlen(pcInsert);
			break;
		case 1:
			indx+=1;
			sprintf(pcInsert, "%d", indx);
			return strlen(pcInsert);
			break;
		case 2:
			indx+=1;
			sprintf(pcInsert, "%d", indx);
			return strlen(pcInsert);
			break;
		default :
			break;
	}

	return 0;
}

/************************ CGI HANDLER for FORM ***************************/
const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *CGI_LEDs_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]); // 1. For LED

const tCGI FORM_CGI = {"/form.cgi", CGIForm_Handler}; // Создаем структуру CGI
const tCGI LED_CGI = {"/leds.cgi", CGI_LEDs_Handler}; // 2. For LED создаем структуру CGI  (в папке fs файл "cgi_leds.html" стр.8)

char name[30];
char leds[3]; // +1 для пробела!
tCGI CGI_TAB[2]; // 3. For LED создадим массив для LED CGI

const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 0)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "fname") == 0)  // if the fname string is found
			{
				memset(name, '\0', 30);  // Очищаем массив перед записью новых данных.
				strcpy(name, pcValue[i]); // Сохраняем значение в переменную.
			}

			else if (strcmp(pcParam[i], "lname") == 0)  // if the lname string is found
			{
				strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
			}
		}
	}

	return "/cgiform.html"; // Здесь пропишем имя файла из которого берем переменные.
}

/************************ CGI HANDLER for LED ***************************/
const char *CGI_LEDs_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 1)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "led_1") == 0)  // if the fname string is found
			{
				memset(leds, '\0', 3);   // Очищаем массив перед записью новых данных. Цифра 3 количество елиментов в массиве которое нужно очистить.
				strcpy(leds, pcValue[i]); // Сохраняем значение в переменную.
			}

			else if (strcmp(pcParam[i], "led_2") == 0)  // if the lname string is found
			{
				strcat(leds, " "); // Сконкатенируем fname и lname разделяя пробелом.
				strcat(leds, pcValue[i]); // Сохраняем значение в переменную.
			}
		}
	}

	return "/cgi_leds.html"; // Здесь пропишем имя файла из которого берем переменные.
}
/************************ END LED CGI HANDLER ***************************/
void http_server_init (void)
{
	httpd_init();

	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 3);
	// 4. For LED Перед созданиме CGI_handle нам нужно назначить
	CGI_TAB[0] = FORM_CGI; // CGI структуру
	CGI_TAB[1] = LED_CGI;  // в массив

	//http_set_cgi_handlers (&FORM_CGI, 1); // Это пример когда у нас всего один "Handler" пример из видео.
	http_set_cgi_handlers (CGI_TAB, 2); // 5. И теперь мы передадим структуру массива в CGI_handle!
}
