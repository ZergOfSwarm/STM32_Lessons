/*
 * tcpserver.c
 *
 *  Created on: Apr 20, 2022
 *      Author: controllerstech.com
 */


#include "lwip/opt.h"

#include "lwip/api.h"
#include "lwip/sys.h"

#include "tcpserver.h"
#include "string.h"
static struct netconn *conn, *newconn;
static struct netbuf *buf;
static ip_addr_t *addr;
static unsigned short port;
char msg[100];
char smsg[200];


/**** Send RESPONSE every time the client sends some data ******/
static void tcp_thread(void *arg)
{
	err_t err, accept_err, recv_error;
	int cleaned_len = 0;
	char cleaned_msg[100];//переменную для хранения очищенного сообщения
	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	if (conn!=NULL)
	{
		/* Bind connection to the port number 7. */
		err = netconn_bind(conn, IP_ADDR_ANY, 10);// port

		if (err == ERR_OK)
		{
			/* Tell connection to go into listening mode. */
			netconn_listen(conn);

			while (1)
			{
				/* Grab new connection. */
				accept_err = netconn_accept(conn, &newconn);

				/* Process the new connection. */
				if (accept_err == ERR_OK)
				{

					/* receive the data from the client */
					while (netconn_recv(newconn, &buf) == ERR_OK)
					{
						/* Extrct the address and port in case they are required */
						ip_addr_t client_addr;
						ip_addr_copy(client_addr, *netbuf_fromaddr(buf));  // get the address of the client
						port = netbuf_fromport(buf);  // get the Port of the client

						/* If there is some data remaining to be sent, the following process will continue */
						do
						{
							strncpy (msg, buf->p->payload, buf->p->len);   // get the message from the client
							printf("We received a message - %s\r\n",msg);

//							// Очистите сообщение от специальных символов, оставив только буквы и цифры
//							char* msg_ptr = (char*)buf->p->payload;
//							cleaned_len = 0;
//							int payload_len = buf->p->len;
//							for (int i = 0; i < payload_len && cleaned_len < sizeof(cleaned_msg) - 1; i++) {
//							    char c = msg_ptr[i];
//							    if (isalnum(c) || isspace(c)) {
//							        cleaned_msg[cleaned_len++] = c;
//							    }
//							}
//							cleaned_msg[cleaned_len] = '\0';

							// Сохраните очищенное сообщение в переменной msg
//							strncpy(msg, cleaned_msg, sizeof(msg)); // Уберем один байт для нулевого символа
//							msg[sizeof(msg) - 1] = '\0'; // Установим нулевой символ вручную

							if (strlen(msg) == 2 && strcmp(msg, "ON") == 0) {
								printf("LED is - %s\r\n",msg);
							} else if  (strlen(msg) == 3 && strcmp(msg, "OFF") == 0) {
								printf("LED is - %s\r\n",msg);
							} else {
							    // Код, выполняемый при неверном значении msg
							    printf("Incorrect value of msg: %s\n", msg);
							}
						    printf("The number of characters: %d\n", strlen(msg));
							// Or modify the message received, so that we can send it back to the client
							int len = sprintf (smsg, "\"%s\" was sent by the Server\n", msg);

							netconn_write(newconn, smsg, len, NETCONN_COPY);  // send the message back to the client
							memset (msg, '\0', 100);  // clear the buffer
						}
						while (netbuf_next(buf) >0);

						netbuf_delete(buf);
					}

					/* Close connection and discard connection identifier. */
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		}
		else
		{
			netconn_delete(conn);
		}
	}
}


void tcpserver_init(void)
{
  sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
}
