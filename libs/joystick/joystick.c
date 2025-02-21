#include "joystick.h"

void joystick_init() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
}

int read_bpm() {
    adc_select_input(1);
    uint16_t adc_value = adc_read();
    printf("%d\n", adc_value);
    if (adc_value>(2047+500)) 
    {
        return 1;
    }
    else if (adc_value<(2047-500))
    {
        return -1;
    }
    return 0;
    // return (adc_value * 210 / 4095) + 30;
}

bool is_button_pressed() {
    return !gpio_get(JOYSTICK_BUTTON_PIN);
}