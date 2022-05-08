/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: Zerg
 */

#include "http_ssi.h"
#include "string.h"
#include "stdio.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "stm32f4xx_hal.h"

#include "db.h"
/************************ SSI HANDLER ***************************/
int indx1 = 0;
int y;
//char json[2500];
char bufervar[10500];
//char arrline[400];

struct tablepins tp[numpin] = {
		{"PA0", "A", 0, GPIO_PIN_0, GPIOA},
		{"PA4", "A", 4, GPIO_PIN_4, GPIOA},
		{"PA5", "A", 5, GPIO_PIN_5, GPIOA},
		{"PA6", "A", 6, GPIO_PIN_6, GPIOA},
		{"PA8", "A", 8, GPIO_PIN_8, GPIOA},
		{"PA9", "A", 9, GPIO_PIN_9, GPIOA},
		{"PA10", "A", 10, GPIO_PIN_10, GPIOA},
		{"PA11", "A", 11, GPIO_PIN_11, GPIOA},
		{"PA12", "A", 12, GPIO_PIN_12, GPIOA},
		{"PA15", "A", 15, GPIO_PIN_15, GPIOA},
		{"PB0", "B", 0, GPIO_PIN_0, GPIOB},
		{"PB1", "B", 1, GPIO_PIN_1, GPIOB},
		{"PB3", "B", 3, GPIO_PIN_3, GPIOB},
		{"PB5", "B", 5, GPIO_PIN_5, GPIOB},
		{"PB6", "B", 6, GPIO_PIN_6, GPIOB},
		{"PB7", "B", 7, GPIO_PIN_7, GPIOB},
		{"PB8", "B", 8, GPIO_PIN_8, GPIOB},
		{"PB9", "B", 9, GPIO_PIN_9, GPIOB},
		{"PB10", "B", 10, GPIO_PIN_10, GPIOB},
		{"PB14", "B", 14, GPIO_PIN_14, GPIOB},
		{"PB15", "B", 15, GPIO_PIN_15, GPIOB},
		{"PC0", "C", 0, GPIO_PIN_0, GPIOC},
		{"PC2", "C", 2, GPIO_PIN_2, GPIOC},
		{"PC3", "C", 3, GPIO_PIN_3, GPIOC},
		{"PC6", "C", 6, GPIO_PIN_6, GPIOC},
		{"PC7", "C", 7, GPIO_PIN_7, GPIOC},
		{"PC13", "C", 13, GPIO_PIN_13, GPIOC},
		{"PD0", "D", 0, GPIO_PIN_0, GPIOD},
		{"PD1", "D", 1, GPIO_PIN_1, GPIOD},
		{"PD3", "D", 3, GPIO_PIN_3, GPIOD},
		{"PD4", "D", 4, GPIO_PIN_4, GPIOD},
		{"PD5", "D", 5, GPIO_PIN_5, GPIOD},
		{"PD6", "D", 6, GPIO_PIN_6, GPIOD},
		{"PD7", "D", 7, GPIO_PIN_7, GPIOD},
		{"PD8", "D", 8, GPIO_PIN_8, GPIOD},
		{"PD9", "D", 9, GPIO_PIN_9, GPIOD},
		{"PD10", "D", 10, GPIO_PIN_10, GPIOD},
		{"PD11", "D", 11, GPIO_PIN_11, GPIOD},
		{"PD12", "D", 12, GPIO_PIN_12, GPIOD},
		{"PD13", "D", 13, GPIO_PIN_13, GPIOD},
		{"PD14", "D", 14, GPIO_PIN_14, GPIOD},
		{"PD15", "D", 15, GPIO_PIN_15, GPIOD},
		{"PE0", "E", 0, GPIO_PIN_0, GPIOE},
		{"PE1", "E", 1, GPIO_PIN_1, GPIOE},
		{"PE2", "E", 2, GPIO_PIN_2, GPIOE},
		{"PE3", "E", 3, GPIO_PIN_3,GPIOE},
		{"PE4", "E", 4, GPIO_PIN_4,GPIOE},
		{"PE5", "E", 5, GPIO_PIN_5, GPIOE},
		{"PE6", "E", 6, GPIO_PIN_6, GPIOE},
		{"PE7", "E", 7, GPIO_PIN_7, GPIOE},
		{"PE8", "E", 8, GPIO_PIN_8, GPIOE},
		{"PE9", "E", 9, GPIO_PIN_9, GPIOE},
		{"PE10", "E", 10, GPIO_PIN_10, GPIOE},
		{"PE11", "E", 11, GPIO_PIN_11, GPIOE},
		{"PE12", "E", 12, GPIO_PIN_12, GPIOE},
		{"PE13", "E", 13, GPIO_PIN_13, GPIOE},
		{"PE14", "E", 14, GPIO_PIN_14, GPIOE},
		{"PE15", "E", 15, GPIO_PIN_15, GPIOE}};

extern struct intoout TO[numpintopin];
extern struct table T[numpin];

int variable3 = 0;
int variable4 = 0;
char name3[10];


static int variable = 0;
int jsongo = 0; // GET json type
int id = 0;     // GET tab
int idtab = 0;  // GET id
//char name[30];
char jsonrp[300]; // было 100
char listaction[400];

/* we will use character "x", "y","z" as tag for SSI */
char const* TAGCHAR[]={"json","formjson","menu"};
char const **TAGS = TAGCHAR;
int f = 0;
//
uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen, u16_t current_tag_part, u16_t *next_tag_part) {
	//printf("+++ jsongo +++ %d \n", jsongo);
	switch (iIndex) {
	case 0:
#if LWIP_HTTPD_SSI_MULTIPART
		if (variable == numpin) {
			//default:
			//printf("Exit from function! \n");
			variable = 0;
			//return strlen(pcInsert);
			break;
		} else {
			//if (isalpha(T[variable].infofield[0])) {
			if(jsongo == 1){
				while(variable3 <= numpintopin-1){
						if(variable3 == 0) {
							strcat(listaction, "{");
						}
						if(variable == TO[variable3].idin && TO[variable3].flag == 1){
//						if(variable == TO[variable3].idin){
							//variable4 = atoi(TO[variable3].idout);
							sprintf(name3, "\"%s\":%d,",tp[TO[variable3].idout].pins, variable3);
							//sprintf(name3, "%d ", TO[variable3].idout);
							strcat(listaction, name3);
						}

						if(variable3 == numpintopin-1 &&  strcmp(listaction[strlen(listaction)-1], '{') > 0){
//							printf("Exit %s \n", listaction);
//							printf("Exit %d \n", strlen(listaction));
//							printf("Exit %с \n", listaction[strlen(listaction)-1]);
							listaction[strlen(listaction)-1] = '\0';
						}
						if(variable3 == numpintopin-1) {
							strcat(listaction, "}");
						}
						variable3++;
				}
				variable3 = 0;
			}

			if (variable == 0) {
				if(jsongo == 1){
				// buttoms
				sprintf(pcInsert,
						"[{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"pinact\":%s,\"info\":\"%s\",\"onoff\":%d},",
						T[variable].topin,
						variable,
						tp[variable].pins,
						T[variable].ptype,
						T[variable].binter,
						T[variable].hinter,
						T[variable].repeat,
						T[variable].rinter,
						T[variable].dcinter,
						T[variable].pclick,
						listaction,
						T[variable].info,
						T[variable].onoff);
				}
				if(jsongo == 2){
				// relay
				sprintf(pcInsert,
						"[{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d},",
						T[variable].topin,
						variable,
						tp[variable].pins,
						T[variable].ptype,
						T[variable].pwm,
						T[variable].on,
						T[variable].istate,
						T[variable].dvalue,
						T[variable].ponr,
						T[variable].info,
						T[variable].onoff);
				}
				if(jsongo == 3){
				// select
					sprintf(pcInsert,
							"[{\"id\":%d,\"pins\":\"%s\",\"topin\":%d},",
							variable,
							tp[variable].pins,
							T[variable].topin);

				}
			} else if (variable == numpin-1) {
				if(jsongo == 1){
				// buttoms
				sprintf(pcInsert,
						"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"pinact\":%s,\"info\":\"%s\",\"onoff\":%d}]",
						T[variable].topin,
						variable,
						tp[variable].pins,
						T[variable].ptype,
						T[variable].binter,
						T[variable].hinter,
						T[variable].repeat,
						T[variable].rinter,
						T[variable].dcinter,
						T[variable].pclick,
						listaction,
						T[variable].info,
						T[variable].onoff);
				}
				if(jsongo == 2){
				// relay
					sprintf(pcInsert,
							"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d}]",
							T[variable].topin,
							variable,
							tp[variable].pins,
							T[variable].ptype,
							T[variable].pwm,
							T[variable].on,
							T[variable].istate,
							T[variable].dvalue,
							T[variable].ponr,
							T[variable].info,
							T[variable].onoff);
				}
				if(jsongo == 3){
				// select
					sprintf(pcInsert,
							"{\"id\":%d,\"pins\":\"%s\",\"topin\":%d}]",
							variable,
							tp[variable].pins,
							T[variable].topin);
				}
			} else {
				if(jsongo == 1){
				// buttoms
				sprintf(pcInsert,
						"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"pinact\":%s,\"info\":\"%s\",\"onoff\":%d},",
						T[variable].topin,
						variable,
						tp[variable].pins,
						T[variable].ptype,
						T[variable].binter,
						T[variable].hinter,
						T[variable].repeat,
						T[variable].rinter,
						T[variable].dcinter,
						T[variable].pclick,
						listaction,
						T[variable].info,
						T[variable].onoff);
				}
				if(jsongo == 2){
				// relay
					sprintf(pcInsert,
							"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d},",
							T[variable].topin,
							variable,
							tp[variable].pins,
							T[variable].ptype,
							T[variable].pwm,
							T[variable].on,
							T[variable].istate,
							T[variable].dvalue,
							T[variable].ponr,
							T[variable].info,
							T[variable].onoff);
				}
				if(jsongo == 3){
				// select
					sprintf(pcInsert,
							"{\"id\":%d,\"pins\":\"%s\",\"topin\":%d},",
							variable,
							tp[variable].pins,
							T[variable].topin);
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
			if(idtab == 1){
			// buttoms
			sprintf(pcInsert,
					"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"binter\":%d,\"hinter\":%d,\"repeat\":%d,\"rinter\":%d,\"dcinter\":%d,\"pclick\":%d,\"info\":\"%s\",\"onoff\":%d}",
					T[id].topin,
					id,
					tp[id].pins,
					T[id].ptype,
					T[id].binter,
					T[id].hinter,
					T[id].repeat,
					T[id].rinter,
					T[id].dcinter,
					T[id].pclick,
					T[id].info,
					T[id].onoff);
			}
			if(idtab == 2){
			// relay
			sprintf(pcInsert,
					"{\"topin\":%d,\"id\":%d,\"pins\":\"%s\",\"ptype\":\"%s\",\"pwm\":%d,\"on\":%d,\"istate\":%d,\"dvalue\":%d,\"ponr\":%d,\"info\":\"%s\",\"onoff\":%d}",
					T[id].topin,
					id,
					tp[id].pins,
					T[id].ptype,
					T[id].pwm,
					T[id].on,
					T[id].istate,
					T[id].dvalue,
					T[id].ponr,
					T[id].info,
					T[id].onoff);
			}
			if(idtab == 3){
			// connection
				sprintf(jsonrp, "{");
				strcat(bufervar, jsonrp);
			while(variable <= numpin-1){
				if(T[variable].topin == 2){
					sprintf(jsonrp, "\"%s\":%d,", tp[variable].pins, variable);
				    strcat(bufervar, jsonrp);
					printf("%s \r\n",  jsonrp);
				}
				variable++;
			}
				bufervar[strlen(bufervar)-1] = '\0';
				sprintf(jsonrp, "}");
				strcat(bufervar, jsonrp);
				variable = 0;
				sprintf(pcInsert,"{\"id\":%d,\"pins\":\"%s\",\"rpins\":%s}", id, tp[id].pins, bufervar);
			}
			memset(bufervar, '\0', sizeof(bufervar));
			return strlen(pcInsert);
			break;
		case 2:
			sprintf(pcInsert,"<a href=\"index.shtml\">Home</a> | <a href=\"tab1.shtml\">Buttom pin</a> | <a href=\"tab2.shtml\">Relay pin</a> | <a href=\"tab3.shtml\">Connection</a> | <a href=\"select.shtml\">Select pin</a>");
			return strlen(pcInsert);
			break;
		default :
			break;
	}
	return 0;
}
/************************ CGI HANDLER for FORM ***************************/
const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* CGIForm_Handler1(int iIndex1, int iNumParams, char *pcParam[], char *pcValue[]);
const char *CGIForm_Handler2(int iIndex1, int iNumParams, char *pcParam[], char *pcValue[]);
//const tCGI FORM_CGI = {"/json.shtml", CGIForm_Handler,"/form1.shtml", CGIForm_Handler1,"/form2.shtml", CGIForm_Handler2}; // Создаем структуру CGI

static const tCGI URL_TABLES[] =
{
    {"/json.shtml", CGIForm_Handler},
    {"/form1.shtml",CGIForm_Handler1},
    {"/form2.shtml",CGIForm_Handler1},
    {"/form3.shtml",CGIForm_Handler1},
	{"/tab1.shtml",CGIForm_Handler2}
};

const uint8_t CGI_URL_NUM = (sizeof(URL_TABLES) / sizeof(tCGI));

//tCGI CGI_TAB[3]; // 3. For LED создадим массив для LED CGI
const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	//printf("GET OK 1 \n");
	if (iIndex == 0) {
		for (int i = 0; i < iNumParams; i++) {
			if (strcmp(pcParam[i], "jsongo") == 0)  // if the fname string is found
					{
				//memset(name, '\0', 30);  // Очищаем массив перед записью новых данных.
				//strcpy(name, pcValue[i]); // Сохраняем значение в переменную.
				jsongo = atoi(pcValue[i]);
			}
		}
	}
	return "/json.shtml"; // Здесь пропишем имя файла из которого берем переменные.
}

const char* CGIForm_Handler1(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
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
		return "/form1.shtml"; // Здесь пропишем имя файла из которого берем переменные.
	} else if (iIndex == 2) {
		return "/form2.shtml";
	} else if (iIndex == 3) {
			return "/form3.shtml";
	} else {
		return "/404.html";
	}
}

const char *CGIForm_Handler2(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	printf("GET OK 1 \n");
	if (iIndex == 4)
	{
		for (int i=0; i<iNumParams; i++)
		{
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

//////////////////// POST START ////////////////////////
static void *current_connection;
static void *valid_connection;
int http_post_content_len;
// char *urls;
char urls[30] = { 0 };
err_t httpd_post_begin(void *connection, const char *uri, const char *http_request, uint16_t http_request_len, int content_len, char *response_uri, uint16_t response_uri_len, uint8_t *post_auto_wnd) {
	//printf("OK POST 1 \r\n");
	//memset(bufervar, '\0', sizeof(bufervar));
	//LWIP_UNUSED_ARG(connection);
	//LWIP_UNUSED_ARG(http_request);
	//LWIP_UNUSED_ARG(http_request_len);
	//LWIP_UNUSED_ARG(content_len);
	//LWIP_UNUSED_ARG(post_auto_wnd);
	http_post_content_len = content_len;
	//urls = uri;
	strncpy(urls, uri, 15);
	//printf("+++++++++uri %s +++++++++++------- \r\n",uri);  // Вывод /select.shtml +
	//printf("httpd_post_begin: Post Content: %s\n",  http_request);
	//printf("%d \r\n",  http_post_content_len);
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
	printf("OK POST 2 \r\n");
	char *data;
	if (current_connection == connection && p != NULL) {
		data = p->payload;
		printf("DATA %s \r\n", data);
		strcat(bufervar, data);
		pbuf_free(p);
		memset(data, '\0', sizeof(*data));
		return ERR_OK;
	}
	if (p != NULL) {
		printf("END \r\n");
		pbuf_free(p);
	}
	return ERR_VAL;
}

void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len) {
	printf("OK POST 3 \r\n");

	char *token;
	char *token1;
	char *token2;
	char *name1;
	int z = 0;
	int id;
	int variable1 = 0;
	int variable2;
	//char urlline[http_post_content_len];
	char urlline[1000];
	char name0[50];
	char nametoken[50];

	memset(name0, '\0', sizeof(name0));
	//memset(urlline, '\0', sizeof(urlline));
	strncpy(urlline, bufervar, http_post_content_len);

	printf("%s \r\n", urlline);
	printf("COUNT %d \r\n", http_post_content_len);
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
			if(y == 1){
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
					while(variable1 <= numpintopin-1){
						if(TO[variable1].flag == 0){
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
//					while(variable1 <= 49){
//						printf("TO1 id %d --val %d \r\n",variable1 ,TO[variable1].idin);
//						printf("TO2 id %d --val %d \r\n",variable1 ,TO[variable1].idout);
//						variable1++;
//					}
//					variable1 = 0;
					//////////////
				}
			}
			y++;
		}
		// решение с пустой строкой
		sprintf(nametoken, "infofield_%d=", id); // Если поле пустое то, его заполняем "null".
		if (strcmp(name0, nametoken) == 0) {
			strcpy(T[id].info, "null");
			//printf("Yes \n");
		}
		memset(name0, '\0', sizeof(name0));
		memset(nametoken, '\0', sizeof(nametoken));
		z++;
	}

	memset(urlline, '\0', sizeof(urlline));
	memset(bufervar, '\0', sizeof(bufervar));
	//memset(json, '\0', sizeof(json));
	//printf("----------- %s ------- \r\n", urls);
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
	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 3);
	// 4. For LED Перед созданиме CGI_handle нам нужно назначить
	//CGI_TAB[0] = FORM_CGI; // CGI структуру
	//http_set_cgi_handlers (&FORM_CGI, 1); // Это пример когда у нас всего один "Handler" пример из видео.
	http_set_cgi_handlers(URL_TABLES, CGI_URL_NUM); // 5. И теперь мы передадим структуру массива в CGI_handle!
}
