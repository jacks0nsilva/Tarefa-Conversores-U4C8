#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define VRX_PIN 27
#define VRY_PIN 26
#define SW_PIN 22
static volatile uint32_t last_time = 0;
static volatile bool led_state = true;

/*PENDENTE REFATORAÇÃO DO CÓDIGO*/

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

void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_time > 200000){
        last_time = current_time;
          if (gpio == SW_PIN) {
        gpio_put(11, !gpio_get(11));
    } else if (gpio == 5) {
        led_state = !led_state;
    }
    }
}


int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(VRX_PIN); // Inicializa o pino do joystick no eixo X
    adc_gpio_init(VRY_PIN); // Inicializa o pino do joystick no eixo Y
    gpio_init(SW_PIN); // Inicializa o pino do botão do joystick
    gpio_set_dir(SW_PIN, GPIO_IN); // Define o pino do botão do joystick como entrada
    gpio_pull_up(SW_PIN); // Habilita o pull-up no pino do botão do joystick

    gpio_init(11);
    gpio_set_dir(11, GPIO_OUT);

    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5);

    initialize_pwm(13);

    initialize_pwm(12);
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(5, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);


    while (true) {
        adc_select_input(1);
        uint16_t vrx = adc_read(); // Lê o valor do joystick no eixo X (0 - 4095)
        sleep_us(10);

        adc_select_input(0);
        uint16_t vry = adc_read(); // Lê o valor do joystick no eixo Y
        sleep_us(10);
        
        if(led_state){
        set_pwm_level_r(vrx, 13);
        set_pwm_level_b(vry, 12);
        }
     
        printf("VRX: %d, VRY: %d\n", vrx, vry);
        sleep_ms(500);
    }
}

