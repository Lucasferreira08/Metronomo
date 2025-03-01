#include "buzzer.h"

static uint pwm_slice_num;
static uint pwm_channel;

void buzzer_init() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    pwm_slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_channel = pwm_gpio_to_channel(BUZZER_PIN);
    
    pwm_config config = pwm_get_default_config();
    pwm_init(pwm_slice_num, &config, false); // Inicia desligado
}

void buzzer_start_tone(int frequency) {
    if (frequency <= 0) {
        buzzer_stop_tone();
        return;
    }

    uint32_t sys_clk = clock_get_hz(clk_sys); // Obtém clock do sistema (125 MHz padrão)
    uint32_t div = 1;
    uint32_t wrap;

    // Calcula div e wrap para melhor resolução
    while ((sys_clk / (div * frequency)) > 65535 && div < 256) div++;
    wrap = (sys_clk / (div * frequency)) - 1;

    pwm_set_clkdiv_int_frac(pwm_slice_num, div, 0);
    pwm_set_wrap(pwm_slice_num, wrap);
    pwm_set_chan_level(pwm_slice_num, pwm_channel, wrap * 0.75f); // 75% duty cycle para volume máximo
    pwm_set_enabled(pwm_slice_num, true);
}

void buzzer_stop_tone() {
    pwm_set_enabled(pwm_slice_num, false);
}