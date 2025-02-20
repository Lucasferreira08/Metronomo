#include "buzzer.h"

static uint pwm_slice_num;

void buzzer_init() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    pwm_slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_init(pwm_slice_num, &config, true);
    pwm_set_enabled(pwm_slice_num, false);
}

void buzzer_start_tone(int frequency) {
    if (frequency <= 0) {
        pwm_set_enabled(pwm_slice_num, false);
        return;
    }
    uint32_t sys_clk = 125000000;
    uint32_t div = sys_clk / (frequency * 512);
    pwm_set_clkdiv_mode(pwm_slice_num, div);
    uint32_t wrap = (sys_clk / (div * frequency)) - 1;
    pwm_set_wrap(pwm_slice_num, wrap);
    pwm_set_chan_level(pwm_slice_num, PWM_CHAN_B, wrap / 2);
    pwm_set_enabled(pwm_slice_num, true);
}

void buzzer_stop_tone() {
    pwm_set_enabled(pwm_slice_num, false);
}