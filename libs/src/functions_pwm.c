#include "libs/include/functions_pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

void initialize_pwm(int pwm_pin){
    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pwm_pin);
    pwm_set_clkdiv(slice, 2.0);
    pwm_set_wrap(slice, 2000);
    pwm_set_gpio_level(pwm_pin, 0);
    pwm_set_enabled(slice, true);
}

void set_pwm_level_r(uint16_t level, int pwm_pin){
    if (level > 2000 && level < 2300) {
        pwm_set_gpio_level(pwm_pin, 0);
    } else {
        uint16_t pwm_level = abs(level - 2145) * 2;  // Ajuste da intensidade
        pwm_set_gpio_level(pwm_pin, pwm_level);
    }
}

void set_pwm_level_b(uint16_t level, int pwm_pin){
    if (level > 1880 && level < 2280) {
        pwm_set_gpio_level(pwm_pin, 0);
    } else {
        uint16_t pwm_level = abs(level - 2145) * 2;  // Ajuste da intensidade
        pwm_set_gpio_level(pwm_pin, pwm_level);
    }
}