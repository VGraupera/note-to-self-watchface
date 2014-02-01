// Standard includes
#include "pebble.h"

#define DAY_FRAME       (GRect(0, 2, 144, 50))
#define TIME_FRAME      (GRect(0, 18, 144, 168-6))
#define DATE_FRAME      (GRect(0, 66, 144-4, 168-62))

#define TEXT_FRAME      (GRect(4, 90, 144-8, 78))

// App-specific data
Window *window;
TextLayer *time_layer;
TextLayer *date_layer;
TextLayer *day_layer;
TextLayer *text_layer;

GFont font_time;
GFont font_subhead;
GFont font_text;

// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {

  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxxxxxxx XX";
  static char day_text[] = "Xxxxxxxxxxx";

  char *time_format;

  if (units_changed & MINUTE_UNIT) {
    if (clock_is_24h_style()) {
      time_format = "%R";
    } else {
      time_format = "%I:%M";
    }

    strftime(time_text, sizeof(time_text), time_format, tick_time);

    // Kludge to handle lack of non-padded hour format string
    // for twelve hour clock.
    if (!clock_is_24h_style() && (time_text[0] == '0')) {
      memmove(time_text, &time_text[1], sizeof(time_text) - 1);
    }

    text_layer_set_text(time_layer, time_text);
  }

  if (units_changed & DAY_UNIT) {
    strftime(day_text, sizeof(day_text), "%A", tick_time);
    text_layer_set_text(day_layer, day_text);

    strftime(date_text, sizeof(date_text), "%B %e", tick_time);
    text_layer_set_text(date_layer, date_text);
  }

}

// Handle the start-up of the app
static void do_init(void) {

  // Create our app's base window
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  font_subhead = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DROID_SANS_18));
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DROID_SANS_BOLD_48));
  font_text = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DROID_SERIF_18));

  day_layer = text_layer_create(DAY_FRAME);
  text_layer_set_text_color(day_layer, GColorWhite);
  text_layer_set_background_color(day_layer, GColorClear);
  text_layer_set_font(day_layer, font_subhead);
  text_layer_set_text_alignment(day_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));

  time_layer = text_layer_create(TIME_FRAME);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, font_time);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

  date_layer = text_layer_create(DATE_FRAME);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_font(date_layer, font_subhead);
  text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

  text_layer = text_layer_create(TEXT_FRAME);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_font(text_layer, font_text);
  text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeTrailingEllipsis);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

    text_layer_set_text(text_layer, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tincidunt, enim eu blandit sagittis, enim magna adipiscing sapien, in pretium odio enim at mi. Aenean et erat tincidunt, pellentesque quam et, porta orci. Maecenas leo orci, vestibulum et tristique at, vulputate nec purus. Cras viverra lacus ornare dapibus bibendum.");

    // Update the screen right away
  time_t now = time(NULL);
  handle_second_tick(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT );
  // And then every second
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
}

static void do_deinit(void) {
  tick_timer_service_unsubscribe();
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(day_layer);
  window_destroy(window);

  fonts_unload_custom_font(font_time);
  fonts_unload_custom_font(font_subhead);
  fonts_unload_custom_font(font_text);
}

// The main event/run loop for our app
int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}
