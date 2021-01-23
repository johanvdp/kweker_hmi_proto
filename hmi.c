#include "stddef.h"
#include "hmi.h"

#define HMI_LED_WIDTH (LV_DPI / 4)
#define HMI_LED_HEIGHT (LV_DPI / 4)
#define HMI_TOOLBAR_HEIGHT (LV_DPI / 4)
#define HMI_SETTING_HEIGHT (LV_DPI / 4)
#define HMI_SETTING_WIDTH 60

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

lv_obj_t* hmi_create_led(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
		const char *text) {
	lv_obj_t *led = lv_led_create(parent, NULL);
	lv_obj_set_size(led, HMI_LED_WIDTH, HMI_LED_HEIGHT);
	lv_obj_set_pos(led, x, y);
	lv_obj_set_style_local_value_str(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT,
			text);
	lv_led_off(led);
	lv_led_set_bright(led, 128);
	return led;
}

lv_obj_t* hmi_create_label_clock(lv_obj_t *parent, lv_coord_t x, lv_coord_t y) {
	lv_obj_t *clock = lv_label_create(parent, NULL);
	lv_obj_set_pos(clock, x, y);
	lv_label_set_text(clock, "12:34");
	return clock;
}

lv_obj_t* hmi_create_button_theme(lv_obj_t *parent, lv_coord_t x, lv_coord_t y) {
	lv_obj_t *button = lv_btn_create(parent, NULL);
	lv_obj_set_pos(button, x, y);
	lv_obj_set_size(button, HMI_SETTING_WIDTH, HMI_SETTING_HEIGHT);
	lv_btn_set_checkable(button, true);

	// follow current setting
	if (lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK) {
		lv_btn_set_state(button, LV_BTN_STATE_CHECKED_RELEASED);
	} else {
		lv_btn_set_state(button, LV_BTN_STATE_RELEASED);
	}

	lv_obj_set_event_cb(button, hmi_button_theme_cb);
	return button;
}

static lv_obj_t* hmi_create_tab_control(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "Control");

	lv_obj_t *label = lv_label_create(tab, NULL);
	lv_label_set_text(label, "placeholder");

	return tab;
}

static void hmi_spinbox_increment_event_cb(lv_obj_t *btn, lv_event_t e) {
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_obj_t *spinbox = (lv_obj_t*) btn->user_data;
		lv_spinbox_increment(spinbox);
	}
}

static void hmi_spinbox_decrement_event_cb(lv_obj_t *btn, lv_event_t e) {
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_obj_t *spinbox = (lv_obj_t*) btn->user_data;
		lv_spinbox_decrement(spinbox);
	}
}

static lv_obj_t* hmi_create_spinbox(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y, lv_coord_t w, uint8_t digit_count,
		uint8_t separator_position, int32_t range_min, int32_t range_max,
		uint32_t step) {
	lv_obj_t *spinbox = lv_spinbox_create(parent, NULL);
	lv_obj_set_pos(spinbox, x, y);
	lv_obj_set_width(spinbox, w);
	lv_textarea_set_text_align(spinbox, LV_LABEL_ALIGN_RIGHT);
	// avoid see of blinking cursors
	lv_textarea_set_cursor_blink_time(spinbox, 0);
	lv_spinbox_set_digit_format(spinbox, digit_count, separator_position);
	lv_spinbox_set_range(spinbox, range_min, range_max);
	lv_spinbox_set_step(spinbox, step);
	lv_spinbox_set_rollover(spinbox, true);

	lv_coord_t height = lv_obj_get_height(spinbox);
	lv_obj_t *btn_plus = lv_btn_create(parent, NULL);
	btn_plus->user_data = spinbox;
	lv_obj_set_size(btn_plus, height, height);
	lv_obj_align(btn_plus, spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn_plus, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_plus, LV_BTN_PART_MAIN,
			LV_STATE_DEFAULT,
			LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn_plus, hmi_spinbox_increment_event_cb);

	lv_obj_t *btn_min = lv_btn_create(parent, btn_plus);
	btn_min->user_data = spinbox;
	lv_obj_align(btn_min, spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn_min, hmi_spinbox_decrement_event_cb);
	lv_obj_set_style_local_value_str(btn_min, LV_BTN_PART_MAIN,
			LV_STATE_DEFAULT,
			LV_SYMBOL_MINUS);
	return spinbox;
}

static lv_obj_t* hmi_create_spinbox_hour(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y) {
	return hmi_create_spinbox(parent, x, y, HMI_SETTING_WIDTH, 2, 0, 0, 23, 1);
}

static lv_obj_t* hmi_create_spinbox_minute(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y) {
	return hmi_create_spinbox(parent, x, y, HMI_SETTING_WIDTH, 2, 0, 0, 59, 1);
}

static lv_obj_t* hmi_create_spinbox_temperature(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y) {
	return hmi_create_spinbox(parent, x, y, HMI_SETTING_WIDTH, 3, 2, 0, 500, 1);
}

static lv_obj_t* hmi_create_spinbox_humidity(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y) {
	return hmi_create_spinbox(parent, x, y, HMI_SETTING_WIDTH, 3, 2, 0, 999, 1);
}

static lv_obj_t* hmi_create_spinbox_co2(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y) {
	return hmi_create_spinbox(parent, x, y, HMI_SETTING_WIDTH, 4, 0, 0, 5000, 1);
}

static lv_obj_t* hmi_create_label(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
		const char *text) {
	lv_obj_t *label = lv_label_create(parent, NULL);
	lv_obj_set_pos(label, x, y + 10);
	lv_label_set_text(label, text);
	return label;
}

static lv_obj_t* hmi_create_tab_settings(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "Settings");

	/** set current time */
	hmi_create_label(tab, 10, 0 * HMI_SETTING_HEIGHT, "current time");
	hmi_spinbox_time_hour = hmi_create_spinbox_hour(tab, 200,
			0 * HMI_SETTING_HEIGHT);
	hmi_spinbox_time_minute = hmi_create_spinbox_minute(tab, 360,
			0 * HMI_SETTING_HEIGHT);
	hmi_create_label(tab, 310, 0 * HMI_SETTING_HEIGHT, ":");

	/** set begin of day time */
	hmi_create_label(tab, 10, 1 * HMI_SETTING_HEIGHT, "begin of day time");
	hmi_spinbox_begin_of_day_hour = hmi_create_spinbox_hour(tab, 200,
			1 * HMI_SETTING_HEIGHT);
	hmi_spinbox_begin_of_day_minute = hmi_create_spinbox_minute(tab, 360,
			1 * HMI_SETTING_HEIGHT);
	hmi_create_label(tab, 310, 1 * HMI_SETTING_HEIGHT, ":");

	/** set begin of night time */
	hmi_create_label(tab, 10, 2 * HMI_SETTING_HEIGHT, "begin of night time");
	hmi_spinbox_begin_of_night_hour = hmi_create_spinbox_hour(tab, 200,
			2 * HMI_SETTING_HEIGHT);
	hmi_spinbox_begin_of_night_minute = hmi_create_spinbox_minute(tab, 360,
			2 * HMI_SETTING_HEIGHT);
	hmi_create_label(tab, 310, 2 * HMI_SETTING_HEIGHT, ":");

	/** set day temperature */
	hmi_create_label(tab, 10, 3 * HMI_SETTING_HEIGHT, "day temperature");
	hmi_spinbox_day_temperature = hmi_create_spinbox_temperature(tab, 360,
			3 * HMI_SETTING_HEIGHT);
	/** set day humidity */
	hmi_create_label(tab, 10, 4 * HMI_SETTING_HEIGHT, "day humidity");
	hmi_spinbox_day_humidity = hmi_create_spinbox_humidity(tab, 360,
			4 * HMI_SETTING_HEIGHT);
	/** set day CO2 concentration */
	hmi_create_label(tab, 10, 5 * HMI_SETTING_HEIGHT, "day co2 concentration");
	hmi_spinbox_day_co2 = hmi_create_spinbox_co2(tab, 360,
			5 * HMI_SETTING_HEIGHT);
	/** set night temperature */
	hmi_create_label(tab, 10, 6 * HMI_SETTING_HEIGHT, "night temperature");
	hmi_spinbox_night_temperature = hmi_create_spinbox_temperature(tab, 360,
			6 * HMI_SETTING_HEIGHT);
	/** set night humidity */
	hmi_create_label(tab, 10, 7 * HMI_SETTING_HEIGHT, "night humidity");
	hmi_spinbox_night_humidity = hmi_create_spinbox_humidity(tab, 360,
			7 * HMI_SETTING_HEIGHT);
	/** set night CO2 concentration */
	hmi_create_label(tab, 10, 8 * HMI_SETTING_HEIGHT,
			"night co2 concentration");
	hmi_spinbox_night_co2 = hmi_create_spinbox_co2(tab, 360,
			8 * HMI_SETTING_HEIGHT);

	hmi_create_label(tab, 10, 9 * HMI_SETTING_HEIGHT, "color theme");
	hmi_create_button_theme(tab, 360, 9 * HMI_SETTING_HEIGHT);

	return tab;
}

static lv_obj_t* hmi_create_tab_about(lv_obj_t *parent) {

	lv_obj_t *tab = lv_tabview_add_tab(parent, "About");

	hmi_label_about = lv_label_create(tab, NULL);
	lv_label_set_long_mode(hmi_label_about, LV_LABEL_LONG_BREAK);
	lv_obj_set_size(hmi_label_about, lv_page_get_scrl_width(tab) - 20,
			lv_obj_get_height(tab));
	lv_label_set_text(hmi_label_about,
			"PRODUCT DESCRIPTION\n"
					"===================\n"
					"Contrary to popular belief, Lorem Ipsum is not simply random text. "
					"It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. "
					"Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. "
					"Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. "
					"This book is a treatise on the theory of ethics, very popular during the Renaissance. "
					"The first line of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in section 1.10.32. "
					"It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. "
					"The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. "
					"Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. "
					"\n"
					"\n"
					"3RD PARTY LICENSES\n"
					"==================\n"
					"Contrary to popular belief, Lorem Ipsum is not simply random text. "
					"It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. "
					"Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. "
					"Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. "
					"This book is a treatise on the theory of ethics, very popular during the Renaissance. "
					"The first line of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in section 1.10.32. "
					"It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. "
					"The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. "
					"Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. "
					"Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).");
	return tab;
}

static lv_obj_t* hmi_create_toolbar(lv_obj_t *parent) {

	lv_obj_t *toolbar = lv_obj_create(parent, NULL);
	lv_obj_clean_style_list(toolbar, LV_OBJ_PART_MAIN);
	lv_obj_set_size(toolbar, LV_HOR_RES, HMI_TOOLBAR_HEIGHT);
	hmi_label_clock = hmi_create_label_clock(toolbar, LV_DPX(10), LV_DPX(10));
	hmi_led_light_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH * 4, 0, "L");
	hmi_led_heater_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH * 3, 0, "H");
	hmi_led_exhaust_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH * 2, 0, "E");
	hmi_led_recirculation_switch = hmi_create_led(toolbar,
	LV_HOR_RES - HMI_LED_WIDTH, 0, "R");

	return toolbar;
}

static lv_obj_t* hmi_create_tabview(lv_obj_t *parent) {
	lv_obj_t *tabview = lv_tabview_create(parent, NULL);
	lv_obj_set_style_local_pad_all(tabview, LV_TABVIEW_PART_TAB_BG,
			LV_STATE_DEFAULT, 0);
	lv_obj_set_style_local_pad_top(tabview, LV_TABVIEW_PART_TAB_BTN,
			LV_STATE_DEFAULT, 0);
	lv_obj_set_pos(tabview, 0, HMI_TOOLBAR_HEIGHT);
	lv_obj_set_size(tabview, LV_HOR_RES, LV_VER_RES - HMI_TOOLBAR_HEIGHT);
	return tabview;
}

void hmi_initialize() {
	lv_obj_t *screen = lv_obj_create(NULL, NULL);
	lv_scr_load(screen);

	hmi_create_toolbar(screen);
	lv_obj_t *tabview = hmi_create_tabview(screen);

	hmi_create_tab_control(tabview);
	hmi_create_tab_settings(tabview);
	hmi_create_tab_about(tabview);
}
