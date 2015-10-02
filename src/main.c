#include <pebble.h>
#include <stdlib.h>
#include <time.h>

Window *my_window;
static Layer *canvas_layer;
static TextLayer *time_layer;
static int w = 144;
static int h = 168;
GColor8 colorArray[64];

#define CHANNEL_MAX 3
#define R     0
#define G     1
#define B     2
#define ALPHA CHANNEL_MAX
  


static void handpicked_update_proc(Layer *layer, GContext *ctx) { 

  // pick best colors, show only those
  int indexArray[28] = {4, 7, 12, 20, 22, 25, 27, 32, 34, 36, 37, 38, 41, 43, 45, 48, 49, 50, 52, 53, 54, 55, 57, 58, 60, 61, 62, 64};
  
  // random bg color
  srand(time(0));
  graphics_context_set_fill_color(ctx, colorArray[indexArray[rand()%28]]);
  graphics_fill_rect(ctx, GRect(0, 0, w, h), 0, GCornerNone);
  
  // time & format
  time_t now = time(0);
  struct tm *tick_time = localtime(&now);
  static char buffer[] = "00:00:00";
  strftime(buffer, sizeof("00:00:00"), "%H %M %S", tick_time);
  
  // print time
  text_layer_set_text(time_layer, buffer);
}

// update layer on tick
static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(canvas_layer);
}
  
void handle_init(void) {
  // init window
  my_window = window_create();
  Layer *window_layer = window_get_root_layer(my_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  GColor8 colorArrayS[64] = {GColorMintGreen, GColorMelon, GColorShockingPink, GColorFolly, GColorSunsetOrange, GColorArmyGreen, GColorDukeBlue, GColorTiffanyBlue, GColorScreaminGreen, GColorPastelYellow, GColorRichBrilliantLavender, GColorBrightGreen, GColorBrilliantRose, GColorCadetBlue, GColorRoseVale, GColorFashionMagenta, GColorJaegerGreen, GColorBabyBlueEyes, GColorPurpureus, GColorChromeYellow, GColorDarkGreen, GColorRed, GColorLiberty, GColorLightGray, GColorVividViolet, GColorRajah, GColorIndigo, GColorMayGreen, GColorIcterine, GColorBulgarianRose, GColorOrange, GColorGreen, GColorWindsorTan, GColorLavenderIndigo, GColorDarkGray, GColorElectricBlue, GColorBlueMoon, GColorCyan, GColorBlack, GColorMediumAquamarine, GColorDarkCandyAppleRed, GColorLimerick, GColorCobaltBlue, GColorCeleste, GColorElectricUltramarine, GColorPictonBlue, GColorInchworm, GColorBlue, GColorVividCerulean, GColorPurple, GColorKellyGreen, GColorMalachite, GColorMidnightGreen, GColorYellow, GColorMagenta, GColorSpringBud, GColorJazzberryJam, GColorVeryLightBlue, GColorWhite, GColorIslamicGreen, GColorOxfordBlue, GColorImperialPurple, GColorBrass, GColorMediumSpringGreen};
  
  for (int i=0; i<64; i++) {
    colorArray[i] = colorArrayS[i];
  }
  
  // init canvas layer
  canvas_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_set_update_proc(canvas_layer, handpicked_update_proc);
  layer_add_child(window_layer, canvas_layer);
  
  // init text layer
  GFont custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PROBA_36));
  time_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_text(time_layer, "00:00:00");

  text_layer_set_font(time_layer, custom_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(time_layer));

  // init timer
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  
  layer_mark_dirty(canvas_layer);
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
