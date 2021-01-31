#include "hmi_control.h"

#define HMI_CONTROL_W 110
#define HMI_CONTROL_H 270
#define HMI_MARGIN 5

/** temperature control */
hmi_control_t hmi_control_temperature;
/** humidity control */
hmi_control_t hmi_control_humidity;
/** CO2 concentration control */
hmi_control_t hmi_control_co2;

/** selector control mode */
lv_obj_t *hmi_control_mode_btnmatrix;
/** setpoint manual control */
lv_obj_t *hmi_control_manual_btnmatrix;

/**
 * Calculate location of value on control bar.
 */
static lv_coord_t hmi_control_get_y(lv_obj_t *bar, int16_t value) {

	lv_coord_t min = lv_bar_get_min_value(bar);
	lv_coord_t max = lv_bar_get_max_value(bar);
	lv_coord_t y = lv_obj_get_y(bar);
	lv_coord_t height = lv_obj_get_height(bar);

	float fraction = (float) (value - min) / (max - min);
	return y + height - (fraction * height) - 10;
}

static void hmi_control_update(hmi_control_t *target, int16_t pv, int16_t sv,
bool hi, bool lo) {

	lv_obj_t *bar = target->bar;
	lv_obj_t *label_sv = target->label_sv;
	lv_obj_t *label_pv = target->label_pv;
	lv_obj_t *label_lo = target->label_lo;
	lv_obj_t *label_hi = target->label_hi;

	lv_coord_t bar_x = lv_obj_get_x(bar);
	lv_coord_t bar_width = lv_obj_get_width(bar);

	// pv
	lv_bar_set_value(bar, pv, LV_ANIM_OFF);
	lv_label_set_text_fmt(label_pv, "=%2d", pv);
	lv_coord_t pv_y = hmi_control_get_y(bar, pv);
	lv_obj_set_pos(label_pv, bar_x + bar_width + HMI_MARGIN, pv_y);

	// sv
	lv_label_set_text_fmt(label_sv, "%2d>", sv);
	lv_coord_t label_width = lv_obj_get_width(label_sv);
	lv_coord_t sv_y = hmi_control_get_y(bar, sv);
	lv_obj_set_pos(label_sv, bar_x - label_width - HMI_MARGIN, sv_y);

	// lo
	lv_obj_set_style_local_text_color(label_lo, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, lo ? LV_COLOR_RED : LV_COLOR_GRAY);

	// hi
	lv_obj_set_style_local_text_color(label_hi, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, hi ? LV_COLOR_RED : LV_COLOR_GRAY);
}

static void hmi_control_create_control(hmi_control_t *target, lv_obj_t *parent,
		lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,
		const char *name, int16_t min, int16_t max) {

	lv_obj_t *control = lv_cont_create(parent, NULL);
	lv_obj_clean_style_list(control, LV_CONT_PART_MAIN);
	lv_obj_set_pos(control, x, y);
	lv_obj_set_size(control, w, h);

	lv_obj_t *label_name = lv_label_create(control, NULL);
	lv_label_set_text(label_name, name);
	lv_obj_align(label_name, control, LV_ALIGN_IN_TOP_MID, 0, HMI_MARGIN);

	lv_obj_t *label_hi = lv_label_create(control, NULL);
	lv_label_set_text(label_hi, "HI");
	lv_obj_set_style_local_text_color(label_hi, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_obj_align(label_hi, label_name, LV_ALIGN_OUT_BOTTOM_MID, 0, HMI_MARGIN);
	target->label_hi = label_hi;

	lv_obj_t *label_lo = lv_label_create(control, NULL);
	lv_label_set_text(label_lo, "LO");
	lv_obj_set_style_local_text_color(label_lo, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_obj_align(label_lo, control, LV_ALIGN_IN_BOTTOM_MID, 0, -HMI_MARGIN);
	target->label_lo = label_lo;

	lv_obj_t *bar = lv_bar_create(control, NULL);
	lv_area_t label_hi_coords;
	lv_obj_get_coords(label_hi, &label_hi_coords);
	lv_area_t label_lo_coords;
	lv_obj_get_coords(label_lo, &label_lo_coords);
	// bar width 10%
	lv_coord_t bar_width = w / 10;
	lv_obj_set_size(bar, bar_width,
			label_lo_coords.y1 - label_hi_coords.y2 - 10);
	lv_bar_set_range(bar, min, max);
	lv_bar_set_type(bar, LV_BAR_TYPE_NORMAL);
	lv_obj_align(bar, label_hi, LV_ALIGN_OUT_BOTTOM_MID, 0, HMI_MARGIN);
	lv_obj_align(bar, label_lo, LV_ALIGN_OUT_TOP_MID, 0, -HMI_MARGIN);
	target->bar = bar;
	lv_coord_t bar_x = lv_obj_get_x(bar);

	lv_obj_t *label_max = lv_label_create(control, NULL);
	lv_label_set_text_fmt(label_max, "-%2d", max);
	lv_coord_t max_y = hmi_control_get_y(bar, max);
	lv_obj_set_pos(label_max, bar_x + bar_width + HMI_MARGIN, max_y);

	lv_obj_t *label_min = lv_label_create(control, NULL);
	lv_coord_t min_y = hmi_control_get_y(bar, min);
	lv_obj_set_pos(label_min, bar_x + bar_width + HMI_MARGIN, min_y);
	lv_label_set_text_fmt(label_min, "-%2d", min);

	lv_obj_t *label_pv = lv_label_create(control, NULL);
	lv_coord_t pv_y = hmi_control_get_y(bar, min);
	lv_obj_set_pos(label_pv, bar_x + bar_width + HMI_MARGIN, pv_y);
	lv_label_set_text_fmt(label_pv, "=%2d", min);
	target->label_pv = label_pv;

	lv_obj_t *label_sv = lv_label_create(control, NULL);
	lv_label_set_text_fmt(label_sv, "%2d>", min);
	lv_coord_t label_width = lv_obj_get_width(label_sv);
	lv_coord_t sv_y = hmi_control_get_y(bar, min);
	lv_obj_set_pos(label_sv, bar_x - label_width - HMI_MARGIN, sv_y);
	target->label_sv = label_sv;
}

static const char *hmi_control_mode_map[] = { "Off", "\n", "Manual", "\n",
		"Auto", "" };

static lv_obj_t* hmi_control_create_mode(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y, lv_coord_t w, lv_coord_t h) {

	lv_obj_t *cont = lv_cont_create(parent, NULL);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_set_pos(cont, x, y);
	lv_obj_set_size(cont, w, h);

	lv_obj_t *label = lv_label_create(cont, NULL);
	lv_label_set_text(label, "Control");
	lv_obj_align(label, cont, LV_ALIGN_IN_TOP_MID, 0, HMI_MARGIN);

	lv_obj_t *matrix = lv_btnmatrix_create(cont, NULL);
	lv_obj_clean_style_list(matrix, LV_BTNMATRIX_PART_BG);

	lv_btnmatrix_set_map(matrix, hmi_control_mode_map);
	lv_btnmatrix_set_one_check(matrix, true);
	lv_btnmatrix_set_btn_ctrl(matrix, 0, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 1, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 2, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 0, LV_BTNMATRIX_CTRL_CHECK_STATE);

	// use remaining size
	lv_area_t label_coords;
	lv_obj_get_coords(label, &label_coords);
	lv_area_t cont_coords;
	lv_obj_get_coords(cont, &cont_coords);
	lv_obj_set_size(matrix, cont_coords.x2 - cont_coords.x1 - 10,
			cont_coords.y2 - label_coords.y2 - 10);
	lv_obj_align(matrix, label, LV_ALIGN_OUT_BOTTOM_MID, 0, HMI_MARGIN);
	return matrix;
}

static const char *hmi_control_manual_map[] = { "Light", "\n", "Heater", "\n",
		"Exhaust", "\n", "Recirc.", "" };

static lv_obj_t* hmi_control_create_manual(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y, lv_coord_t w, lv_coord_t h) {

	lv_obj_t *cont = lv_cont_create(parent, NULL);
	lv_obj_set_pos(cont, x, y);
	lv_obj_set_size(cont, w, h);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);

	lv_obj_t *label = lv_label_create(cont, NULL);
	lv_label_set_text(label, "Manual");
	lv_obj_align(label, cont, LV_ALIGN_IN_TOP_MID, 0, HMI_MARGIN);

	lv_obj_t *matrix = lv_btnmatrix_create(cont, NULL);
	lv_obj_clean_style_list(matrix, LV_BTNMATRIX_PART_BG);

	lv_btnmatrix_set_map(matrix, hmi_control_manual_map);
	lv_btnmatrix_set_one_check(matrix, false);
	lv_btnmatrix_set_btn_ctrl(matrix, 0, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 1, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 2, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 3, LV_BTNMATRIX_CTRL_CHECKABLE);

	// use remaining size
	lv_area_t label_coords;
	lv_obj_get_coords(label, &label_coords);
	lv_area_t cont_coords;
	lv_obj_get_coords(cont, &cont_coords);
	lv_obj_set_size(matrix, cont_coords.x2 - cont_coords.x1 - 10,
			cont_coords.y2 - label_coords.y2 - 10);
	lv_obj_align(matrix, label, LV_ALIGN_OUT_BOTTOM_MID, 0, HMI_MARGIN);
	return matrix;
}

lv_obj_t* hmi_control_create_tab(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "Control");

	hmi_control_create_control(&hmi_control_temperature, tab, HMI_MARGIN,
	HMI_MARGIN,
	HMI_CONTROL_W, HMI_CONTROL_H, "Temperature [°C]", 0, 50);
	hmi_control_update(&hmi_control_temperature, 26, 30, false, true);

	hmi_control_create_control(&hmi_control_humidity, tab,
	HMI_MARGIN + HMI_CONTROL_W, HMI_MARGIN,
	HMI_CONTROL_W, HMI_CONTROL_H, "Humidity [%RH]", 0, 100);
	hmi_control_update(&hmi_control_humidity, 46, 40, true, false);

	hmi_control_create_control(&hmi_control_co2, tab,
			(HMI_MARGIN + HMI_CONTROL_W) * 2,
			HMI_MARGIN, HMI_CONTROL_W, HMI_CONTROL_H, "CO2 conc. [ppm]", 0,
			2000);
	hmi_control_update(&hmi_control_co2, 415, 800, false, true);

	hmi_control_mode_btnmatrix = hmi_control_create_mode(tab,
			(HMI_MARGIN + HMI_CONTROL_W) * 3 + 10, HMI_MARGIN, 110, 120);

	hmi_control_manual_btnmatrix = hmi_control_create_manual(tab,
			(HMI_MARGIN + HMI_CONTROL_W) * 3 + 10, 125, 110, 150);

	return tab;
}
