#ifndef FUNCTIONS_PWM_H
#define FUNCTIONS_PWM_H


#include <stdint.h>

void initialize_pwm(int pwm_pin);
void set_pwm_level_r(uint16_t level, int pwm_pin);
void set_pwm_level_b(uint16_t level, int pwm_pin);

#endif