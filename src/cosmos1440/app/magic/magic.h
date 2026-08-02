/*
 * magic.h
 *
 *  Created on: Apr 11, 2026
 *      Author: ChatDeepseekGPTCapilot
 */

#ifndef MAGIC_H_
#define MAGIC_H_

void MadgwickAHRSupdate(float* quaternion, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float dt, float beta);
void MadgwickAHRSupdateIMU(float* quaternion, float gx, float gy, float gz, float ax, float ay, float az, float dt, float beta);

#endif /* MAGIC_H_ */
