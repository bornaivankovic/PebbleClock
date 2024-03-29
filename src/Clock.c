#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time()
{
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    // Create a long-lived buffer
    static char time_buffer[] = "HH:MM AM";
    static char date_buffer[] = "DDDDDDDDD\ndd.mm.yyyy.";

    strftime(date_buffer,sizeof("DDDDDDDDD\ndd.mm.yyyy."),"%A\n%d.%m.%Y.",tick_time);

    //Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true)
    {
        // Use 24 hour format
        strftime(time_buffer,sizeof("HH:MM AM"),"%H:%M",tick_time);
		text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
    }
    else
    {
        // Use 12 hour format
        strftime(time_buffer,sizeof("HH:MM AM"),"%I:%M %p",tick_time);
		text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, time_buffer);
    text_layer_set_text(s_date_layer, date_buffer);
}

static void main_window_load(Window *window)
{
    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
    s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

    // Create time TextLayer
    s_date_layer = text_layer_create(GRect(0, 0, 144, 50));
    text_layer_set_background_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorClear);
    text_layer_set_text(s_date_layer, "Monday,\n01.01.2000.");
    text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

    // Create date TextLayer
    s_time_layer = text_layer_create(GRect(0, 60, 144, 50));
    text_layer_set_background_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_time_layer, GColorClear);
    text_layer_set_text(s_time_layer, "00:00 AM");
	if(clock_is_24h_style()==true){
		text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	}
	else{
    	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	}
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window)
{
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    // Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);

    // Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    update_time();
}
static void init()
{
    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers)
    {
        .load = main_window_load,
         .unload = main_window_unload
    });


    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    update_time();

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

}
static void deinit()
{
    window_destroy(s_main_window);
}


int main()
{
    init();
    app_event_loop();
    deinit();
}
