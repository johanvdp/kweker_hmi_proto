#include "hmi.h"
#include "hmi_control.h"
#include "hmi_settings.h"
#include "hmi_about.h"

#define HMI_WIDTH 480
#define HMI_HEIGHT 320
#define HMI_LED_WIDTH 20
#define HMI_LED_HEIGHT 20
#define HMI_TOOLBAR_HEIGHT 32
#define HMI_BUTTON_WIDTH 65
#define HMI_BUTTON_HEIGHT 26
#define HMI_MARGIN 5

/** current time */
lv_obj_t *hmi_label_clock;
/** current circadian [day, night] */
lv_obj_t *hmi_label_circadian;
/** current control mode [off, manual, automatic] */
lv_obj_t *hmi_label_control_mode;
/** current light switch state [off, on] */
lv_obj_t *hmi_led_light_switch;
/** current heater switch state [off, on] */
lv_obj_t *hmi_led_heater_switch;
/** current exhaust fan switch state [off, on] */
lv_obj_t *hmi_led_exhaust_switch;
/** current recirculation fan switch state [off, on] */
lv_obj_t *hmi_led_recirculation_switch;

/** tab page holder */
static lv_obj_t *hmi_tabview;
/** navigation buttons */
static lv_obj_t *hmi_btnmatrix_tabview;

static lv_obj_t* hmi_create_led(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
		const char *text) {

	lv_obj_t *led = lv_led_create(parent, NULL);
	lv_obj_set_pos(led, x, y);
	lv_obj_set_size(led, HMI_LED_WIDTH, HMI_LED_HEIGHT);
	lv_obj_set_style_local_value_str(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT,
			text);
	lv_led_off(led);

	return led;
}

static lv_obj_t* hmi_create_label(lv_obj_t *parent, uint16_t x, uint16_t y,
		const char *text) {

	lv_obj_t *label = lv_label_create(parent, NULL);
	lv_obj_set_pos(label, x, y);
	lv_label_set_text(label, text);

	return label;
}

static void hmi_navigation_event_cb(lv_obj_t *button, lv_event_t e) {

	if (e == LV_EVENT_CLICKED) {
		/** navigation button clicked */
		uint16_t id = lv_btnmatrix_get_active_btn(hmi_btnmatrix_tabview);
		if (id >= 0 && id <= 2) {
			/** select corresponding tab */
			lv_tabview_set_tab_act(hmi_tabview, id, LV_ANIM_ON);
		}
	}
}

static const char *hmi_navigation_map[] = { "Control", "Settings", "About", "" };

static lv_obj_t* hmi_create_navigation_btnmatrix(lv_obj_t *parent, lv_coord_t x,
		lv_coord_t y, lv_coord_t w, lv_coord_t h) {

	lv_obj_t *matrix = lv_btnmatrix_create(parent, NULL);
	// intentionally chop of lower edge of buttons
	lv_obj_set_pos(matrix, x, y + HMI_MARGIN);
	lv_obj_set_size(matrix, w, h);
	lv_obj_clean_style_list(matrix, LV_BTNMATRIX_PART_BG);

	lv_btnmatrix_set_map(matrix, hmi_navigation_map);
	lv_btnmatrix_set_one_check(matrix, true);
	lv_btnmatrix_set_btn_ctrl(matrix, 0, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 1, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 2, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_btnmatrix_set_btn_ctrl(matrix, 0, LV_BTNMATRIX_CTRL_CHECK_STATE);

	lv_obj_set_event_cb(matrix, hmi_navigation_event_cb);

	return matrix;
}

static lv_obj_t* hmi_create_toolbar(lv_obj_t *parent) {

	lv_obj_t *toolbar = lv_obj_create(parent, NULL);
	lv_obj_clean_style_list(toolbar, LV_OBJ_PART_MAIN);
	uint16_t parent_width = lv_obj_get_width(parent);
	lv_obj_set_size(toolbar, parent_width, HMI_TOOLBAR_HEIGHT);

	hmi_label_clock = hmi_create_label(toolbar, 8, 8,
			"12:34");
	hmi_label_circadian = hmi_create_label(toolbar, 48, 2,
			"DAY");
	hmi_label_control_mode = hmi_create_label(toolbar, 48, 16,
			"OFF");

	hmi_btnmatrix_tabview = hmi_create_navigation_btnmatrix(toolbar,
			100, 0, 250, HMI_TOOLBAR_HEIGHT);

	hmi_led_light_switch = hmi_create_led(toolbar,
			parent_width - (HMI_LED_WIDTH + HMI_MARGIN) * 4, HMI_MARGIN, "L");

	hmi_led_heater_switch = hmi_create_led(toolbar,
			parent_width - (HMI_LED_WIDTH + HMI_MARGIN) * 3, HMI_MARGIN, "H");

	hmi_led_exhaust_switch = hmi_create_led(toolbar,
			parent_width - (HMI_LED_WIDTH + HMI_MARGIN) * 2, HMI_MARGIN, "E");

	hmi_led_recirculation_switch = hmi_create_led(toolbar,
			parent_width - (HMI_LED_WIDTH + HMI_MARGIN), HMI_MARGIN, "R");

	return toolbar;
}

static void hmi_tabview_event_cb(lv_obj_t *button, lv_event_t e) {

	if (e == LV_EVENT_VALUE_CHANGED) {
		/** tabview slide to other tab */
		uint16_t id = lv_tabview_get_tab_act(hmi_tabview);
		if (id >= 0 && id <= 2) {
			/** synchronize selected navigation button */
			lv_btnmatrix_set_btn_ctrl(hmi_btnmatrix_tabview, id, LV_BTNMATRIX_CTRL_CHECK_STATE);
		}
	}
}

static lv_obj_t* hmi_create_tabview(lv_obj_t *parent) {

	lv_obj_t *tabview = lv_tabview_create(parent, NULL);
	lv_obj_set_pos(tabview, 0, HMI_TOOLBAR_HEIGHT);
	uint16_t parent_height = lv_obj_get_height(parent);
	uint16_t parent_width = lv_obj_get_width(parent);
	lv_obj_set_size(tabview, parent_width, parent_height - HMI_TOOLBAR_HEIGHT);
	lv_tabview_set_btns_pos(tabview, LV_TABVIEW_TAB_POS_NONE);
	lv_obj_set_event_cb(tabview, hmi_tabview_event_cb);
	return tabview;
}

void hmi_initialize() {

	lv_obj_t *screen = lv_obj_create(NULL, NULL);
	lv_scr_load(screen);

	hmi_create_toolbar(screen);
	hmi_tabview = hmi_create_tabview(screen);

	hmi_control_create_tab(hmi_tabview);
	hmi_settings_create_tab(hmi_tabview);
	hmi_about_create_tab(hmi_tabview);
}
