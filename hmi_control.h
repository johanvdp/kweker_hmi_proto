#ifndef _HMI_CONTROL_H_
#define _HMI_CONTROL_H_

#include "lvgl/lvgl.h"

/*
 * ------------
 * Tab: Control
 * ------------
 */

typedef struct {
	lv_obj_t *bar;
	lv_obj_t *label_sv;
	lv_obj_t *label_pv;
	lv_obj_t *label_lo;
	lv_obj_t *label_hi;
} hmi_control_t;

lv_obj_t* hmi_control_create_tab(lv_obj_t *parent);

/** temperature control */
extern hmi_control_t hmi_control_temperature;
/** humidity control */
extern hmi_control_t hmi_control_humidity;
/** CO2 concentration control */
extern hmi_control_t hmi_control_co2;

/** selector control mode off */
extern lv_obj_t *hmi_btn_control_mode_off;
/** selector control mode manual */
extern lv_obj_t *hmi_btn_control_mode_manual;
/** selector control mode automatic */
extern lv_obj_t *hmi_btn_control_mode_automatic;

/** setpoint manual light switch */
extern lv_obj_t *hmi_btn_light_switch;
/** setpoint manual heater switch */
extern lv_obj_t *hmi_btn_heater_switch;
/** setpoint manual exhaust fan switch */
extern lv_obj_t *hmi_btn_exhaust_switch;
/** setpoint manual recirculation fan switch */
extern lv_obj_t *hmi_btn_recirculation_switch;

#endif /* _HMI_CONTROL_H_ */
