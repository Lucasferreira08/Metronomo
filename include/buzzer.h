#ifndef BUZZER_H
#define BUZZER_H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN 21  // GPIO conectado ao buzzer

void buzzer_init();

void buzzer_start_tone(int frequency);

void buzzer_stop_tone();

#endif 