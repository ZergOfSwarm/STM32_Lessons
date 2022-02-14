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

int indx = 0; // Эта переменная будет отображаться Web server
/* we will use character "x", "y","z" as tag for SSI */
char const* TAGCHAR[]={"x", "y", "z"};
char const** TAGS=TAGCHAR;

uint16_t ssi_handler (int iIndex, char *pcInsert, int iInsertLen) // iIndex получает индекс из массива тегов.
{
	switch (iIndex) {
		case 0:
			indx+=1;
			sprintf(pcInsert, "%d", indx); // pcInsert -указатель на данные которые мы посылаем на web server.
			return strlen(pcInsert); //
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
/*---------------------------------------------------------------------------------*/
		// Если нужно добавить еще вывод данных на web site то, просто добавь
//			switch (iIndex) {
//					case 0:
//						adc_get_value; // Добавим к примеру вывод аналогового пина.
//						sprintf(pcInsert, "%d", adc_get_value); // pcInsert -указатель на данные которые мы посылаем на web server.
//						return strlen(pcInsert); //
//						break;
/*---------------------------------------------------------------------------------*/
	return 0;
}


void http_server_init (void) // Инициализация hhtp сервера.
{
	httpd_init();

	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 3); // Где 3-это количество тегов.
}
