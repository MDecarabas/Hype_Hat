// LED ANIMATE

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "app_timer.h"

#include "microbit_v2.h"
#include "led_matrix.h"
#include "led_text.h"
#include "font.h"


// Timer definition
APP_TIMER_DEF(my_timer);

// Buffer to store scrolling text to display
// Text's maximum length is 100 characters, plus 1 trailing empty buffer
static uint8_t texts[3][101][8] = {0};

// Track length of scrolling text
static uint8_t lengths[3] = {0};

// Current display window
static int display_column_start;
static int display_text_num;
static unsigned char display_text_red;
static unsigned char display_text_green;
static unsigned char display_text_blue;
static float brightness_text;


void set_text(int text_num, char* text_string, uint8_t text_length) {
    
    // Set text length
    lengths[text_num] = text_length;

    // Store text in texts buffer
    for (int i = 0; i < text_length; i++) {
        for (int j = 0; j < 8; j++) {
            texts[text_num][i][j] = font[(int)text_string[i]][j];
        }
    }

    // // (FOR DEBUGGING) Print out what the text looks like in the buffer
    // for (int i = 0; i < 30; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         printf("%x ", texts[text_num][i][j]);
    //     }
    //     printf("\n");
    // }

}


void play_text(int text_num, unsigned char r, unsigned char g, unsigned char b, float freq) {
    
    // Display parameters
    display_column_start = -1;
    display_text_num = text_num;
    display_text_red = r;
    display_text_green = g;
    display_text_blue = b;
    brightness_text = 1;
    
    // Initialize and create timer
    app_timer_init();
    app_timer_create(&my_timer, APP_TIMER_MODE_REPEATED, next_frame_text);

    // Start app timer (move frame every 0.2 seconds)
    app_timer_start(my_timer, 32786 * freq, NULL);

}


void stop_text() {

    // Stop timer
    app_timer_stop(my_timer);

    // Clear LED matrix
    clear_matrix();

}


void next_frame_text() {

    // Increment column number 
    display_column_start = (display_column_start + 1) % ((lengths[display_text_num] + 1) * 8);
    
    // Set LED matrix based on string message
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 32; j++) {
            int display_column_curr = (display_column_start + j) % ((lengths[display_text_num] + 1) * 8);
            int display_letter_index = display_column_curr / 8;
            int display_letter_offset = display_column_curr % 8;
            uint8_t display_letter_row = texts[display_text_num][display_letter_index][i];
            bool isOff = !((1 << (display_letter_offset)) & display_letter_row);
            if (isOff) {
                set_data(get_led_num(i+1, j+1), 0, 0, 0);
            } else {
                set_data(get_led_num(i+1, j+1), 
                         display_text_red * brightness_text, 
                         display_text_green * brightness_text, 
                         display_text_blue * brightness_text);
            }
        }
    }

    // Print what the LED matrix should like 
    // debug_matrix();
    
}


void set_brightness_text(float brightness) {
    if (brightness <= 1) {
        brightness_text = brightness;
    }
}
