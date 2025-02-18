#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "libs/include/functions_pwm.h"
#include "libs/include/definicoes.h"
#include "libs/include/ssd1306.h"

static volatile uint32_t last_time = 0;
static volatile bool led_state = true;
static volatile bool retangule_state = false;

ssd1306_t ssd;

// Variáveis para definir a posição inicial do eixo X e Y do quadrado
uint8_t square_x = 60; // Posição inicial do quadrado no eixo X
uint8_t square_y = 28; // Posição inicial do quadrado no eixo Y


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


    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o display em 400KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define a SDA como pino de comunicação I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Define a SCL como pino de comunicação I2C

    gpio_pull_up(I2C_SDA); // Habilita o pull-up na SDA
    gpio_pull_up(I2C_SCL); // Habilita o pull-up na SCL
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO,I2C_PORT); // Inicializa o display OLED
    ssd1306_config(&ssd); // Configura o display OLED
    ssd1306_send_data(&ssd); // Envia os dados para o display OLED


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

        // Define deslocamento baseado nos valores do joystick
        int8_t dx = 0, dy = 0;

        if (vrx < 1400) dx = -4; // Move para a esquerda
        if (vrx > 2700) dx = 4;  // Move para a direita
        if (vry < 1400) dy = 4; // Move para cima
        if (vry > 2700) dy = -4;  // Move para baixo

        // Atualiza a posição do quadrado, garantindo que fique dentro dos limites

        square_x = (square_x + dx >= 2 && square_x + dx <= 116) ? square_x + dx : square_x;
        square_y = (square_y + dy >= 2 && square_y + dy <= 52) ? square_y + dy : square_y;


        // Limpa o display antes de desenhar o novo quadro
        ssd1306_fill(&ssd, false);

        // Redesenha o retângulo externo
        ssd1306_rect(&ssd, 0, 0, 128, 64, true, false);
        if(retangule_state) ssd1306_rect(&ssd, 2, 2, 124, 60, true, false); 

        // Desenha o quadrado na nova posição
        ssd1306_rect(&ssd, square_y, square_x, 8, 8, true, true);

        // Envia os dados atualizados para o display
        ssd1306_send_data(&ssd);

        // Aguarda um curto período de tempo para evitar uma taxa de atualização muito alta
        sleep_ms(50);
    }
}

// Função de tratamento de interrupção para o botão do joystick
static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_time > 200000){
        last_time = current_time;
          if (gpio == SW_PIN) {
        gpio_put(LED_GREEN, !gpio_get(LED_GREEN)); // Se o botão do Joystick for pressionado, inverte o estado do LED verde
        retangule_state = !retangule_state;
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