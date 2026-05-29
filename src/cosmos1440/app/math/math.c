/*
 * math.c
 *
 *  Created on: May 23, 2026
 *      Author: ChatDeepseekGPTCapilot
 *
 *      WE BELIVE IN THIS CODE
 */

#include "math/math.h"
#include <math.h>

#define a (6378000)
#define b (6356780)
#define PI_F (3.14159265358979323846)

float conversion_radians(float num);
float radius_curvature(float latitude);

rectangular_system_data_t transform_rectangular_system(float latitude, float longitude, float altitude);

void transformation_into_topocentric_system(float latitude_now, float longitude_now, rectangular_system_data_t target, rectangular_system_data_t now, rectangular_system_data_t *enu);

rectangular_system_data_t math(float latitude_target_gps, float longitude_target_gps,float altitude_target_gps, float latitude_now_gps, float longitude_now_gps, float altitude_now_gps)
{
    rectangular_system_data_t target_xyz = {0};
    rectangular_system_data_t now_xyz = {0};
    rectangular_system_data_t enu = {0};

    target_xyz = transform_rectangular_system(conversion_radians(latitude_target_gps), conversion_radians(longitude_target_gps),altitude_target_gps);
    now_xyz = transform_rectangular_system(conversion_radians(latitude_now_gps), conversion_radians(longitude_now_gps),altitude_now_gps);

    transformation_into_topocentric_system(latitude_now_gps, longitude_now_gps, target_xyz, now_xyz, &enu);

    return enu;
}

rectangular_system_data_t transform_rectangular_system(float latitude, float longitude, float altitude)
{
    rectangular_system_data_t rectangular_system_data = {0};

    rectangular_system_data.X = (radius_curvature(latitude) + altitude) * cosf(latitude) * cosf(longitude);
    rectangular_system_data.Y = (radius_curvature(latitude) + altitude) * cosf(latitude) * sinf(longitude);
    rectangular_system_data.Z = (((b * b) / (a * a)) * radius_curvature(latitude) + altitude) * sinf(latitude);

    return rectangular_system_data;
}

float radius_curvature(float latitude)
{
    float res = (a * a) / sqrtf((a * a) * powf(cosf(latitude), 2) + (b * b) * powf(sinf(latitude), 2));

    return res;
}

float conversion_radians(float num)
{
	return num * PI_F / 180;
}

void transformation_into_topocentric_system(float latitude_now, float longitude_now, rectangular_system_data_t target, rectangular_system_data_t now, rectangular_system_data_t *enu)
{
    float lat = conversion_radians(latitude_now);
    float lon = conversion_radians(longitude_now);

    float dX = target.X - now.X;
    float dY = target.Y - now.Y;
    float dZ = target.Z - now.Z;

    enu->X = -sinf(lon) * dX + cosf(lon) * dY;
    enu->Y = -sinf(lat) * cosf(lon) * dX - sinf(lat) * sinf(lon) * dY + cosf(lat) * dZ;
    enu->Z = cosf(lat) * cosf(lon) * dX + cosf(lat) * sinf(lon) * dY + sinf(lat) * dZ;
}
