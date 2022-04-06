#include "lwip/opt.h"
 #include "lwip/arch.h"
 #include "lwip/api.h"

 #include "httpserver-netconn.h"

 #if LWIP_NETCONN

 #ifndef HTTPD_DEBUG
 #define HTTPD_DEBUG         LWIP_DBG_OFF
 #endif

 static const char http_html_hdr[] =
     "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
 static const char http_index_html[] =
 "<html><head><title>Congrats!</title></head>\
 <body><h1 align=\"center\">Hello World!</h1>\
 <h2 align=\"center\">Welcome to Fire lwIP HTTP Server!</h1>\
 <p align=\"center\">This is a small test page, served by httpserver-netconn.</p>\
 <p align=\"center\"><a href=\"http://www.firebbs.cn/forum.php/\">\
 <font size=\"6\"> Zerg - it is part of SWARM! </font> </a></p>\
 <a href=\"http://http://192.168.18.129/\">\
 <p align=\"center\"><img src=\"https://linkbuilder.su/images/uploads/glossary/web_site.jpg\" /></a>\
 </body></html>";

 /** Serve one HTTP connection accepted in the http thread */
 static void
 http_server_netconn_serve(struct netconn *conn)
 {
     struct netbuf *inbuf;
     char *buf;
     u16_t buflen;
     err_t err;

     /* читаем данные */
     err = netconn_recv(conn, &inbuf);

     if (err == ERR_OK)
     {
         netbuf_data(inbuf, (void**)&buf, &buflen);

         /* Суждение - это команда Get для HTTP*/
         if (buflen>=5 &&
                 buf[0]=='G' &&
                 buf[1]=='E' &&
                 buf[2]=='T' &&
                 buf[3]==' ' &&
                 buf[4]=='/' )
         {

             /* Отправка данных */
             netconn_write(conn, http_html_hdr,
             sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
             /* Отправка данных веб-страницы */
             netconn_write(conn, http_index_html,
                 sizeof(http_index_html)-1, NETCONN_NOCOPY);
         }
     }
     netconn_close(conn); /* Выключить соединение */

     /* Выпуск INBUF */
     netbuf_delete(inbuf);
 }

 /** The main function, never returns! */
 static void
 http_server_netconn_thread(void *arg)
 {
     struct netconn *conn, *newconn;
     err_t err;
     LWIP_UNUSED_ARG(arg);

     /* Создать структуру подключения Netconn*/
     /* Номер порта 80 и IP-адрес*/
     conn = netconn_new(NETCONN_TCP);
     netconn_bind(conn, IP_ADDR_ANY, 80);

     LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);

     /* 监听 */
     netconn_listen(conn);

     do
     {
         //Запрос на обработку подключения
         err = netconn_accept(conn, &newconn);
         if (err == ERR_OK)
         {
             //Отправить данные веб-страницы
             http_server_netconn_serve(newconn);

             //Удалить структуру подключения
             netconn_delete(newconn);
         }
     }
     while (err == ERR_OK);
     //закрытие
     netconn_close(conn);
     netconn_delete(conn);
 }

 /** Initialize the HTTP server (start its thread) */
 void
 http_server_netconn_init(void)
 {
 sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL, 2048, 4);
 }

 #endif
