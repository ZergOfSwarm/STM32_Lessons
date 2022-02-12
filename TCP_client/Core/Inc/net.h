/*
 * net.h
 *
 *  Created on: Feb 12, 2022
 *      Author: denis
 */

#ifndef INC_NET_H_
#define INC_NET_H_
//-----------------------------------------------
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lwip.h"
#include "lwip/tcp.h"
//-----------------------------------------------
#include "lwip/tcp.h"

//-----------------------------------------------
typedef struct USART_prop{
uint8_t usart_buf[26]; // Буфер для UASRT увеличь еслинужно передовать большие строки!
uint8_t usart_cnt;
uint8_t is_tcp_connect;//статус попытки создать соединение TCP с сервером
uint8_t is_text;//статус попытки передать текст серверу
} USART_prop_ptr;
//-----------------------------------------------
void net_ini(void);

void UART6_RxCpltCallback(void);
//-----------------------------------------------

#endif /* INC_NET_H_ */
