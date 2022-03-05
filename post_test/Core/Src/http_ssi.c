/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "fs.h"
#include "httpd.h"
#include "multipart_parser.h"

#include "http_ssi.h"
#include "stdio.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "stm32f4xx_hal.h"

char joined_var[700];
char *token;
char *token1;
/*******************/
char *token2;
//char *name;
char *name1;
int k = 0;
int id;

//int indx1 = 0;
int y;
char table1[300];

struct table {
	char pulluptype[40];
	int bounceinterval;
	int holdinterval;
	int repeat;
	int repeatinterval;
	int doubleclickinterval;
	int preventclick;
	char infofield[20];
	int onoff;
};

struct table T[3]; // Укажем количество строк в таблице!
/*******************/
int y = 0;
int flag = 0;

/************************ SSI HANDLER ***************************/
int indx = 0;
int indx1 = 0;
char bufervar[300];
char joned_str[500];
/* we will use character "x", "y","z" as tag for SSI */
char const *TAGCHAR[] = { "tab", "test" }; //
char const **TAGS = TAGCHAR;

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
	switch (iIndex) {
	case 0:
		for (int h = 0; h <= 2; h++) { // 9 для 10 строк
			sprintf(table1,
					"<tr><td>%d</td><td>Pin%d</td><td><input type=\"text\" name=\"pulluptype_%d\" size=\"10\" value=\"%s\"></td> \n",
					h,h,h, T[h].pulluptype);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"bounceinterval_%d\" size=\"10\" value=\"%d\"></td> \n",
					h, T[h].bounceinterval);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"holdinterval_%d\" size=\"10\" value=\"%d\"></td> \n",
					h, T[h].holdinterval);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"repeat_%d\" size=\"10\" value=\"%d\"></td> \n",
					h, T[h].repeat);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"repeatinterval_%d\" size=\"10\" value=\"%d\"></td> \n",
					h, T[h].repeatinterval);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"doubleclickinterval_%d\" size=\"10\" value=\"%d\"></td> \n",
					h, T[h].doubleclickinterval);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"preventclick_%d\" size=\"10\" value=\"%d\"></td> \n",
					h, T[h].preventclick);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"text\" name=\"infofield_%d\" size=\"10\" value=\"%s\"></td> \n",
					h, T[h].infofield);
			strcat(pcInsert, table1);
			sprintf(table1,
					"<td><input type=\"number\" name=\"onoff_%d\" size=\"10\" value=\"%d\"></td></tr> \n",
					h, T[h].onoff);
			strcat(pcInsert, table1);
		}
		return strlen(pcInsert);
		break;
	case 1:
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
const tCGI FORM_CGI = { "/form.cgi", CGIForm_Handler }; // Создаем структуру CGI
char name[300]; // Если цикл перебора найдет "fname" сохранит в переменную 'name[30]' а если найдет "lname" он сканкатенирует сюда данные "lname".

const char* CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	printf("OK GET \r\n");
	if (iIndex == 0) {
		for (int i = 0; i < iNumParams; i++) // Цикл перебора параметров ("fname" и "lname") из html странички
				{
			if (strcmp(pcParam[i], "Bounce_interval") == 0) // if the fname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				strcpy(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("Bounce_interval: %s\r\n", name);
			} else if (strcmp(pcParam[i], "Hold_interval") == 0) // if the lname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				//strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("Hold_interval: %s\r\n", name);
			} else if (strcmp(pcParam[i], "Repeat") == 0) // if the lname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				//strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("Repeat: %s\r\n", name);
			} else if (strcmp(pcParam[i], "Repeat_interval") == 0) // if the lname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				//strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("Repeat_interval: %s\r\n", name);
			} else if (strcmp(pcParam[i], "Double_click_interval") == 0) // if the lname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				//strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("Double_click_interval: %s\r\n", name);
			} else if (strcmp(pcParam[i], "Prevent_Click") == 0) // if the lname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				//strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("Prevent_Click: %s\r\n", name);
			} else if (strcmp(pcParam[i], "OnOff") == 0) // if the lname string is found
					{
				memset(name, '\0', 300); // Очищаем массив перед записью новых данных.
				//strcat(name, " "); // Сконкатенируем fname с lname разделяя пробелом.
				strcat(name, pcValue[i]); // Сохраняем значение в переменную.
				printf("OnOff: %s\r\n", name);
			}
		}
	}
	return "/cgiform.shtml"; // По окончании работы 'CGIForm_Handler' возвращаем указатель в нужный файл.
}
//////////////////// POST START ////////////////////////
char *content_type;

/* Find Header Key Name in the header. */
const char*
find_header_name(char *header) {

#define HEADER_NAME_TITLE               "name="
#define HEADER_NAME_TITLE_LEN           5

	if (header != NULL) {
		char *header_name_begin = strstr(header, HEADER_NAME_TITLE); // Find name= in Header
		char *header_name = strtok(header_name_begin, "\""); // Find the first "
		header_name = strtok(NULL, "\"");					// Go to the last "
#ifdef  HTTPD_DEBUG
		//printf("POST multipart Header Key found: %s\n", header_name);
#endif

		return header_name;
	}
	return NULL;
}

/* Find boundary value in the Content-Type. */
const char*
find_boundary( content_type) {

#define BOUNDARY_TITLE               "boundary="
#define BOUNDARY_TITLE_LEN           9

	if (content_type != NULL) {
		char *boundary_begin = strstr(content_type, BOUNDARY_TITLE);// Find Boundary= in Content-Type
		char *boundary = boundary_begin + BOUNDARY_TITLE_LEN;// Remove the Boundary=
#ifdef  HTTPD_DEBUG
		//printf("POST multipart Boundary found: %s\n", boundary);
#endif

		return boundary;
	}
	return NULL;
}

// List of accepted URI for POST requests
multipart_parser *parser;
static uint8_t http_post_uri_file_index = 0;
static uint32_t http_post_content_len = 0;
#define HTTP_POST_URI_NUM	2
const char *a[HTTP_POST_URI_NUM] = { "/cgiform.shtml", "/form.cgi" };

/*
 * Mulitpart Parser settings
 *
 * read_on_part_data_begin: (nul
 * read_header_name: Content-Disposition: read_header_value: form-data; name="key_name"
 * read_on_headers_complete: (null)
 * read_part_data: form_value			// May be called multiple times if a file
 * read_on_part_data_end: (null)
 * read_on_body_end: (null)
 *
 */
multipart_parser_settings callbacks;
multipart_parser *_parser;

/* Header which contains the Key with the name */
int read_header_name(multipart_parser *p, const char *at, size_t length) {
#ifdef  HTTPD_DEBUG
	//printf("read_header_name: %.*s: \n", length, at);
#endif

	/* Parse the Header Value */
	/* Content-Disposition: read_header_value: form-data; name="variable_name" */
	char *key_name = find_header_name(at);

#ifdef  HTTPD_DEBUG
	//printf("Key Name: %s\n", key_name);
#endif

	return 0;
}

int read_header_value(multipart_parser *p, const char *at, size_t length) {
#ifdef  HTTPD_DEBUG
	//printf("read_header_value: %.*s\n", length, at);
#endif
	return 0;
}

/* Value for the latest key */
/* If this is a file, this may be called multiple times. */
/* Wait until part_end for the complete file. */
int read_part_data(multipart_parser *p, const char *at, size_t length) {
#ifdef  HTTPD_DEBUG
	//printf("read_part_data: %.*s\n", length, at);
#endif
	return 0;
}

/* Beginning of a key and value */
int read_on_part_data_begin(multipart_parser *p, const char *at, size_t length) {
	//printf("Total length is - %d\n", http_post_content_len);
#ifdef  HTTPD_DEBUG
	//printf("read_on_part_data_begin: %.*s\n", length, at);
#endif
	return 0;
}

/* End of header which contains the key */
int read_on_headers_complete(multipart_parser *p, const char *at, size_t length) {
#ifdef  HTTPD_DEBUG
	//printf("read_on_headers_complete: %.*s\n", length, at);
#endif
	return 0;
}

/** End of the key and value */
/* If this is a file, the file is complete. */
/* If this is a value, then the value is complete. */
int read_on_part_data_end(multipart_parser *p, const char *at, size_t length) {
#ifdef  HTTPD_DEBUG
	//printf("read_on_part_data_end: %.*s\n", length, at);
#endif
	return 0;
}

/* End of the entire form */
int read_on_body_end(multipart_parser *p, const char *at, size_t length) {
#ifdef  HTTPD_DEBUG
	//printf("read_on_body_end: %.*s\n", length, at);
#endif
	return 0;
}

static err_t http_parse_post(char *data, uint32_t length) {
#ifdef  HTTPD_DEBUG
	//printf("http_parse_post POST data: %s\n", data);
#endif

	/* Parse the data */
	multipart_parser_execute(_parser, data, length);

	return ERR_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Called when a POST request has been received. The application can decide
 * whether to accept it or not.
 *
 * @param connection Unique connection identifier, valid until httpd_post_end
 *        is called.
 * @param uri The HTTP header URI receiving the POST request.
 * @param http_request The raw HTTP request (the first packet, normally).
 * @param http_request_len Size of 'http_request'.
 * @param content_len Content-Length from HTTP header.
 * @param response_uri Filename of response file, to be filled when denying the
 *        request
 * @param response_uri_len Size of the 'response_uri' buffer.
 * @param post_auto_wnd Set this to 0 to let the callback code handle window
 *        updates by calling 'httpd_post_data_recved' (to throttle rx speed)
 *        default is 1 (httpd handles window updates automatically)
 * @param content_type Content-Type string.
 * @return ERR_OK: Accept the POST request, data may be passed in
 *         another err_t: Deny the POST request, send back 'bad request'.
 */
err_t httpd_post_begin(void *connection, const char *uri,
		const char *http_request, uint16_t http_request_len, int content_len,
		char *response_uri, uint16_t response_uri_len, uint8_t *post_auto_wnd) {

	// Check the URI given with the list
	for (uint8_t i = 0; i < HTTP_POST_URI_NUM; i++) {
		if (strcmp(uri, a[i]) == 0) {

			http_post_uri_file_index = i;
			http_post_content_len = content_len;

#ifdef  HTTPD_DEBUG
			//printf("httpd_post_begin: Post Content: %s\n",  http_request);
#endif

			memset(&callbacks, 0, sizeof(multipart_parser_settings));

			callbacks.on_header_field = read_header_name;
			callbacks.on_header_value = read_header_value;
			callbacks.on_part_data = read_part_data;
			callbacks.on_part_data_begin = read_on_part_data_begin;
			callbacks.on_headers_complete = read_on_headers_complete;
			callbacks.on_part_data_end = read_on_part_data_end;
			callbacks.on_body_end = read_on_body_end;

			/*
			 * Get the boundary from the content-type
			 * Then pass it to the parser
			 */
			const char *boundary = find_boundary(content_type);
			if (boundary != NULL) {
				_parser = multipart_parser_init(boundary, &callbacks);
			}

			return ERR_OK;
		}
	}

	//returns /404.html when response_uri is empty
	return ERR_VAL;
}

/** Called for each pbuf of data that has been received for a POST.
 * ATTENTION: The application is responsible for freeing the pbufs passed in!
 *
 * @param connection Unique connection identifier.
 * @param p Received data.
 * @return ERR_OK: Data accepted.
 *         another err_t: Data denied, http_post_get_response_uri will be called.
 */
err_t httpd_post_receive_data(void *connection, struct pbuf *p) {

	char *data;
	err_t ret_val = ERR_ARG;

	struct http_state *hs = (struct http_state*) connection;
	if (hs != NULL && p != NULL) {
		data = p->payload;
		ret_val = http_parse_post(data, http_post_content_len);
		//printf("%s\r\n", data);
	}
	/****************************************************/
	strcat(joined_var, data); // Объединение строк.
	//printf("+++++++++++++%s\n", joined_var);
	printf("\r\n");

	if (p != NULL) {
		pbuf_free(p);
	}
	//printf("************%d\r\n", ret_val);
	return ret_val;
}

/** Called when all data is received or when the connection is closed.
 * The application must return the filename/URI of a file to send in response
 * to this POST request. If the response_uri buffer is untouched, a 404
 * response is returned.
 *
 * @param connection Unique connection identifier.
 * @param response_uri Filename of response file on success
 * @param response_uri_len Size of the 'response_uri' buffer.
 */
void httpd_post_finished(void *connection, char *response_uri,
		u16_t response_uri_len) {

	struct http_state *hs = (struct http_state*) connection;
	if (hs != NULL) {
		strncpy(response_uri, a[http_post_uri_file_index], response_uri_len);
	}
	char *rest = joined_var;
	printf("++++++++joined_var++++++++%s\n", joined_var);
	//printf("-------rest------%s\n", rest);

	while ((token = strtok_r(rest, "&", &rest))) {
		//printf("%s\n", token); // Имя переменной
		char *rest1 = token;
		y = 0;
		while ((token1 = strtok_r(rest1, "=", &rest1))) {
			k = 0;
			if (y == 0) {
				printf("NAME %s\n", token1); // Value переменной
				// имя переменной
				//name = token1;
				// получение id
				char *rest2 = token1;
				while ((token2 = strtok_r(rest2, "_", &rest2))) {
					if (k == 0) {
						name1 = token2;
					}
					if (k == 1) {
						id = atoi(token2);
						printf("ID %d\n", id);
					}
					k++;
				}
			}
			if (y == 1) {
				// значение переменной
				printf("VALUE %s\n", token1);
				// записывае в структуру
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
			}
			y++;
		}

	}
	k++;

//	 printf("%s * %s * %s \n", T[0].pulluptype, T[1].pulluptype, T[2].pulluptype);
//	 printf("%d * %d * %d \n", T[0].bounceinterval, T[1].bounceinterval, T[2].bounceinterval);
//	 printf("%d * %d * %d \n", T[0].holdinterval, T[1].holdinterval, T[2].holdinterval);

	/* End the parser */
	multipart_parser_free(_parser);
}
////////////////////  END of POST request ////////////////////////

void http_server_init(void) {
	httpd_init();
	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 2);
	http_set_cgi_handlers(&FORM_CGI, 1); // Это пример когда у нас всего один "Handler" пример из видео.
}
