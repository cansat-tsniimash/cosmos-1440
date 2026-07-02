/*
 * math.h
 *
 *  Created on: May 23, 2026
 *      Author: ChatDeepseekGPTCapilot
 *
 *      WE BELIVE IN THIS CODE
 */

#ifndef MATH_MATH_H_
#define MATH_MATH_H_

#include "stm32f1xx.h"

typedef struct {
    float X;
    float Y;
    float Z;
} rectangular_system_data_t;


rectangular_system_data_t math(float latitude_target_gps, float longitude_target_gps, float altitude_target_gps, float latitude_now_gps, float longitude_now_gps, float altitude_now_gps, const float q[4]);

float conversion_radians(float num);

float radius_curvature(float latitude);

rectangular_system_data_t transform_rectangular_system(float latitude, float longitude, float altitude);

void transformation_into_topocentric_system(float latitude_now, float longitude_now, rectangular_system_data_t target, rectangular_system_data_t now, rectangular_system_data_t *enu);

#endif /* MATH_MATH_H_ */
