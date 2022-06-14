/**
 ******************************************************************************
 *  @file SMA_filter_lib.h
 *  @brief Простое скользящее среднее, или арифметическое скользящее среднее 
 *  (англ. simple moving average, англ. SMA)
 *  @author Волков Олег
 *  @date 12.06.2022
 *
  ******************************************************************************
 * @attention
 * 
 *  Библиотека помогает отфильтровать данные с 12 битного АЦП микроконтроллера, 
 *  приведя показания к более устойчивым.
 *  
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md
 *  Группа ВК: https://vk.com/solderingiron.stm32
 *
 ******************************************************************************
 */

#ifndef __SMA_FILTER_LIB_H
#define __SMA_FILTER_LIB_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

/* Макросы -------------------------------------------------------------------------------------------------------------------------------*/
#define SMA_FILTER_ORDER 32 /* Порядок SMA фильтра */

/* Выберите версию фильтра(Делают они одно и то же, просто разными способами. Какой из них оптимальнее - решать Вам)*/
#define SMA_VERSION_1
//#define SMA_VERSION_2
	
	
/* Прототипы функций ----------------------------------------------------------------------------------------------------------------------*/
	
#if defined (SMA_VERSION_1)
	uint16_t SMA_FILTER_Get_Value(uint16_t *SMA_Filter_buffer, uint16_t *RAW_Data);
#elif defined (SMA_VERSION_2)
	uint16_t SMA_FILTER_Get_Value(uint8_t *SMA_Filter_counter, uint16_t *SMA_Filter_buffer, uint32_t *SMA_Filter_Result, uint16_t *RAW_Data);
#endif

	
	
#ifdef __cplusplus
}
#endif

#endif /* __SMA_FILTER_LIB_H */

