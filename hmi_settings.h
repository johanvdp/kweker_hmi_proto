#ifndef _HMI_SETTINGS_H_
#define _HMI_SETTINGS_H_

#include "lvgl/lvgl.h"

/*
 * -------------
 * Tab: Settings
 * -------------
 */
lv_obj_t* hmi_settings_create_tab(lv_obj_t *parent);

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

#endif /* _HMI_SETTINGS_H_ */
