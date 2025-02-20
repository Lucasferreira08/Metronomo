#ifndef JOYSTICK_H
#define JOYSTICK_H

#define JOYSTICK_X_PIN 27
#define JOYSTICK_BUTTON_PIN 22

#include "hardware/adc.h"
#include <stdio.h>

void joystick_init();

int read_bpm();

bool is_button_pressed();

#endif 