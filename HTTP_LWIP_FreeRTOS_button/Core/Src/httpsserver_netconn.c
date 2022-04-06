#include "lwip/opt.h"
 #include "lwip/arch.h"
 #include "lwip/api.h"

 #include "httpserver-netconn.h"
#include "stdbool.h" /* Для True/False*/
 #if LWIP_NETCONN

 #ifndef HTTPD_DEBUG
 #define HTTPD_DEBUG         LWIP_DBG_OFF
 #endif


 const static char http_html_hdr[] =
     "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";

 const unsigned char Led1On_Data[] =
     "<HTML> \
 <head><title>HTTP LED Control</title></head> \
 <center> \
 <p> \
 <font size=\"6\">LED<font style = \"color:red\">已打开！</font> \
 <form method=post action=\"off\" name=\"ledform\"> \
 <input type=\"submit\" value=\"关闭\" style=\"width:80px;height:30px;\"></form> \
 </center> \
 </HTML> ";

 // Когда светодиод выключен, верните браузер на следующую HTML информацию,
 // результаты отображения отображаются на рисунке 15-7.
 const unsigned char Led1Off_Data[] =
     "<HTML> \
 <head><title>HTTP LED Control</title></head> \
 <center> \
 <p> \
 <font size=\"6\">LED<font style = \"color:red\">OFF！</font> \
 <form method=post action=\"on\" name=\"ledform\"> \
 <input type=\"submit\" value=\"-OFF-\" style=\"width:80px;height:30px;\"></form> \
 </center> \
 </HTML> ";

 static const char http_index_html[] =
     "<html><head><title>Congrats!</title></head>\
 <body><h2 align=\"center\">Welcome to Fire lwIP HTTP Server!</h2>\
 <p align=\"center\">This is a small test page : http control led.</p>\
 <p align=\"center\"><a href=\"https://google.com/\">\
 <font size=\"6\"> Link to google </font> </a></p>\
 <a href=\"http://www.firebbs.cn/forum.php/\">\
 <img src=\"http://www.firebbs.cn/data/attachment/portal/201806/\
 05/163015rhz7mbgbt0zfujzh.jpg\"/></a>\
 </body></html>";

 static bool led_on = false;

 /*Отправить данные веб-страницы*/
 void httpserver_send_html(struct netconn *conn, bool led_status)
 {
     //Отправить данные
     netconn_write(conn, http_html_hdr,
                 sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

     /* В зависимости от состояния LED, отправьте разные LED данные */
     if (led_status == true)
         netconn_write(conn, Led1On_Data,
                     sizeof(Led1On_Data)-1, NETCONN_NOCOPY);
     else
         netconn_write(conn, Led1Off_Data,
                     sizeof(Led1Off_Data)-1, NETCONN_NOCOPY);

     netconn_write(conn, http_index_html,
                 sizeof(http_index_html)-1, NETCONN_NOCOPY);

 }

 static void httpserver_serve(struct netconn *conn)
 {
     struct netbuf *inbuf;
     char *buf;
     u16_t buflen;
     err_t err;

     /* В ожидании команд от клиента */
     err = netconn_recv(conn, &inbuf);

     if (err == ERR_OK)
     {
         netbuf_data(inbuf, (void**)&buf, &buflen);
         /* “GET”命令 */
         if (buflen>=5 &&
                 buf[0]=='G' &&
                 buf[1]=='E' &&
                 buf[2]=='T' &&
                 buf[3]==' ' &&
                 buf[4]=='/' )
         {

             /* Отправить данные */
             httpserver_send_html(conn, led_on);
         }
         //“POST” запрос
         else if (buflen>=8&&buf[0]=='P'&&buf[1]=='O'
                 &&buf[2]=='S'&&buf[3]=='T')
         {
             if (buf[6]=='o'&&buf[7]=='n')
             {
                 //Запрос на включение LED
                 led_on = true;
                 //LED1_ON;
                 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);// ON
             }
             else if (buf[6]=='o'&&buf[7]=='f'&&buf[8]=='f')
             {
                 //Запрос на отключени LED
                 led_on = false;
                 //LED1_OFF;
                 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);// OFF
             }
             //Отправить данные
             httpserver_send_html(conn, led_on);
         }

         netbuf_delete(inbuf);
     }
     /* закрытие */
     netconn_close(conn);

 }

 /** The main function, never returns! */
 static void
 httpserver_thread(void *arg)
 {
     struct netconn *conn, *newconn;
     err_t err;
     LWIP_UNUSED_ARG(arg);

     /* Создать структуру соединения */
     conn = netconn_new(NETCONN_TCP);
     LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);

     led_on = true;
     //LED1_ON;
     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);// ON

     /* Связка IP-адрес и номер порта*/
     netconn_bind(conn, NULL, 80);

     /* мониторинг */
     netconn_listen(conn);

     do
     {
         err = netconn_accept(conn, &newconn);
         if (err == ERR_OK)
         {
             httpserver_serve(newconn);
             netconn_delete(newconn);
         }
     }
     while (err == ERR_OK);

     netconn_close(conn);

     netconn_delete(conn);
 }

 /** Initialize the HTTP server (start its thread) */
 void
 httpserver_init()
 {
 sys_thread_new("http_server_netconn",httpserver_thread, NULL, 1024, 4);
 }

 #endif
