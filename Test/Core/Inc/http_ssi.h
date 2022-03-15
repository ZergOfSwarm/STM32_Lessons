/*
 * http_ssi.h
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */

#ifndef INC_HTTP_SSI_H_
#define INC_HTTP_SSI_H_
#define num_lines 70  // Укажи число строк

void http_server_init (void);

struct table{    // Создали структуру с необходимым набором типов элиментов.
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

#endif /* INC_HTTP_SSI_H_ */
