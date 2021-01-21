#ifndef _HMI_H_
#define _HMI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/src/lvgl.h"
#endif


void hmi_create();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _HMI_H_ */
