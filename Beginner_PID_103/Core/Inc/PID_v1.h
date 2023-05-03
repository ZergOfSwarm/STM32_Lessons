/*
 * PID_v1.h
 *
 *  Created on: 21 апр. 2023 г.
 *      Author: denis
 */

#ifndef INC_PID_V1_H_
#define INC_PID_V1_H_

#define MANUAL 0
#define AUTOMATIC 1
#define DIRECT 0
#define REVERSE 1

void Compute();
void SetTunings(double Kp, double Ki, double Kd);
void SetSampleTime(int NewSampleTime);
void SetOutputLimits(double Min, double Max);
void Initialize();
void SetMode(int Mode);
void SetControllerDirection(int Direction);


#endif /* INC_PID_V1_H_ */
