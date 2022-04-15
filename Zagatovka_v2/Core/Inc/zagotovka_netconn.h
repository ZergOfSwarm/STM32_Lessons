#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

char parse(const char *line);
void zagotovka_netconn_init(void);
void DynWebPage(struct netconn *conn);
#define num_lines 1                // Укажи число строк 58
void My_Form_Page(struct netconn *conn, int id);
struct table {                       // Создали структуру с необходимым набором типов элиментов.
	char pulluptype[20]; // было 40
	int bounceinterval;
	int holdinterval;
	int repeat;
	int repeatinterval;
	int doubleclickinterval;
	int preventclick;
	char infofield[30]; // было 20
	int onoff;
};

#endif /* __HTTPSERVER_NETCONN_H__ */
