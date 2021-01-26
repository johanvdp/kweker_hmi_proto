#include "hmi_control.h"

#define HMI_CONTROL_W 120
#define HMI_CONTROL_H 180
#define HMI_CONTROL_BAR_W 10
#define HMI_CONTROL_BAR_H 100

uint16_t hmi_control_get_y(lv_obj_t *bar, int16_t value) {
	uint16_t min = lv_bar_get_min_value(bar);
	uint16_t max = lv_bar_get_max_value(bar);
	uint16_t y = lv_obj_get_y(bar);
	uint16_t height = lv_obj_get_height(bar);

	float fraction = (float) (value - min) / (max - min);
	return y + height - (fraction * height) - 10;
}

void hmi_control_update(hmi_control_t *target, int16_t pv, int16_t sv,
bool hi, bool lo) {
	lv_obj_t *bar = target->bar;
	lv_obj_t *label_sv = target->label_sv;
	lv_obj_t *label_pv = target->label_pv;
	lv_obj_t *label_lo = target->label_lo;
	lv_obj_t *label_hi = target->label_hi;

	uint16_t bar_x = lv_obj_get_x(bar);
	uint16_t bar_width = lv_obj_get_width(bar);

	// pv
	lv_bar_set_value(bar, pv, LV_ANIM_OFF);
	lv_label_set_text_fmt(label_pv, "=%2d", pv);
	uint16_t pv_y = hmi_control_get_y(bar, pv);
	lv_obj_set_pos(label_pv, bar_x + bar_width + 2, pv_y);

	// sv
	lv_label_set_text_fmt(label_sv, "%2d>", sv);
	uint16_t label_width = lv_obj_get_width(label_sv);
	uint16_t sv_y = hmi_control_get_y(bar, sv);
	lv_obj_set_pos(label_sv, bar_x - label_width - 2, sv_y);

	// lo
	lv_obj_set_style_local_text_color(label_lo, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, lo ? LV_COLOR_RED : LV_COLOR_GRAY);

	// hi
	lv_obj_set_style_local_text_color(label_hi, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, hi ? LV_COLOR_RED : LV_COLOR_GRAY);
}

void hmi_control_create_control(hmi_control_t *target, lv_obj_t *parent,
		lv_coord_t x, lv_coord_t y, const char *name, int16_t min, int16_t max) {

	lv_obj_t *control = lv_cont_create(parent, NULL);
	lv_obj_set_pos(control, x, y);
	lv_obj_set_size(control, HMI_CONTROL_W, HMI_CONTROL_H);
	// nothing to select or edit
	lv_obj_clean_style_list(control, LV_OBJ_PART_MAIN);

	lv_obj_t *bar = lv_bar_create(control, NULL);
	lv_obj_set_size(bar, HMI_CONTROL_BAR_W, HMI_CONTROL_BAR_H);
	lv_bar_set_range(bar, min, max);
	lv_bar_set_type(bar, LV_BAR_TYPE_NORMAL);
	lv_obj_align(bar, control, LV_ALIGN_CENTER, 0, 0);
	target->bar = bar;
	uint16_t bar_x = lv_obj_get_x(bar);
	uint16_t bar_width = lv_obj_get_width(bar);

	lv_obj_t *label_hi = lv_label_create(control, NULL);
	lv_label_set_text(label_hi, "HI");
	lv_obj_set_style_local_text_color(label_hi, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_obj_align(label_hi, bar, LV_ALIGN_OUT_TOP_MID, 0, 0);
	target->label_hi = label_hi;

	lv_obj_t *label_name = lv_label_create(control, NULL);
	lv_label_set_text(label_name, name);
	lv_obj_align(label_name, label_hi, LV_ALIGN_OUT_TOP_MID, 0, 0);

	lv_obj_t *label_lo = lv_label_create(control, NULL);
	lv_label_set_text(label_lo, "LO");
	lv_obj_set_style_local_text_color(label_lo, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_obj_align(label_lo, bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	target->label_lo = label_lo;

	lv_obj_t *label_max = lv_label_create(control, NULL);
	lv_label_set_text_fmt(label_max, "-%2d", max);
	uint16_t max_y = hmi_control_get_y(bar, max);
	lv_obj_set_pos(label_max, bar_x + bar_width + 2, max_y);

	lv_obj_t *label_min = lv_label_create(control, NULL);
	uint16_t min_y = hmi_control_get_y(bar, min);
	lv_obj_set_pos(label_min, bar_x + bar_width + 2, min_y);
	lv_label_set_text_fmt(label_min, "-%2d", min);

	lv_obj_t *label_pv = lv_label_create(control, NULL);
	uint16_t pv_y = hmi_control_get_y(bar, min);
	lv_obj_set_pos(label_pv, bar_x + bar_width + 2, pv_y);
	lv_label_set_text_fmt(label_pv, "=%2d", min);
	target->label_pv = label_pv;

	lv_obj_t *label_sv = lv_label_create(control, NULL);
	lv_label_set_text_fmt(label_sv, "%2d>", min);
	uint16_t label_width = lv_obj_get_width(label_sv);
	uint16_t sv_y = hmi_control_get_y(bar, min);
	lv_obj_set_pos(label_sv, bar_x - label_width - 2, sv_y);
	target->label_sv = label_sv;
}

lv_obj_t* hmi_control_create_tab(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "Control");
	// disable manual sliding between tabs
	lv_page_set_scroll_propagation(tab, false);

	hmi_control_create_control(&hmi_control_temperature, tab, 1, 1,
			"Temperature [°C]", 0, 50);
	hmi_control_update(&hmi_control_temperature, 26, 30, false, true);

	hmi_control_create_control(&hmi_control_humidity, tab, 1 + HMI_CONTROL_W, 1,
			"Humidity [%RH]", 0, 100);
	hmi_control_update(&hmi_control_humidity, 46, 40, true, false);

	hmi_control_create_control(&hmi_control_co2, tab, 1 + HMI_CONTROL_W * 2, 1,
			"CO2 conc. [ppm]", 0, 2000);
	hmi_control_update(&hmi_control_co2, 415, 800, false, true);

	return tab;
}
