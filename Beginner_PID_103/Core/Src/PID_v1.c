/*
 * PID_v1.c
 *
 *  Created on: 21 апр. 2023 г.
 *      Author: denis
 */

#include <PID_v1.h>
#include <stdbool.h>
#include <stdint.h>
unsigned long lastTime; // Время в миллисекундах (через функцию millis()) в момент последнего вычисления регулятора.
double Input; // Tекущее значение измерения (например, температуры).
uint32_t Output; // Tекущее значение управляющего воздействия (например, ШИМ сигнала).
double Setpoint; // Желаемое значение измерения.
double ITerm; // Значение интегральной составляющей регулятора.
double lastInput; //Значение измерения в момент последнего вычисления регулятора.
double kp; // Коэффициент пропорциональной составляющих регулятора
double ki; // Коэффициент интегральной составляющих регулятора
double kd; // Коэффициент дифференциальной составляющих регулятора
int SampleTime = 1; //1000 Время (ms) между вычислениями регулятора.
double outMin; // минимальное ограничение на значение управляющего воздействия.
double outMax; // Максимальное ограничение на значение управляющего воздействия.
bool inAuto = true; // Флаг, указывающий, находится ли регулятор в автоматическом режиме.
extern uint32_t HAL_GetTick();
double error;

int controllerDirection = DIRECT;

/* Compute() - Вычисляет новое значение Output на основе текущих значений Input,
 Setpoint и коэффициентов регулятора (kp, ki, kd).
 Результат сохраняется в Output.
 Функция вызывается каждый раз, когда проходит достаточно времени (SampleTime) с момента последнего вычисления.
 */
	void Compute()
	{
	   if(!inAuto) return;
	   unsigned long now = HAL_GetTick();
	   int timeChange = (now - lastTime);
	   if(timeChange>=SampleTime)
	   {
	      /*Compute all the working error variables*/
	      error = Setpoint - Input;
	      ITerm+= (ki * error);
	      if(ITerm > outMax) ITerm= outMax;
	      else if(ITerm < outMin) ITerm= outMin;
	      double dInput = (Input - lastInput);

	      /*Compute PID Output*/
	      Output = kp * error + ITerm- kd * dInput;
	      if(Output > outMax) Output = outMax;
	      else if(Output < outMin) Output = outMin;

	      /*Remember some variables for next time*/
	      lastInput = Input;
	      lastTime = now;
	   }
	}
/*
SetTunings() - Задает новые значения коэффициентов регулятора (kp, ki, kd) и пересчитывает их
в зависимости от SampleTime.
*/
	void SetTunings(double Kp, double Ki, double Kd)
	{
	   if (Kp<0 || Ki<0|| Kd<0) return;

	  double SampleTimeInSec = ((double)SampleTime)/1000;
	   kp = Kp;
	   ki = Ki * SampleTimeInSec;
	   kd = Kd / SampleTimeInSec;

	  if(controllerDirection ==REVERSE)
	   {
	      kp = (0 - kp);
	      ki = (0 - ki);
	      kd = (0 - kd);
	   }
	}
/*
SetSampleTime() - Задает новое значение SampleTime и пересчитывает коэффициенты ki и kd в зависимости
от изменения времени между вычислениями.
*/
	void SetSampleTime(int NewSampleTime)
	{
	   if (NewSampleTime > 0)
	   {
	      double ratio  = (double)NewSampleTime
	                      / (double)SampleTime;
	      ki *= ratio;
	      kd /= ratio;
	      SampleTime = (unsigned long)NewSampleTime;
	   }
	}
/*
SetOutputLimits() - Задает новые значения коэффициентов регулятора (kp, ki, kd) и
пересчитывает их в зависимости от SampleTime.
*/
	void SetOutputLimits(double Min, double Max)
	{
	   if(Min > Max) return;
	   outMin = Min;
	   outMax = Max;

	   if(Output > outMax) Output = outMax;
	   else if(Output < outMin) Output = outMin;

	   if(ITerm > outMax) ITerm= outMax;
	   else if(ITerm < outMin) ITerm= outMin;
	}
/*
nitialize() - Задает новые значения коэффициентов регулятора (kp, ki, kd) и
пересчитывает их в зависимости от SampleTime.
*/
	void Initialize()
	{
	   lastInput = Input;
	   ITerm = Output;
	   if(ITerm > outMax) ITerm= outMax;
	   else if(ITerm < outMin) ITerm= outMin;
	}
/*
SetMode() Задает новые значения коэффициентов регулятора (kp, ki, kd) и
пересчитывает их в зависимости от SampleTime.
*/
	void SetMode(int Mode)
	{
	    bool newAuto = (Mode == AUTOMATIC);
	    if(newAuto == !inAuto)
	    {  /*we just went from manual to auto*/
	        Initialize();
	    }
	    inAuto = newAuto;
	}
/*SetControllerDirection() - Задает новые значения коэффициентов регулятора (kp, ki, kd)
и пересчитывает их в зависимости от SampleTime.
*/
	void SetControllerDirection(int Direction)
	{
	   controllerDirection = Direction;
	}
