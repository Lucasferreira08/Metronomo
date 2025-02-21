#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "display.h"
#include "joystick.h"
#include "buzzer.h"
#include "config.h"
// #include "rhythm.h"

ssd1306_t ssd; // Inicializa a estrutura do display

typedef struct {
    uint8_t numerator;
    uint8_t denominator;
} TimeSignature;

TimeSignature time_signatures[] = {{4, 4}, {3, 4}};
const uint8_t NUM_TIME_SIGNATURES = 2;
volatile uint8_t current_time_sig_index = 0;
volatile TimeSignature current_time_signature;

volatile int current_bpm = 60;
volatile int current_beat = 0;
volatile bool update_display = true;

void update_display_func() {
    ssd1306_fill(&ssd, false);
    draw_bpm(&ssd, "BPM %d", current_bpm, 0, 0);
    draw_time(&ssd, "Time %d %d", current_time_signature.numerator, current_time_signature.denominator, 0, 16);
    // ssd1306_draw_string(&ssd, "PLAY", 0, 32);
    ssd1306_send_data(&ssd);
}

int64_t buzzer_stop_alarm_callback(alarm_id_t id, void *user_data) {
    buzzer_stop_tone();
    return 0;
}

int64_t metronome_alarm_callback(alarm_id_t id, void *user_data) {
    int frequency = (current_beat == 0) ? 880 : 440;  // 880 : 440
    buzzer_start_tone(frequency);
    add_alarm_in_ms(50, buzzer_stop_alarm_callback, NULL, false);
    
    current_beat = (current_beat + 1) % current_time_signature.numerator;
    
    uint32_t interval_ms = (60 * 1000) / current_bpm;
    add_alarm_in_ms(interval_ms, metronome_alarm_callback, NULL, false);
    return 0;
}

int main() {
    stdio_init_all();

    display_init(&ssd);
    joystick_init();
    buzzer_init();

    current_time_signature = time_signatures[0];

    add_alarm_in_ms((60 * 1000) / current_bpm, metronome_alarm_callback, NULL, false);

    bool button_debounce = false;
    while (1) {
        int add_bpm = read_bpm();
        if (add_bpm) {
            if (current_bpm>=240 && add_bpm>0)
            {
                current_bpm=30;
                add_bpm--;
            }
            else if (current_bpm<=30 && add_bpm<0) 
            {
                current_bpm=240;
                add_bpm++;
            }
            
            current_bpm += add_bpm;
            update_display = true;
        }

        bool pressed = is_button_pressed();
        if (pressed && !button_debounce) {
            current_time_sig_index = (current_time_sig_index + 1) % NUM_TIME_SIGNATURES;
            current_time_signature = time_signatures[current_time_sig_index];
            current_beat = 0;
            update_display = true;
            button_debounce = true;
        } else if (!pressed) {
            button_debounce = false;
        }

        if (update_display) {
            // ssd1306_fill(&ssd, false);
            // draw_bpm(&ssd, "BPM: ", current_bpm, 0, 0);
            // char time_sig_str[16];
            // sprintf(time_sig_str, "%d/%d", current_time_signature.numerator, current_time_signature.denominator);
            // ssd1306_draw_string(&ssd, time_sig_str, 0, 16);
            // ssd1306_send_data(&ssd);
            update_display_func(current_time_signature);
            // update_display = false;
        }
        
        update_display = true;
        sleep_ms(100);
    }
}