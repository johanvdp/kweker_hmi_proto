#include "hmi.h"
#include "hmi_control.h"
#include "hmi_settings.h"
#include "hmi_about.h"

#define HMI_LED_WIDTH (LV_DPI / 6)
#define HMI_LED_HEIGHT (LV_DPI / 6)
#define HMI_TOOLBAR_HEIGHT (LV_DPI / 4)

lv_obj_t* hmi_create_led(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
		const char *text) {
	lv_obj_t *led = lv_led_create(parent, NULL);
	lv_obj_set_size(led, HMI_LED_WIDTH, HMI_LED_HEIGHT);
	lv_obj_set_pos(led, x, y);
	lv_obj_set_style_local_value_str(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT,
			text);
	lv_led_on(led);
	return led;
}

lv_obj_t* hmi_create_label(lv_obj_t *parent, uint16_t x, uint16_t y,
		const char *text) {
	lv_obj_t *label = lv_label_create(parent, NULL);
	lv_label_set_text(label, text);
	lv_obj_set_pos(label, x, y);
	return label;
}

lv_obj_t* hmi_create_toolbar(lv_obj_t *parent) {
	lv_obj_t *toolbar = lv_obj_create(parent, NULL);
	lv_obj_clean_style_list(toolbar, LV_OBJ_PART_MAIN);
	lv_obj_set_size(toolbar, LV_HOR_RES, HMI_TOOLBAR_HEIGHT);

	hmi_label_clock = hmi_create_label(toolbar, LV_DPX(10), LV_DPX(10),
			"12:34");

	hmi_create_label(toolbar, LV_DPX(60), LV_DPX(2), "circadian:");
	hmi_label_circadian = hmi_create_label(toolbar, LV_DPX(140), LV_DPX(2),
			"DAY");

	hmi_create_label(toolbar, LV_DPX(60), LV_DPX(22), "control:");
	hmi_label_control_mode = hmi_create_label(toolbar, LV_DPX(140), LV_DPX(22),
			"OFF");

	hmi_led_light_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH * 4 - LV_DPX(5) * 4, LV_DPX(5), "L");

	hmi_led_heater_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH * 3 - LV_DPX(5) * 3, LV_DPX(5), "H");

	hmi_led_exhaust_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH * 2 - LV_DPX(5) * 2, LV_DPX(5), "E");

	hmi_led_recirculation_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH - LV_DPX(5), LV_DPX(5), "R");

	return toolbar;
}

lv_obj_t* hmi_create_tabview(lv_obj_t *parent, uint16_t toolbar_height) {
	lv_obj_t *tabview = lv_tabview_create(parent, NULL);

	// remove excessive whitespace
    lv_obj_set_style_local_pad_top(tabview, LV_TABVIEW_PART_TAB_BG,
			LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(tabview, LV_TABVIEW_PART_TAB_BG,
			LV_STATE_DEFAULT, 0);

	lv_obj_set_style_local_pad_top(tabview, LV_TABVIEW_PART_TAB_BTN,
			LV_STATE_DEFAULT, 0);
	lv_obj_set_pos(tabview, 0, toolbar_height);
	uint16_t parent_height = lv_obj_get_height(parent);
	uint16_t parent_width = lv_obj_get_width(parent);
	lv_obj_set_size(tabview, parent_width, parent_height - toolbar_height);
	return tabview;
}

void hmi_initialize() {
	lv_obj_t *screen = lv_obj_create(NULL, NULL);
	lv_scr_load(screen);

	lv_obj_t *toolbar = hmi_create_toolbar(screen);
	uint16_t toolbar_height = lv_obj_get_height(toolbar);
	lv_obj_t *tabview = hmi_create_tabview(screen, toolbar_height);

	hmi_control_create_tab(tabview);
	hmi_settings_create_tab(tabview);
	hmi_about_create_tab(tabview);
}
