// Standard includes
#include "pebble.h"

#define DAY_FRAME       (GRect(0, 2, 144, 50))
#define TIME_FRAME      (GRect(0, 22, 144, 168-6))
#define DATE_FRAME      (GRect(0, 66, 144-4, 168-62))

// App-specific data
Window *window;
TextLayer *time_layer;
TextLayer *date_layer;
TextLayer *day_layer;

// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {

  static char time_text[] = "00:00 AM"; // Needs to be static because it's used by the system later.
  static char date_text[] = "February 12 XX";
  static char day_text[] = "Saturday XX";

  if (units_changed & MINUTE_UNIT) {
    // Update the time - Deal with 12 / 24 format
    clock_copy_time_string(time_text, sizeof(time_text));
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

  day_layer = text_layer_create(DAY_FRAME);
  text_layer_set_text_color(day_layer, GColorWhite);
  text_layer_set_background_color(day_layer, GColorClear);
  text_layer_set_font(day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(day_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));

  time_layer = text_layer_create(TIME_FRAME);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

  date_layer = text_layer_create(DATE_FRAME);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

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
}

// The main event/run loop for our app
int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}
