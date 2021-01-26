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
/** tab page holder */
static lv_obj_t *hmi_tabview;
/** button navigate to tab control */
static lv_obj_t *hmi_button_control;
/** button navigate to tab settings */
static lv_obj_t *hmi_button_settings;
/** button navigate to tab about */
static lv_obj_t *hmi_button_about;
/** current light switch state [off, on] */
static lv_obj_t *hmi_led_light_switch;
/** current heater switch state [off, on] */
static lv_obj_t *hmi_led_heater_switch;
/** current exhaust fan switch state [off, on] */
static lv_obj_t *hmi_led_exhaust_switch;
/** current recirculation fan switch state [off, on] */
static lv_obj_t *hmi_led_recirculation_switch;

#endif /* _HMI_H_ */
