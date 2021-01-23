#ifndef _HMI_H_
#define _HMI_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/src/lvgl.h"
#endif

/*
 * Initialize
 */
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

/*
 * ------------
 * Tab: Control
 * ------------
 */

/** current temperature [0..50.0 ℃]  */
static lv_obj_t *hmi_label_current_temperature;
/** current humidity [0..99.0 %RH]  */
static lv_obj_t *hmi_label_current_humidity;
/** current CO2 concentration [0..2000 ppm]  */
static lv_obj_t *hmi_label_current_co2;
/** setpoint temperature [0..50.0 ℃]  */
static lv_obj_t *hmi_label_setpoint_temperature;
/** setpoint humidity [0..99.0 %RH]  */
static lv_obj_t *hmi_label_setpoint_humidity;
/** setpoint CO2 concentration [0..2000 ppm]  */
static lv_obj_t *hmi_label_setpoint_co2;
/** indicator temperature low [on, off] */
static lv_obj_t *hmi_led_control_temperature_low;
/** indicator temperature high [on, off] */
static lv_obj_t *hmi_led_control_temperature_high;
/** indicator humidity low [on, off] */
static lv_obj_t *hmi_led_control_humidity_low;
/** indicator humidity high [on, off] */
static lv_obj_t *hmi_led_control_humidity_high;
/** indicator CO2 concentration low [on, off] */
static lv_obj_t *hmi_led_control_co2_low;
/** indicator CO2 concentration high [on, off] */
static lv_obj_t *hmi_led_control_co2_high;
/** selector control mode off */
static lv_obj_t *hmi_btn_control_mode_off;
/** selector control mode manual */
static lv_obj_t *hmi_btn_control_mode_manual;
/** selector control mode automatic */
static lv_obj_t *hmi_btn_control_mode_automatic;
/** setpoint manual light switch */
static lv_obj_t *hmi_btn_light_switch;
/** setpoint manual heater switch */
static lv_obj_t *hmi_btn_heater_switch;
/** setpoint manual exhaust fan switch */
static lv_obj_t *hmi_btn_exhaust_switch;
/** setpoint manual recirculation fan switch */
static lv_obj_t *hmi_btn_recirculation_switch;

/*
 * -------------
 * Tab: Settings
 * -------------
 */
/** set current time hour */
static lv_obj_t *hmi_spinbox_time_hour;
/** set current time minute */
static lv_obj_t *hmi_spinbox_time_minute;
/** set begin of day time hour */
static lv_obj_t *hmi_spinbox_begin_of_day_hour;
/** set begin of day time minute */
static lv_obj_t *hmi_spinbox_begin_of_day_minute;
/** set begin of night time hour */
static lv_obj_t *hmi_spinbox_begin_of_night_hour;
/** set begin of night time minute */
static lv_obj_t *hmi_spinbox_begin_of_night_minute;
/** set day temperature */
static lv_obj_t *hmi_spinbox_day_temperature;
/** set day humidity */
static lv_obj_t *hmi_spinbox_day_humidity;
/** set day CO2 concentration */
static lv_obj_t *hmi_spinbox_day_co2;
/** set night temperature */
static lv_obj_t *hmi_spinbox_night_temperature;
/** set night humidity */
static lv_obj_t *hmi_spinbox_night_humidity;
/** set night CO2 concentration */
static lv_obj_t *hmi_spinbox_night_co2;

/*
 * ----------
 * Tab: About
 * ----------
 */
/** about page content */
static lv_obj_t *hmi_label_about;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _HMI_H_ */
