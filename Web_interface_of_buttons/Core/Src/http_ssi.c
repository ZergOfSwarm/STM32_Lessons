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
//char json[0]; //2500 было!  в 1-й строке 198 json ответ от сервера 58 строк. http://192.168.18.125/tabjson.shtml
char bufervar[12500]; // в 1-й строке 201 символ POST request, 58 строк. 12000
//char arrline[1400];  //1400 было!
struct table T[num_lines]; // Создали массив 'T' структурного типа, размер = 'num_lines'

//struct tablepins tp[num_lines];

int tabline = (num_lines - 1);
static int variable = 0;

/* we will use character "x", "y","z" as tag for SSI */
char const *TAGCHAR[] = { "json", "test", "jsontag" };
char const **TAGS = TAGCHAR;
int f = 0;

struct tablepins { // Создали структуру с необходимым набором типов элиментов для PIN's.
	char pins[5];
	char port[2];
	int number;
	uint32_t hal_pin;
	uint32_t gpio_name;
} tp[] = { "PA0", "A", 0, GPIO_PIN_0, GPIOA, "PA4", "A", 4, GPIO_PIN_4, GPIOA,
		"PA5", "A", 5,
		GPIO_PIN_5, GPIOA, "PA6", "A", 6, GPIO_PIN_6, GPIOA, "PA8", "A", 8,
		GPIO_PIN_8, GPIOA, "PA9", "A", 9, GPIO_PIN_9, GPIOA, "PA10", "A", 10,
		GPIO_PIN_10, GPIOA, "PA11", "A", 11,
		GPIO_PIN_11, GPIOA, "PA12", "A", 12, GPIO_PIN_12, GPIOA, "PA15", "A",
		15, GPIO_PIN_15, GPIOA, "PB0", "B", 0, GPIO_PIN_0, GPIOB, "PB1", "B", 1,
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
		GPIO_PIN_11, GPIOD, "PD12", "D", 12, GPIO_PIN_12, GPIOD, "PD13", "D",
		13, GPIO_PIN_13, GPIOD, "PD14", "D", 14, GPIO_PIN_14, GPIOD, "PD15",
		"D", 15, GPIO_PIN_15, GPIOD, "PE0", "E", 0, GPIO_PIN_0, GPIOE, "PE1",
		"E", 1, GPIO_PIN_1, GPIOE, "PE2", "E", 2, GPIO_PIN_2, GPIOE, "PE3", "E",
		3, GPIO_PIN_3,GPIOE, "PE4", "E", 4, GPIO_PIN_4,GPIOE, "PE5", "E", 5,
		GPIO_PIN_5, GPIOE, "PE6", "E", 6, GPIO_PIN_6, GPIOE, "PE7", "E", 7,
		GPIO_PIN_7, GPIOE, "PE8", "E", 8, GPIO_PIN_8, GPIOE, "PE9", "E", 9,
		GPIO_PIN_9, GPIOE, "PE10", "E", 10,
		GPIO_PIN_10, GPIOE, "PE11", "E", 11, GPIO_PIN_11, GPIOE, "PE12", "E",
		12, GPIO_PIN_12, GPIOE, "PE13", "E", 13, GPIO_PIN_13, GPIOE, "PE14",
		"E", 14, GPIO_PIN_14, GPIOE, "PE15", "E", 15, GPIO_PIN_15, GPIOE };

void test_function(void) {
	int flag_A, flag_B, flag_C, flag_D, flag_E = 0;
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; // Для инициализации порта/ов нам необходимо объявить структуру
	for (int id = 0; id <= (num_lines - 1); id++) {
		if (T[id].onoff == 1) {
			printf("test_function %s %d %s %d %lu\r\n", tp[id].pins, id,
					tp[id].port, tp[id].number);
			if (strcmp(tp[id].port, "A") == 0 && flag_A != 1) { // Если пин на порту 'A'
				flag_A = 1;
				__HAL_RCC_GPIOA_CLK_ENABLE(); // Включаем тактирование порта 'A'

			} else if (strcmp(tp[id].port, "B") == 0) { // Если пин на порту 'B'
				flag_B = 1;
				__HAL_RCC_GPIOB_CLK_ENABLE(); // Включаем тактирование порта 'B
				GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Включаем режим выход
			} else if (strcmp(tp[id].port, "C") == 0) { // Если пин на порту 'C'
				flag_C = 1;
				__HAL_RCC_GPIOC_CLK_ENABLE(); // Включаем тактирование порта 'C'
				GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Включаем режим выход
			} else if (strcmp(tp[id].port, "D") == 0) { // Если пин на порту 'D'
				flag_D = 1;
				__HAL_RCC_GPIOD_CLK_ENABLE(); // Включаем тактирование порта 'D'
				GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Включаем режим выход
			} else if (strcmp(tp[id].port, "E") == 0) { // Если пин на порту 'E'
				flag_E = 1;
				__HAL_RCC_GPIOE_CLK_ENABLE(); // Включаем тактирование порта 'E'
				GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Включаем режим выход
			}
			// Для всех портов
			GPIO_InitStruct.Pin = tp[id].hal_pin; // Номер вывода (uint32_t)
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Включаем режим выход GPIO_MODE_INPUT GPIO_MODE_ANALOG GPIO_MODE_OUTPUT_PP
			if (strcmp(T[id].pulluptype, "GPIO_NOPULL") == 0 ) { // "GPIO_PULLUP"  "GPIO_PULLDOWN"
				GPIO_InitStruct.Pull = GPIO_NOPULL;
			}else if(strcmp(T[id].pulluptype, "GPIO_PULLUP") == 0 ){
				GPIO_InitStruct.Pull = GPIO_PULLUP;
			}else if(strcmp(T[id].pulluptype, "GPIO_PULLDOWN") == 0 ){
				GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			}else{
				GPIO_InitStruct.Pull = GPIO_NOPULL;
			}
//			GPIO_InitStruct.Pull = GPIO_PULLDOWN; // Пин кнопки притягиваем к земле 'Pull-Down'.
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Скорость переключение пина
			HAL_GPIO_Init(tp[id].gpio_name, &GPIO_InitStruct); // Функция для управления портом 'A'

		} else {

			//printf("test_function-ERROR r\n");
		}
		if (T[id].onoff == 0){
			HAL_GPIO_DeInit(tp[id].gpio_name, tp[id].hal_pin); // сброс конфигурации не активных пинов
		}
	}
}

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen,
		u16_t current_tag_part, u16_t *next_tag_part) {
	//char *info[30] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	char *info;
	switch (iIndex) {
	case 0:
#if LWIP_HTTPD_SSI_MULTIPART
		if (variable == num_lines) {
			//default:
			//printf("Exit from function! \n");
			variable = 0;
			//return strlen(pcInsert);
			break;
		} else {
			//if (isalpha(T[variable].infofield[0])) {

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

			if (variable == 0) {
				sprintf(pcInsert,
						"[{\"pins\":\"%s\",\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d},",
						tp[variable].pins, T[variable].pulluptype,
						T[variable].bounceinterval, T[variable].holdinterval,
						T[variable].repeat, T[variable].repeatinterval,
						T[variable].doubleclickinterval,
						T[variable].preventclick, info, T[variable].onoff);
			} else if (variable == num_lines - 1) {
				sprintf(pcInsert,
						"{\"pins\":\"%s\",\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d}]",
						tp[variable].pins, T[variable].pulluptype,
						T[variable].bounceinterval, T[variable].holdinterval,
						T[variable].repeat, T[variable].repeatinterval,
						T[variable].doubleclickinterval,
						T[variable].preventclick, info, T[variable].onoff);
			} else {
				sprintf(pcInsert,
						"{\"pins\":\"%s\",\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d},",
						tp[variable].pins, T[variable].pulluptype,
						T[variable].bounceinterval, T[variable].holdinterval,
						T[variable].repeat, T[variable].repeatinterval,
						T[variable].doubleclickinterval,
						T[variable].preventclick, info, T[variable].onoff);
			}
			/***********************************************************************/
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
		indx1 += 1;
		sprintf(pcInsert, "%d", indx1);
		return strlen(pcInsert);
		break;
	case 2:
//		strcat(json, "[");
//		for (int i = 0; i <= tabline; i++) {
//			/**************************************************************/
//			sprintf(arrline, "{\"pulluptype\":\"%s\",\"bounceinterval\":%d,\"holdinterval\":%d,\"repeat\":%d,\"repeatinterval\":%d,\"doubleclickinterval\":%d,\"preventclick\":%d,\"infofield\":\"%s\",\"onoff\":%d}",
//										T[variable].pulluptype, T[variable].bounceinterval, T[variable].holdinterval,
//										T[variable].repeat, T[variable].repeatinterval, T[variable].doubleclickinterval,
//										T[variable].preventclick, T[variable].infofield, T[variable].onoff);
//			/**************************************************************/
//			strcat(json, arrline); // объеденяем строки
//			if (i != tabline)
//				strcat(json, ",");
//		}
//		strcat(json, "]");
//		printf(" DON' WORK!!! %s\n", json);
//		//pcInsert = json;
//		sprintf(pcInsert, "%s", json);
//		memset(json, '\0', sizeof(json));
//		memset(arrline, '\0', sizeof(arrline));
//		return strlen(pcInsert);
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

	//printf("OK POST 1 \r\n");
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

	//printf("OK POST 2 \r\n");
	char *data;

	if (current_connection == connection && p != NULL) {

		data = p->payload;
		//printf("DATA %s \r\n", data);
		strcat(bufervar, p->payload);
		pbuf_free(p);
		memset(data, '\0', sizeof(*data));
		return ERR_OK;
	}
	if (p != NULL) {
		//printf("END \r\n");
		pbuf_free(p);
	}
	return ERR_VAL;
}

void httpd_post_finished(void *connection, char *response_uri,
		u16_t response_uri_len) {
	//printf("OK POST 3 \r\n");

	char name0[50];
	char nametoken[50];
	char *token;
	char *token1;
	char *token2;
	char *name1;
	int z = 0;
	int id;
	char urlline[http_post_content_len];

	strncpy(urlline, bufervar, http_post_content_len);
	//printf("%s \r\n", urlline);
	//printf("COUNT %d \r\n", http_post_content_len);

	char *rest = urlline;

	while ((token = strtok_r(rest, "&", &rest))) {
		//printf("TOKEN - %s\n", token);
		char *rest1 = token;
//		char *name0 = token;
		strcat(name0, token);
		y = 0;
		while ((token1 = strtok_r(rest1, "=", &rest1))) {
			z = 0;
			if (y == 0) {
				//printf("---NAME %s\n", token1);
				//name0 = token1; // имя переменной
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
				//printf("VAL-222 \n");
				//printf("VAL %s\n", token1);
				// записывае в структуру
				//printf("---NAME-1 %s\n", name1);
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
		sprintf(nametoken, "infofield_%d=", id); // Если поле пустое то его заполняем "null".
		if (strcmp(name0, nametoken) == 0) {
			strcpy(T[id].infofield, "null");
			//printf("Yes \n");
		}
		z++;
		memset(name0, '\0', sizeof(name0));
		memset(nametoken, '\0', sizeof(nametoken));
	}
	memset(urlline, '\0', sizeof(urlline));
	memset(bufervar, '\0', sizeof(bufervar));
//memset(json, '\0', sizeof(json));
//printf("httpd_post_finished! \r\n");
	/* default page is "login failed" */

	zerg_write_data_to_sd(num_lines - 1);

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
	test_function();

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
