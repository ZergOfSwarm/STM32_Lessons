/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */

/*********************************/
#include "lwip/opt.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include"http_ssi.h"
#include "string.h"
#include "stdio.h"
#include "lwip/tcp.h"
#include "stm32f4xx_hal.h"
/************************ SSI HANDLER ***************************/
int indx = 0;
int indx1 = 0;
/* we will use character "x", "y","z" as tag for SSI */
char const *TAGCHAR[] = { "x", "y", "z", "test" };
char const **TAGS = TAGCHAR;

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
	switch (iIndex) {
	case 0:
		indx += 1;
		sprintf(pcInsert, "%d", indx);
		return strlen(pcInsert);
		break;
	case 1:
		indx += 1;
		sprintf(pcInsert, "%d", indx);
		return strlen(pcInsert);
		break;
	case 2:
		indx += 1;
		sprintf(pcInsert, "%d", indx);
		return strlen(pcInsert);
		break;
	case 3:
		indx1 += 1;
		sprintf(pcInsert, "%d", indx1);
		return strlen(pcInsert);
		break;
	default:
		break;
	}

	return 0;
}

/************************ CGI HANDLER for FORM ***************************/
const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]);
const char* CGI_LEDs_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]); // 1. For LED

const tCGI FORM_CGI = { "/login.cgi", CGIForm_Handler }; // Создаем структуру CGI
const tCGI LED_CGI = { "/leds.cgi", CGI_LEDs_Handler }; // 2. For LED создаем структуру CGI  (в папке fs файл "cgi_leds.html" стр.8)

char name[30];
char leds[3]; // +1 для пробела!
tCGI CGI_TAB[2]; // 3. For LED создадим массив для LED CGI

const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	if (iIndex == 0) {
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "fname") == 0) // if the fname string is found
					{
				memset(name, '\0', 30); // Очищаем массив перед записью новых данных.
				strcpy(name, pcValue[i]); // Сохраняем значение в переменную.
			}

			else if (strcmp(pcParam[i], "lname") == 0) // if the lname string is found
					{
				strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
			}
		}
	}

	return "/1.shtml"; // Здесь пропишем имя файла из которого берем переменные.
}

/************************ CGI HANDLER for LED ***************************/
const char* CGI_LEDs_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	if (iIndex == 1) {
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "led_1") == 0) // if the fname string is found
					{
				memset(leds, '\0', 3); // Очищаем массив перед записью новых данных. Цифра 3 количество елиментов в массиве которое нужно очистить.
				strcpy(leds, pcValue[i]); // Сохраняем значение в переменную.
			}

			else if (strcmp(pcParam[i], "led_2") == 0) // if the lname string is found
					{
				strcat(leds, " "); // Сконкатенируем fname и lname разделяя пробелом.
				strcat(leds, pcValue[i]); // Сохраняем значение в переменную.
			}
		}
	}

	return "/cgi_leds.html"; // Здесь пропишем имя файла из которого берем переменные.
}
/************************ 3 functions for POST request ***************************/
/** define LWIP_HTTPD_EXAMPLE_GENERATEDFILES to 1 to enable this file system */
#ifndef LWIP_HTTPD_EXAMPLE_SIMPLEPOST
#define LWIP_HTTPD_EXAMPLE_SIMPLEPOST 0
#endif

////#if LWIP_HTTPD_EXAMPLE_SIMPLEPOST

#if !LWIP_HTTPD_SUPPORT_POST
//#error This needs LWIP_HTTPD_SUPPORT_POST
#endif

#define USER_PASS_BUFSIZE 16

static void *current_connection;
static void *valid_connection;
static char last_user[USER_PASS_BUFSIZE];

err_t httpd_post_begin(void *connection, const char *uri,
		const char *http_request, uint16_t http_request_len, int content_len,
		char *response_uri, uint16_t response_uri_len, uint8_t *post_auto_wnd) {
	//printf("httpd_post_begin! \r\n");
	LWIP_UNUSED_ARG(connection);
	LWIP_UNUSED_ARG(http_request);
	LWIP_UNUSED_ARG(http_request_len);
	LWIP_UNUSED_ARG(content_len);
	LWIP_UNUSED_ARG(post_auto_wnd);
	if (!memcmp(uri, "/login.cgi", 11)) {
		if (current_connection != connection) {
			current_connection = connection;
			valid_connection = NULL;
			/* default page is "login failed" */
			snprintf(response_uri, response_uri_len, "/index.html");
			/* e.g. for large uploads to slow flash over a fast connection, you should
			 manually update the rx window. That way, a sender can only send a full
			 tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
			 We do not need to throttle upload speed here, so: */
			*post_auto_wnd = 1;
			return ERR_OK;
		}
	}
	return ERR_VAL;
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p) {
	//printf("httpd_post_receive_data! \r\n");
	if (current_connection == connection) {
		u16_t token_user = pbuf_memfind(p, "user=", 5, 0);
		u16_t token_pass = pbuf_memfind(p, "pass=", 5, 0);
		if ((token_user != 0xFFFF) && (token_pass != 0xFFFF)) {
			u16_t value_user = token_user + 5;
			u16_t value_pass = token_pass + 5;
			u16_t len_user = 0;
			u16_t len_pass = 0;
			u16_t tmp;
			/* find user len */
			tmp = pbuf_memfind(p, "&", 1, value_user);
			if (tmp != 0xFFFF) {
				len_user = tmp - value_user;
			} else {
				len_user = p->tot_len - value_user;
			}
			/* find pass len */
			tmp = pbuf_memfind(p, "&", 1, value_pass);
			if (tmp != 0xFFFF) {
				len_pass = tmp - value_pass;
			} else {
				len_pass = p->tot_len - value_pass;
			}
			if ((len_user > 0) && (len_user < USER_PASS_BUFSIZE)
					&& (len_pass > 0) && (len_pass < USER_PASS_BUFSIZE)) {
				/* provide contiguous storage if p is a chained pbuf */
				char buf_user[USER_PASS_BUFSIZE];
				char buf_pass[USER_PASS_BUFSIZE];
				char *user = (char*) pbuf_get_contiguous(p, buf_user,
						sizeof(buf_user), len_user, value_user);
				char *pass = (char*) pbuf_get_contiguous(p, buf_pass,
						sizeof(buf_pass), len_pass, value_pass);
				if (user && pass) {
					user[len_user] = 0;
					pass[len_pass] = 0;
					if (!strcmp(user, "lwip") && !strcmp(pass, "post")) {
						/* user and password are correct, create a "session" */
						valid_connection = connection;
						memcpy(last_user, user, sizeof(last_user));
					}
				}
			}
		}
		/* not returning ERR_OK aborts the connection, so return ERR_OK unless the
		 conenction is unknown */
		return ERR_OK;
	}
	return ERR_VAL;
}

void httpd_post_finished(void *connection, char *response_uri,
		u16_t response_uri_len) {
	//printf("httpd_post_finished! \r\n");
	/* default page is "login failed" */
	snprintf(response_uri, response_uri_len, "/loginfail.html");
	if (current_connection == connection) {
		if (valid_connection == connection) {
			/* login succeeded */
			snprintf(response_uri, response_uri_len, "/session.html");
		}
		current_connection = NULL;
		valid_connection = NULL;
	}
}
/************************************************************************/
void http_server_init(void) {
	httpd_init();

	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 4);
	// 4. For LED Перед созданиме CGI_handle нам нужно назначить
	CGI_TAB[0] = FORM_CGI; // CGI структуру
	CGI_TAB[1] = LED_CGI;  // в массив

	//http_set_cgi_handlers (&FORM_CGI, 1); // Это пример когда у нас всего один "Handler" пример из видео.
	http_set_cgi_handlers(CGI_TAB, 2); // 5. И теперь мы передадим структуру массива в CGI_handle!
}
