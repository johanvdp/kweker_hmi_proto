#ifndef _HMI_ABOUT_H_
#define _HMI_ABOUT_H_

#include "lvgl/lvgl.h"

/*
 * ----------
 * Tab: About
 * ----------
 */
lv_obj_t* hmi_about_create_tab(lv_obj_t *parent);

/** about page content */
extern lv_obj_t *hmi_label_about;

#endif /* _HMI_ABOUT_H_ */
