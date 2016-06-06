#include <stdio.h>

#include "allegro5/allegro.h"
#include "allegro5/allegro_color.h"

#define SCREEN_SIZE 1000

extern void assembler_mandelbrot_function(unsigned* pixels_array, unsigned int mandel_size, float mandel_x, float mandel_y, float mandel_step);

int al_initialization()
{
    if(!al_init())
    {
        printf("Allegro initialization error.\n");
        return 0;
    }

    if(!al_install_keyboard())
    {
        printf("Keyboard initialization error.\n");
        return 0;
    }

    if(!al_install_mouse())
    {
        printf("Keyboard initialization error.\n");
        return 0;
    }

    return 1;
}

int al_destroy(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* bitmap, ALLEGRO_DISPLAY* display)
{
    al_destroy_event_queue(queue);
    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
}

void draw(ALLEGRO_BITMAP* bitmap, float mandel_x, float mandel_y, float mandel_step)
{
    // Creates bitmap region to modify 
    ALLEGRO_LOCKED_REGION *region = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);

    // Array of pixels: 32bits/4bytes for pixel
    unsigned* pixels_array;
    pixels_array = (unsigned*) region->data;
    // HACK: Finds indicator for the first pixel of the bitmap (bottom left corner)
    pixels_array += SCREEN_SIZE;
    pixels_array -= SCREEN_SIZE * SCREEN_SIZE;

	// Assembler function
    assembler_mandelbrot_function(pixels_array, SCREEN_SIZE, mandel_x, mandel_y, mandel_step);

    // Saves changes and display modified bitmap
    al_unlock_bitmap(bitmap);
  	al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_flip_display();
}

int main(int argc, char *argv[])
{
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_BITMAP* bitmap;

    if(!al_initialization())
        return 1;

    // Creates window to display application
    display = al_create_display(SCREEN_SIZE, SCREEN_SIZE);
    if(!display)
    {
        printf("Display initialization error.\n");
        return 1;
    }

    // Creates queue for application key events
    queue = al_create_event_queue();
    if(!queue)
    {
        printf("Event queue initialization error.\n");
        return 1;
    }
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    // Creates bitmap to display in window application
    bitmap = al_create_bitmap(SCREEN_SIZE, SCREEN_SIZE);
    if(!bitmap)
    {
        printf("Bitmap load error.\n");
        return 1;
    }
    al_set_target_bitmap(al_get_backbuffer(display));

	// Draw assembler_mandelbrot_function with original coords
    float mandel_x = -2.0, mandel_y = -1.5, mandel_step = 3.0/SCREEN_SIZE;
    draw(bitmap, mandel_x, mandel_y, mandel_step);

    while(true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
		
		// Map escape key for ending application
        if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            al_destroy(queue, bitmap, display);
            return 0;
        }

		// Map mouse click for scaling mandelbrot
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			mandel_x += event.mouse.x * mandel_step;
			mandel_y += mandel_step * SCREEN_SIZE;
			mandel_y -= event.mouse.y * mandel_step;	

			mandel_step /= 2.0; // it's scale

			mandel_x -= mandel_step * SCREEN_SIZE / 2;
			mandel_y -= mandel_step * SCREEN_SIZE / 2;

			draw(bitmap, mandel_x, mandel_y, mandel_step);
		}
    }
}
