#ifndef _HMI_H_
#define _HMI_H_

#include "lvgl/lvgl.h"

void hmi_initialize();

/*
 * -------
 * Toolbar
 * -------
 */

/** current time */
static lv_obj_t *hmi_label_clock;
/** current circadian [day, night] */
static lv_obj_t *hmi_label_circadian;
/** current control mode [off, manual, automatic] */
static lv_obj_t *hmi_label_control_mode;
/** current light switch state [off, on] */
static lv_obj_t *hmi_led_light_switch;
/** current heater switch state [off, on] */
static lv_obj_t *hmi_led_heater_switch;
/** current exhaust fan switch state [off, on] */
static lv_obj_t *hmi_led_exhaust_switch;
/** current recirculation fan switch state [off, on] */
static lv_obj_t *hmi_led_recirculation_switch;

#endif /* _HMI_H_ */
