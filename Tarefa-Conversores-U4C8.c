#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "libs/include/functions_pwm.h"
#include "libs/include/definicoes.h"

static volatile uint32_t last_time = 0;
static volatile bool led_state = true;

/*PENDENTE REFATORAÇÃO DO CÓDIGO*/



static void gpio_irq_handler(uint gpio, uint32_t events); // Declaração da função de tratamento de interrupção
void initialize_gpio(int pin, bool direction); // Declaração da função de inicialização do pino

int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(VRX_PIN); // Inicializa o pino do joystick no eixo X
    adc_gpio_init(VRY_PIN); // Inicializa o pino do joystick no eixo Y

    initialize_gpio(SW_PIN, GPIO_IN); // Inicializa o pino do botão do joystick

    initialize_gpio(LED_GREEN, GPIO_OUT); // Inicializa o pino do LED verde

    initialize_gpio(BUTTON_A, GPIO_IN); // Inicializa o pino do botão A

    initialize_pwm(LED_RED); // Inicializa o LED vermelho como saída PWM

    initialize_pwm(LED_BLUE); // Inicializa o LED azul como saída PWM

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
        set_pwm_level_r(vrx, LED_RED);
        set_pwm_level_b(vry, LED_BLUE);
        }
     
        printf("VRX: %d, VRY: %d\n", vrx, vry);
        sleep_ms(500);
    }
}

// Função de tratamento de interrupção para o botão do joystick
static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_time > 200000){
        last_time = current_time;
          if (gpio == SW_PIN) {
        gpio_put(11, !gpio_get(11)); // Se o botão do Joystick for pressionado, inverte o estado do LED verde
    } else if (gpio == BUTTON_A) {
        led_state = !led_state; // Se o botão A for pressionado, desativa os LEDs vermelho e azul
    }
    }
}

// Função para inicializar os pinos GPIO
void initialize_gpio(int pin, bool direction) {
    gpio_init(pin);
    gpio_set_dir(pin, direction);
    if(direction == GPIO_IN){
        gpio_pull_up(pin);
    } else if(direction == GPIO_OUT){
        gpio_put(pin, 0);
    }
}