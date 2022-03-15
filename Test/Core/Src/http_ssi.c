/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */

#include "http_ssi.h"
#include "string.h"
#include "stdio.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "stm32f4xx_hal.h"
#include "zerg_sdio.h"
/************************ SSI HANDLER ***************************/

int indx1 = 0;
int y;
char json[2500];
char bufervar[10000]; // + 3150 если по 5 символов с переменную!
char arrline[1400];
//#define num_lines 70 // Укажи число строк в файле 'http_ssi.h'

//struct table{    // Создали структуру с необходимым набором типов элиментов в файле 'http_ssi.h'
//	char pulluptype[40];
//	int bounceinterval;
//	int holdinterval;
//	int repeat;
//	int repeatinterval;
//	int doubleclickinterval;
//	int preventclick;
//	char infofield[20];
//	int onoff;
//};

struct table T[num_lines]; // Создали массив 'T' структурного типа, размер = 'num_lines'.

int tabline = (num_lines-1);
static int variable = 0;

/* we will use character "x", "y","z" as tag for SSI */
char const *TAGCHAR[] = { "json", "test", "jsontag" };
char const **TAGS = TAGCHAR;
int f = 0;

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen,
		u16_t current_tag_part, u16_t *next_tag_part) {

	switch (iIndex) {
	case 0:
#if LWIP_HTTPD_SSI_MULTIPART
			if(variable == num_lines){
				//default:
				//printf("Exit from function! \n");
				variable=0;
				//return strlen(pcInsert);
				break;
			}else{
//				if(variable == 0){
//				sprintf(pcInsert, "[{\"name\":\"%s\",\"num1\":%d,\"num2\":%d},", T[variable].name, T[variable].num1, T[variable].num2);
//				}else if(variable == num_lines-1){
//					sprintf(pcInsert, "{\"name\":\"%s\",\"num1\":%d,\"num2\":%d}]", T[variable].name, T[variable].num1, T[variable].num2);
//				}else{
//				//snprintf(pcInsert, iInsertLen, "part0 gf gfjgfkjgkfdj gjf kgj kfjgfjgjfkgfjkgjfkgjkfjkjkjjkj kjkjjfkjgk gjf kgjf kjgkf jgfkgj  gkfjfgj gfkj jjjjjjjjjjj <br> \n");
//
//				sprintf(pcInsert, "{\"name\":\"%s\",\"num1\":%d,\"num2\":%d},", T[variable].name, T[variable].num1, T[variable].num2);
//				}

				/***********************************************************************/
				if(variable == 0){
					sprintf(pcInsert, "[{\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d},",
				T[variable].pulluptype, T[variable].bounceinterval, T[variable].holdinterval,
				T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval,
				T[variable].preventclick, T[variable].infofield, T[variable].onoff);
				}else if(variable == num_lines-1){
					sprintf(pcInsert, "{\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d}]",
									T[variable].pulluptype, T[variable].bounceinterval, T[variable].holdinterval,
									T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval,
									T[variable].preventclick, T[variable].infofield, T[variable].onoff);
				}else{
					sprintf(pcInsert, "{\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d},",
														T[variable].pulluptype, T[variable].bounceinterval, T[variable].holdinterval,
														T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval,
														T[variable].preventclick, T[variable].infofield, T[variable].onoff);
				}
				/***********************************************************************/
				*next_tag_part = variable;
				//printf("*next_tag_part = %d\n",*next_tag_part);
				variable ++;

				//strcat(pcInsert, "]");
				return strlen(pcInsert);
				break;
			}
//				if(f = 0) {
//					*next_tag_part = 1;
//					 snprintf(pcInsert, iInsertLen, "part1 ");
//					 f++
//					 return strlen(pcInsert);
//					 break;
//				}
//				if(f = 1) {
//					*next_tag_part = 2;
//					 snprintf(pcInsert, iInsertLen, "part1 ");
//					 return strlen(pcInsert);
//					 break;
//				}
//
//			}
//			break;
//			switch (current_tag_part) {
//			    case 0:
//			      snprintf(pcInsert, iInsertLen, "part0");
//			      *next_tag_part = 1;
//			      printf("Ok 1 \n");
//			      return strlen(pcInsert);
//			      break;
//			    case 1:
//			      snprintf(pcInsert, iInsertLen, "part1");
//			      *next_tag_part = 2;
//			      printf("Ok 2 \n");
//			      return strlen(pcInsert);
//			      break;
//			    case 2:
//			      snprintf(pcInsert, iInsertLen, "part2");
//			      printf("Ok 3 \n");
//			      return strlen(pcInsert);
//			      break;
//			    default:
//			      printf("No \n");
//			      break;
//			    }
			#else
		printf("LWIP_HTTPD_SSI_MULTIPART disabled /n");
#endif
		break;
	case 1:
		indx1 += 1;
		sprintf(pcInsert, "%d", indx1);
		return strlen(pcInsert);
		break;
	case 2:
		strcat(json, "[");
		for (int i = 0; i <= tabline; i++) {
			/**************************************************************/
			sprintf(arrline, "{\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d}",
										T[variable].pulluptype, T[variable].bounceinterval, T[variable].holdinterval,
										T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval,
										T[variable].preventclick, T[variable].infofield, T[variable].onoff);
			/**************************************************************/
			strcat(json, arrline); // объеденяем строки
			if (i != tabline)
				strcat(json, ",");
		}
		strcat(json, "]");
		printf(" %s\n", json);
		//pcInsert = json;
		sprintf(pcInsert, "%s", json);
		memset(json, '\0', sizeof(json));
		memset(arrline, '\0', sizeof(arrline));
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

const tCGI FORM_CGI = { "/form.cgi", CGIForm_Handler }; // Создаем структуру CGI

char name[30];

tCGI CGI_TAB[2]; // 3. For form создадим массив для form CGI

const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	printf("OK GET \r\n");
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

//////////////////// POST START ////////////////////////
static void *current_connection;
static void *valid_connection;
int http_post_content_len;

err_t httpd_post_begin(void *connection, const char *uri,
		const char *http_request, uint16_t http_request_len, int content_len,
		char *response_uri, uint16_t response_uri_len, uint8_t *post_auto_wnd) {

	printf("OK POST 1 \r\n");
	//LWIP_UNUSED_ARG(connection);
	//LWIP_UNUSED_ARG(http_request);
	//LWIP_UNUSED_ARG(http_request_len);
	//LWIP_UNUSED_ARG(content_len);
	//LWIP_UNUSED_ARG(post_auto_wnd);
	http_post_content_len = content_len;

	//printf("httpd_post_begin: Post Content: %s\n",  http_request);
	//printf("%d \r\n",  http_post_content_len);

	if (current_connection != connection) {

		current_connection = connection;
		valid_connection = NULL;

		/* default page is "login failed" */
		snprintf(response_uri, response_uri_len, "/cgi_leds.html");
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
		strcat(bufervar, p->payload);
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

void httpd_post_finished(void *connection, char *response_uri,
		u16_t response_uri_len) {
	printf("OK POST 3 \r\n");

	char *token;
	char *token1;
	char *token2;
	char *name1;
	int z = 0;
	int id;
	char urlline[http_post_content_len];

	strncpy(urlline, bufervar, http_post_content_len);

	printf("%s \r\n", urlline);
	printf("COUNT %d \r\n", http_post_content_len);

	char *rest = urlline;

	while ((token = strtok_r(rest, "&", &rest))) {
		//printf("%s\n", token);
		char *rest1 = token;
		y = 0;
		while ((token1 = strtok_r(rest1, "=", &rest1))) {
			z = 0;
			if (y == 0) {
				//printf("NAME %s\n", token1);
				//name = token1; // имя переменной
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
				// записывае в структуру
				/************************************************/
				if (strcmp(name1, "pulluptype") == 0) {
					strcpy(T[id].pulluptype, token1);
				} else if (strcmp(name1, "bounceinterval") == 0) {
					T[id].bounceinterval = atoi(token1);
				} else if (strcmp(name1, "holdinterval") == 0) {
					T[id].holdinterval = atoi(token1);
				} else if (strcmp(name1, "repeat") == 0) {
					T[id].repeat = atoi(token1);
				} else if (strcmp(name1, "repeatinterval") == 0) {
					T[id].repeatinterval = atoi(token1);
				} else if (strcmp(name1, "doubleclickinterval") == 0) {
					T[id].doubleclickinterval = atoi(token1);
				} else if (strcmp(name1, "preventclick") == 0) {
					T[id].preventclick = atoi(token1);
				} else if (strcmp(name1, "infofield") == 0) {
					strcpy(T[id].infofield, token1);
				} else if (strcmp(name1, "onoff") == 0) {
					T[id].onoff = atoi(token1);
				}
				/************************************************/
			}
			y++;
		}
		z++;
	}
	memset(urlline, '\0', sizeof(urlline));
	memset(bufervar, '\0', sizeof(bufervar));
	memset(json, '\0', sizeof(json));
	//printf("httpd_post_finished! \r\n");
	/* default page is "login failed" */

	zerg_write_data_to_sd(num_lines-1);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	snprintf(response_uri, response_uri_len, "/tab.shtml");
	if (current_connection == connection) {
		if (valid_connection == connection) {
			/* login succeeded */
			snprintf(response_uri, response_uri_len, "/tab.shtml");
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
	CGI_TAB[0] = FORM_CGI; // CGI структуру

	//http_set_cgi_handlers (&FORM_CGI, 1); // Это пример когда у нас всего один "Handler" пример из видео.
	http_set_cgi_handlers(CGI_TAB, 1); // 5. И теперь мы передадим структуру массива в CGI_handle!
}
