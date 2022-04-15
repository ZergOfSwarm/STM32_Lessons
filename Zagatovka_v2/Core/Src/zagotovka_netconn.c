/**
 ******************************************************************************
 * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c
 * @author  MCD Application Team
 * @brief   Basic http server implementation using LwIP netconn API
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "lwip/apps/fs.h"
#include "string.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <zagotovka_netconn.h>
#include <zagotovka_sdio.h>

/* Private typedef -----------------------------------------------------------*/
int variable = 0; // For loop in json.
struct table T[num_lines];		// доступ из вне к 'struct table'./* Private define ------------------------------------------------------------*/

#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
int flag = 0;
/* Format of dynamic web page: the page header */
//static const unsigned char PAGE_START[] = "";
struct tablepins { // Создали структуру с необходимым набором типов элиментов для PIN's.
	char pins[5];
	char port[2];
	int number;
	uint32_t hal_pin;
	uint32_t gpio_name;
} tp[] = { "PA0", "A", 0, GPIO_PIN_0, GPIOA, "PA4", "A", 4, GPIO_PIN_4, GPIOA, "PA5", "A", 5,
GPIO_PIN_5, GPIOA, "PA6", "A", 6, GPIO_PIN_6, GPIOA, "PA8", "A", 8,
GPIO_PIN_8, GPIOA, "PA9", "A", 9, GPIO_PIN_9, GPIOA, "PA10", "A", 10,
GPIO_PIN_10, GPIOA, "PA11", "A", 11,
GPIO_PIN_11, GPIOA, "PA12", "A", 12, GPIO_PIN_12, GPIOA, "PA15", "A", 15, GPIO_PIN_15, GPIOA, "PB0", "B", 0, GPIO_PIN_0, GPIOB, "PB1", "B", 1,
GPIO_PIN_1, GPIOB, "PB3", "B", 3,
GPIO_PIN_3, GPIOB, "PB5", "B", 5, GPIO_PIN_5, GPIOB, "PB6", "B", 6,
GPIO_PIN_6, GPIOB, "PB7", "B", 7, GPIO_PIN_7, GPIOB, "PB8", "B", 8,
GPIO_PIN_8, GPIOB, "PB9", "B", 9,
GPIO_PIN_9, GPIOB, "PB10", "B", 10, GPIO_PIN_10, GPIOB, "PB14", "B", 14,
GPIO_PIN_14, GPIOB, "PB15", "B", 15, GPIO_PIN_15, GPIOB, "PC0", "C", 0,
GPIO_PIN_0, GPIOC, "PC2", "C", 2,
GPIO_PIN_2, GPIOC, "PC3", "C", 3, GPIO_PIN_3, GPIOC, "PC6", "C", 6,
GPIO_PIN_6, GPIOC, "PC7", "C", 7, GPIO_PIN_7, GPIOC, "PC13", "C", 13,
GPIO_PIN_13, GPIOC, "PD0", "D", 0,
GPIO_PIN_0, GPIOD, "PD1", "D", 1, GPIO_PIN_1, GPIOD, "PD3", "D", 3,
GPIO_PIN_3, GPIOD, "PD4", "D", 4, GPIO_PIN_4, GPIOD, "PD5", "D", 5,
GPIO_PIN_5, GPIOD, "PD6", "D", 6,
GPIO_PIN_6, GPIOD, "PD7", "D", 7, GPIO_PIN_7, GPIOD, "PD8", "D", 8,
GPIO_PIN_8, GPIOD, "PD9", "D", 9, GPIO_PIN_9, GPIOD, "PD10", "D", 10,
GPIO_PIN_10, GPIOD, "PD11", "D", 11,
GPIO_PIN_11, GPIOD, "PD12", "D", 12, GPIO_PIN_12, GPIOD, "PD13", "D", 13, GPIO_PIN_13, GPIOD, "PD14", "D", 14, GPIO_PIN_14, GPIOD, "PD15", "D", 15, GPIO_PIN_15, GPIOD, "PE0", "E", 0, GPIO_PIN_0, GPIOE, "PE1", "E", 1, GPIO_PIN_1, GPIOE, "PE2", "E", 2, GPIO_PIN_2, GPIOE, "PE3", "E", 3, GPIO_PIN_3,
GPIOE, "PE4", "E", 4, GPIO_PIN_4, GPIOE, "PE5", "E", 5,
GPIO_PIN_5, GPIOE, "PE6", "E", 6, GPIO_PIN_6, GPIOE, "PE7", "E", 7,
GPIO_PIN_7, GPIOE, "PE8", "E", 8, GPIO_PIN_8, GPIOE, "PE9", "E", 9,
GPIO_PIN_9, GPIOE, "PE10", "E", 10,
GPIO_PIN_10, GPIOE, "PE11", "E", 11, GPIO_PIN_11, GPIOE, "PE12", "E", 12, GPIO_PIN_12, GPIOE, "PE13", "E", 13, GPIO_PIN_13, GPIOE, "PE14", "E", 14, GPIO_PIN_14, GPIOE, "PE15", "E", 15, GPIO_PIN_15, GPIOE };
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
char cp[10];
char parse(const char *line) {
	char *buf = { 0 };
	/* Find out where everything is */
	const char *start_of_path = strchr(line, ' ') + 1;
	const char *start_of_query = strchr(start_of_path, '?');
	const char *end_of_query = strchr(start_of_query, ' ');

	/* Get the right amount of memory */
	char path[start_of_query - start_of_path];
	char query[end_of_query - start_of_query];

	/* Copy the strings into our memory */
	strncpy(path, start_of_path, start_of_query - start_of_path);
	strncpy(query, start_of_query, end_of_query - start_of_query);

	/* Null terminators (because strncpy does not provide them) */
	path[sizeof(path)] = 0;
	query[sizeof(query)] = 0;

	/*Print */
	sprintf(cp, "%s\n", query);
	// printf("%s\n", path);
	// strncpy(cp, cp + 1,4);
	strcpy(cp, cp + 1);
	return cp;
}

/**
 * @brief serve tcp connection
 * @param conn: pointer on connection structure
 * @retval None
 */
static void http_server_serve(struct netconn *conn) {
	struct netbuf *inbuf;
	err_t recv_err;
	char *buf;
	u16_t buflen;
	struct fs_file file;
//	char data_str[3000];
//	int sum = 0;

	/* Read the data from the port, blocking if nothing yet there.
	 We assume the request (the part we care about) is in one netbuf */
	recv_err = netconn_recv(conn, &inbuf);
	char my_json[200];    //??????????????????
	char full_json[700]; //??????????????????
	memset(full_json, 0, 700);
	memset(my_json, 0, 200);
	if (recv_err == ERR_OK) {
		if (netconn_err(conn) == ERR_OK) {
			do {
				netbuf_data(inbuf, (void**) &buf, &buflen); // Получили данные и собираем POST отрезки.
//				netbuf_data(inbuf, (void*) &buf, &buflen); // Получили данные и собираем POST отрезки.
//				netbuf_data(inbuf, &buf, &buflen); // Получили данные и собираем POST отрезки.

				if ((buflen >= 5) && (strncmp(buf, "GET /", 5) == 0)) {
					//printf("++++++ GET ++++++ %d/ size - %d \r\n", buflen, sizeof(buflen));

					if (strncmp(buf, "GET / ", 6) == 0) {
						/* Load STM32F4xx page */
						printf(" GET !!!\n\r");
						fs_open(&file, "/zagatovka.html");
						netconn_write(conn, (const unsigned char* )(file.data), (size_t )file.len, NETCONN_NOCOPY);
						fs_close(&file);

					} else if (strncmp(buf, "GET /form.html", 14) == 0) {
						/* Load STM32F4xx page */
//												printf(" GET !!!\n\r");
//												fs_open(&file, "/form.html");
//												netconn_write(conn, (const unsigned char* )(file.data), (size_t )file.len, NETCONN_NOCOPY);
//												fs_close(&file);
						printf("BUFFER IS - %s", buf);
						if (strncmp(buf, "GET /form.html?id=", 18) == 0) {
							//parse(buf);
//							    char *str [] = {0};
//								printf(" MY ID is - %s\n",strcpy(str,parse(buf)));

							char *token;
							char *rest = parse(buf);
							while ((token = strtok_r(rest, "=", &rest))) {
								printf("Result of Parse - %s\r\n", token);
							}
						}
						My_Form_Page(conn, 0);

					} else if (strncmp(buf, "GET /json.html", 14) == 0) {
						char *info;
						strcat(full_json, "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\n\r\n"); // объеденяем строки

						while (variable < num_lines) {
							if (variable == 0) {
								strcat(full_json, "[");
							}
							if (isalpha(T[variable].infofield[0])) {
								info = T[variable].infofield;
								//printf("-----------It's char\n");
							} else if (isdigit(T[variable].infofield[0])) {
								info = NULL;
								//printf("-----------It's int\n");
							} else {
								info = NULL;
								//printf("-----------It's empty\n");
							}
							//printf("----------TEST---------\n");

							if (variable == num_lines - 1) {
								sprintf(
										my_json,  // Получаем json
										"{\"pins\":\"%s\",\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d}]", tp[variable].pins, T[variable].pulluptype,
										T[variable].bounceinterval, T[variable].holdinterval, T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval, T[variable].preventclick, info, T[variable].onoff);

							} else {
								sprintf(my_json, "{\"pins\":\"%s\",\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d},", tp[variable].pins, T[variable].pulluptype,
										T[variable].bounceinterval, T[variable].holdinterval, T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval, T[variable].preventclick, info, T[variable].onoff);
							}
							strcat(full_json, my_json); // объеденяем строки
//							sprintf(my_json,  // Получаем json
//									"%s %s %d %d %d %d %d %d %s %d",
//									tp[variable].pins, T[variable].pulluptype,
//									T[variable].bounceinterval, T[variable].holdinterval,
//									T[variable].repeat, T[variable].repeatinterval,
//									T[variable].doubleclickinterval,
//									T[variable].preventclick, info, T[variable].onoff);

							variable++;
						}
						variable = 0;
						netconn_write(conn, full_json, strlen(full_json), NETCONN_COPY);
//						memset(full_json, 0, sizeof(full_json));
//						memset(my_json, 0, sizeof(my_json));
					} else if (strncmp(buf, "GET /zagatovka.html", 19) == 0) {
						//parse(buf);  // PARSSSSSSSSSSSSSSSS
						printf(" preventclick_%d is: %d \n", 0, T[0].preventclick);
						fs_open(&file, "/zagatovka.html");
						netconn_write(conn, (const unsigned char* )(file.data), (size_t )file.len, NETCONN_NOCOPY);
						fs_close(&file);
					} else {
						/* Load Error page */
						fs_open(&file, "/404.html");
						netconn_write(conn, (const unsigned char* )(file.data), (size_t )file.len, NETCONN_NOCOPY);
						fs_close(&file);
					}
				}

				//printf("TEST \n");
				if ((buflen >= 6) && (strncmp(buf, "POST /", 6) == 0)) {
					if ((strncmp(buf, "POST /post.html", 15) == 0) || (strncmp(buf, "POST / ", 7) == 0)) { //20
						flag = 1;
					} else {
						/* Load Error page */
						fs_open(&file, "/404.html");
						netconn_write(conn, (const unsigned char* )(file.data), (size_t )file.len, NETCONN_NOCOPY);
					}
				}
				//strncat(data_str, buf, buflen); // Объеденяем кусочки данных в буфере.
//				sum += buflen;
				//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0); //toggle data led
			} while (netbuf_next(inbuf) >= 0); // Очищаем пока есть что либо в буфере.
			if (flag == 1) {
				fs_open(&file, "/post.html");
				netconn_write(conn, (const unsigned char* )(file.data), (size_t )file.len, NETCONN_NOCOPY); // Отправка на web page...
				fs_close(&file);
				flag = 0;
			}
			/* Is this an HTTP GET command? (only check the first 5 chars, since
			 there are other formats for GET, and we're keeping it very simple )*/
		}
	}
	/* Close the connection (server closes in HTTP) */
	netconn_close(conn);

	/* Delete the buffer (netconn_recv gives us ownership,
	 so we have to make sure to deallocate the buffer) */
	netbuf_delete(inbuf);
}

/**
 * @brief  http server thread
 * @param arg: pointer on argument(not used here)
 * @retval None
 */
static void http_server_netconn_thread(void *arg) {
	struct netconn *conn, *newconn;
	err_t err, accept_err;

	/* Create a new TCP connection handle */
	conn = netconn_new(NETCONN_TCP);

	if (conn != NULL) {
		/* Bind to port 80 (HTTP) with default IP address */
		err = netconn_bind(conn, NULL, 80);

		if (err == ERR_OK) {
			/* Put the connection into LISTEN state */
			netconn_listen(conn);

			while (1) {
				/* accept any icoming connection */
				accept_err = netconn_accept(conn, &newconn);
				if (accept_err == ERR_OK) {
					/* serve connection */
					http_server_serve(newconn);

					/* delete connection */
					netconn_delete(newconn);
				}
			}
		}
	}
}

/**
 * @brief  Initialize the HTTP server (start its thread)
 * @param  none
 * @retval None
 */
void zagotovka_netconn_init() {
	sys_thread_new("HTTP", http_server_netconn_thread, NULL,DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}

/**
 * @brief  Create and send a dynamic Web Page. This page contains the list of
 *         running tasks and the number of page hits.
 * @param  conn pointer on connection structure
 * @retval None
 */
//void DynWebPage(struct netconn *conn) {
//	portCHAR PAGE_BODY[512];
//	portCHAR pagehits[10] = { 0 };
//
//	memset(PAGE_BODY, 0, 512);
//
//	/* Update the hit count */
//	nPageHits++;
//	sprintf(pagehits, "%d", (int) nPageHits);
//	strcat(PAGE_BODY, pagehits);
//	strcat((char*) PAGE_BODY, "<pre><br>Name          State  Priority  Stack   Num");
//	strcat((char*) PAGE_BODY, "<br>---------------------------------------------<br>");
//
//	/* The list of tasks and their status */
////osThreadList((unsigned char *)(PAGE_BODY + strlen(PAGE_BODY)));
//	strcat((char*) PAGE_BODY, "<br><br>---------------------------------------------");
//	strcat((char*) PAGE_BODY, "<br>B : Blocked, R : Ready, D : Deleted, S : Suspended<br>");
//
//	/* Send the dynamically generated page */
//	netconn_write(conn, PAGE_START, strlen((char* )PAGE_START), NETCONN_COPY);
//	netconn_write(conn, PAGE_BODY, strlen(PAGE_BODY), NETCONN_COPY);
//}

void My_Form_Page(struct netconn *conn, int id) {
	static const char first[] =
			"<!DOCTYPE html><title>Таблица</title><meta content=\"text/html; charset=utf-8\"http-equiv=Content-Type><meta content=\"max-age=0\"http-equiv=cache-control><meta content=no-cache http-equiv=cache-control><meta content=0 http-equiv=expires><meta content=\"Tue, 01 Jan 1980 1:00:00 GMT\"http-equiv=expires><meta content=no-cache http-equiv=pragma><style>table,td,th{border:1px solid #000;border-collapse:collapse;padding:5px}tr:nth-child(odd){background-color:#cfc}</style><body>";
	static const char second[] = "</body></html>";
	static const char test[] = "test";

	netconn_write(conn, first, strlen(first), NETCONN_COPY);
	netconn_write(conn, test, strlen(test), NETCONN_COPY);
	netconn_write(conn, second, strlen(second), NETCONN_COPY);
}

