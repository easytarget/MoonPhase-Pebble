#include <pebble.h>

static Window *s_window;
static BitmapLayer *s_moon_layer;
static GBitmap *s_moon_bitmap;
//static TextLayer *s_text_layer;
static TextLayer *s_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Graphics layer for the background
  s_moon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MOON_FULL);
  s_moon_layer = bitmap_layer_create(GRect(0, 0, 144, 144));
  bitmap_layer_set_bitmap(s_moon_layer, s_moon_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_moon_layer));
  
  // Two text layers, one for static text, one for time
	//s_text_layer = text_layer_create(GRect(25, 10, 100,100));
	s_time_layer = text_layer_create(GRect(0, 134, 144, 34));
	
	// Set the color, text, font, and text alignment
  //text_layer_set_text_color(s_text_layer, GColorBlack);
  //text_layer_set_background_color(s_text_layer, GColorWhite);
	//text_layer_set_text(s_text_layer, "Greetings\nFrom\nOwen! :-)");
	//text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	//text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);
  
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_background_color(s_time_layer, GColorBlack);
	text_layer_set_text(s_time_layer, "00:00");
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	
	// Add the text layers to the main window
	//layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_time_layer));
  
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded the window");
}

static void main_window_unload(Window *window) {
	// Destroy the text layers
	//text_layer_destroy(s_text_layer);
	text_layer_destroy(s_time_layer);
  // Destroy GBitmap+Layer
  gbitmap_destroy(s_moon_bitmap);
  bitmap_layer_destroy(s_moon_layer);
  
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloaded the window");
}

static void init(void) {
	// Create a window, black background.
	s_window = window_create();
  window_set_background_color (s_window, GColorBlack);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
	// Push the window with animated=true
	window_stack_push(s_window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushed the window");
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Make sure the time is displayed from the start
  update_time();
  
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Registered Tick Service");
}

static void deinit(void) {
	// Destroy the window
	window_destroy(s_window);
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Destroyed the window");
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
