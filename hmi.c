#include "stddef.h"
#include "hmi.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hmi_button_theme_cb(lv_obj_t *button, lv_event_t e);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * tabview;
static lv_obj_t * label_clock;
static lv_obj_t * tab_control;
static lv_obj_t * tab_settings;
static lv_obj_t * tab_about;
static lv_obj_t * led_light;
static lv_obj_t * led_heater;
static lv_obj_t * led_exhaust_fan;
static lv_obj_t * led_recirc_fan;

static void hmi_button_theme_cb(lv_obj_t *button, lv_event_t e) {
	if (e == LV_EVENT_VALUE_CHANGED) {
		uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
		if (lv_btn_get_state(button) == LV_BTN_STATE_CHECKED_RELEASED)
			flag = LV_THEME_MATERIAL_FLAG_DARK;

		LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(),
				lv_theme_get_color_secondary(), flag, lv_theme_get_font_small(),
				lv_theme_get_font_normal(), lv_theme_get_font_subtitle(),
				lv_theme_get_font_title());
	}
}

lv_obj_t * hmi_create_led(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, const char *text) {
	lv_obj_t *led = lv_led_create(parent, NULL);
	lv_obj_set_pos(led, x, y);
	lv_obj_set_style_local_value_str(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT,
			text);
	lv_led_off(led);
	lv_led_set_bright(led, 128);
	return led;
}

lv_obj_t* hmi_create_clock(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
	lv_obj_t *clock = lv_label_create(parent, NULL);
	lv_obj_set_pos(clock, x, y);
	lv_label_set_text(clock, "12:34");
	return clock;
}

lv_obj_t* hmi_create_switch_theme(lv_obj_t *parent) {
	lv_obj_t *button = lv_btn_create(parent, NULL);
	lv_btn_set_checkable(button, true);
	lv_obj_t * label = lv_label_create(button, NULL);
	lv_label_set_text(label, "Dark");

	if (lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK)
		lv_btn_set_state(button, LV_BTN_STATE_CHECKED_RELEASED);

	lv_obj_set_event_cb(button, hmi_button_theme_cb);
	return button;
}

static lv_obj_t* hmi_create_tab_control(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "Control");


    return tab;
}

static lv_obj_t* hmi_create_tab_settings(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "Settings");

	hmi_create_switch_theme(tab);

	return tab;
}


static lv_obj_t* hmi_create_tab_about(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "About");

	lv_obj_t *label = lv_label_create(tab, NULL);
    lv_label_set_text(label, "About content");
	return tab;
}

void hmi_create() {
	tabview = lv_tabview_create(lv_scr_act(), NULL);

	/* confine tabview width to allow for status information */
	lv_obj_set_style_local_pad_left(tabview, LV_TABVIEW_PART_TAB_BG,
			LV_STATE_DEFAULT, LV_DPX(80));
	lv_obj_set_style_local_pad_right(tabview, LV_TABVIEW_PART_TAB_BG,
			LV_STATE_DEFAULT, LV_DPX(200));

	label_clock = hmi_create_clock(tabview, LV_DPX(10), LV_DPX(30));

	led_light = hmi_create_led(tabview, LV_HOR_RES - (LV_DPI / 3) * 4, LV_DPX(10), "L");
	led_heater = hmi_create_led(tabview, LV_HOR_RES - (LV_DPI / 3) * 3, LV_DPX(10), "H");
	led_exhaust_fan = hmi_create_led(tabview, LV_HOR_RES - (LV_DPI / 3) * 2, LV_DPX(10), "E");
	led_recirc_fan = hmi_create_led(tabview, LV_HOR_RES - (LV_DPI / 3), LV_DPX(10), "R");

	tab_control = hmi_create_tab_control(tabview);
	tab_settings = hmi_create_tab_settings(tabview);
	tab_about = hmi_create_tab_about(tabview);
}
