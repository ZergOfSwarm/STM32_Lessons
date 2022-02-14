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

/************************ CGI HANDLER ***************************/
const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]); // 1. For LED

const tCGI FORM_CGI = {"/form.cgi", CGIForm_Handler}; // Создаем структуру CGI
//const tCGI LED_CGI = {"/led.cgi", CGILED_Handler}; // 2. For LED создаем структуру CGI

char name[30];

//tCGI CGI_TAB[2]; // 3. For LED создадим массив для LED CGI


const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 0)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "fname") == 0)  // if the fname string is found
			{
				memset(name, '\0', 30);
				strcpy(name, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "lname") == 0)  // if the fname string is found
			{
				strcat(name, " ");
				strcat(name, pcValue[i]);
			}
		}
	}

	return "/cgiform.html";
}
/************************ END CGI HANDLER ***************************/

const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 1)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "fname") == 0)  // if the fname string is found
			{
				memset(name, '\0', 30);
				strcpy(name, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "lname") == 0)  // if the lname string is found
			{
				strcat(name, " "); // Сконкатенируем fname и lname разделяя пробелом.
				strcat(name, pcValue[i]);
			}
		}
	}

	return "/cgiled.html";
}

void http_server_init (void)
{
	httpd_init();

	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 3);
	// 4. For LED Перед созданиме CGI_handle нам нужно назначить
	//CGI_TAB[0] = FORM_CGI; // CGI структуру
	//CGI_TAB[1] = LED_CGI;  // в массив

	http_set_cgi_handlers (&FORM_CGI, 1);
//	http_set_cgi_handlers (CGI_TAB, 2); // 5. И теперь мы передадим структуру массива в CGI_handle!
}
