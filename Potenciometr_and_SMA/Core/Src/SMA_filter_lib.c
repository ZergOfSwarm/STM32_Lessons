/**
 ******************************************************************************
 *  @file SMA_filter_lib.c
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

#include "SMA_filter_lib.h"

#if defined (SMA_VERSION_1)
/* Создать в main.c ----------------------------------------------------------------*/
  //uint16_t SMA_Filter_Buffer_1[SMA_FILTER_ORDER] = { 0, };
/*----------------------------------------------------------------------------------*/

#elif defined (SMA_VERSION_2)
/* Создать в main.c ----------------------------------------------------------------*/
  //uint16_t SMA_Filter_Buffer_1[SMA_FILTER_ORDER] = { 0, };
  //uint8_t SMA_Filter_counter_1 = 0;
  //uint32_t SMA_Filter_Result_1 = 0;

/*----------------------------------------------------------------------------------*/
#endif


#if defined (SMA_VERSION_1)
/**
 ******************************************************************************
 *  @breif Функция получения отфильтрованного значения(SMA Filter)
 *  @param  *SMA_Filter_buffer - Массив, где будут лежать сырые значения 
 *  @param  *RAW_Data - Сырые данные	         
 *  @retval  SMA_Filter_Result - Значение, полученное после SMA фильтра.
 ******************************************************************************
 */
uint16_t SMA_FILTER_Get_Value(uint16_t *SMA_Filter_buffer, uint16_t *RAW_Data) {
	
	/* Создадим переменную для суммы сырых значений */
	uint32_t SMA_Filter_Result = 0;
	
	/* Начнем заполнять массив сырыми значениями с конца */
	SMA_Filter_buffer[SMA_FILTER_ORDER - 1] = *RAW_Data;
	
	/* Просуммируем все элементы массива */
	for (uint8_t i = 0; i < SMA_FILTER_ORDER; i++) {
		SMA_Filter_Result += SMA_Filter_buffer[i];
	}
	
	/*
	 * Найдем среднее арифметическое значение:
	 * Если SMA_FILTER_ORDER -это 2 в какой-то степени, то можно деление заменить смещением
	 * Пример: SMA_FILTER_ORDER = 32;
	 * 32 = 2*2*2*2*2;
	 * Тогда SMA_Filter_Result = SMA_Filter_Result/32 будет равен, как SMA_Filter_Result = SMA_Filter_Result >> 5u;
	 *
	 */
	//SMA_Filter_Result = SMA_Filter_Result / SMA_FILTER_ORDER;
	SMA_Filter_Result = SMA_Filter_Result >> 5u; //32 = 2^5;
		
	/* Сдвинем все элементы массива влево на 1 */
	for (uint8_t i = 0; i < SMA_FILTER_ORDER; i++) {
		SMA_Filter_buffer[i] = SMA_Filter_buffer[i + 1];
	}
	
	return (uint16_t)SMA_Filter_Result; //Вернем среднее арифметическое значение
}

#elif defined (SMA_VERSION_2)
/**
 ******************************************************************************
 *  @breif Функция получения отфильтрованного значения(SMA Filter)
 *  @param  *SMA_Filter_counter - Счетчик перемещения по буферу с сырыми данными
 *  @param  *SMA_Filter_buffer - Массив, где будут лежать сырые значения 	         
 *  @param *RAW_Data - Сырые данные
 *  @retval  *SMA_Filter_Result - Значение, полученное после SMA фильтра.
 ******************************************************************************
 */
uint16_t SMA_FILTER_Get_Value(uint8_t *SMA_Filter_counter, uint16_t *SMA_Filter_buffer, uint32_t *SMA_Filter_Result, uint16_t *RAW_Data) {
	
	/* Если счетчик достигнет конца буфера - сбросим его */
	if (++ *SMA_Filter_counter >= SMA_FILTER_ORDER) {
		*SMA_Filter_counter = 0;
	} 
	
	/* Из результирующего значения вычтем самое старое*/
	*SMA_Filter_Result -= SMA_Filter_buffer[*SMA_Filter_counter];
	
	/* Прибавим к результирующему значению сырые данные*/
	*SMA_Filter_Result += *RAW_Data;
	
	/* Добавим к массиву сырые данные */
	SMA_Filter_buffer[*SMA_Filter_counter] = *RAW_Data;

	/*
	 * Найдем среднее арифметическое значение:
	 * Если SMA_FILTER_ORDER -это 2 в какой-то степени, то можно деление заменить смещением
	 * Пример: SMA_FILTER_ORDER = 32;
	 * 32 = 2*2*2*2*2;
	 * Тогда SMA_Filter_Result = SMA_Filter_Result/32 будет равен, как SMA_Filter_Result = SMA_Filter_Result >> 5u;
	 *
	 */
	//return *SMA_Filter_Result / SMA_FILTER_ORDER;
	return *SMA_Filter_Result >> 5u; //Вернем среднее арифметическое значение
}
#endif
