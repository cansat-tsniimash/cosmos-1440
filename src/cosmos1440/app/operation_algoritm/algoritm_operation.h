/*
 * algoritm_operation.h
 *
 *  Created on: Mar 28, 2026
 *      Author: boron
 */

#ifndef OPERATION_ALGORITM_ALGORITM_OPERATION_H_
#define OPERATION_ALGORITM_ALGORITM_OPERATION_H_

typedef enum
{
    OA_WAIT_GPS = 0,
    OA_CHECK_LIGHT, //1
    OA_PREPARATION,//2
    OA_ROCKET,//3
    OA_FIFRE,//4
    OA_CONTROL,//5
    OA_DECLINE//6
} operation_algoritm_t;

#endif /* OPERATION_ALGORITM_ALGORITM_OPERATION_H_ */
