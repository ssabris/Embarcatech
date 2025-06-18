/* 
Aluna: Sabrina Bernardi da Silva
Enunciado da questão 5:
Modifique o exemplo de código apresentado na videoaula (reproduzido abaixo) para 
controlar os três LEDs RGB da placa BitDogLab usando o módulo PWM e interrupções, 
seguindo as orientações a seguir: 
- O LED vermelho deve ser acionado com um PWM de 1kHz. 
- O duty cycle deve ser iniciado em 5% e atualizado a cada 2 segundos em incrementos 
de 5%. Quando atingir o valor máximo, deve retornar a 5%. 
- O LED azul deve ser acionado com um PWM de 10kHz.

Tomei a liberdade de acrescentar o LED verde, intercalando com o LED azul. 
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs comuns
#define LED_R 13  // Pino do LED Vermelho (PWM)
#define LED_B 11  // Pino do LED Azul (ON/OFF)
#define LED_G 12  // Pino do LED Verde (ON/OFF)

// Frequências PWM para o LED vermelho
#define PWM_FREQ_R 1000  // 1 kHz para LED Vermelho

// Duty Cycle inicial do LED vermelho
volatile uint16_t duty_cycle_r = 5;

// Configuração do PWM para o LED vermelho
uint setup_pwm(uint gpio, uint freq) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint wrap = 125000000 / (freq * 16);  // Ajuste correto do clock

    pwm_set_clkdiv(slice, 16.0f);
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(gpio, (wrap * duty_cycle_r) / 100);
    pwm_set_enabled(slice, true);

    return slice;
}

// Alterna o estado do LED azul e do LED verde
bool toggle_leds(struct repeating_timer *t) {
    static bool led_state = false;
    led_state = !led_state;
    gpio_put(LED_B, led_state);
    gpio_put(LED_G, !led_state);
    printf("LED Azul: %s | LED Verde: %s\n", led_state ? "ON" : "OFF", led_state ? "OFF" : "ON");
    return true;  // Continua a interrupção
}

// Atualiza o brilho do LED vermelho a cada 2 segundos
bool update_red_led(struct repeating_timer *t) {
    duty_cycle_r += 5;
    if (duty_cycle_r > 100) duty_cycle_r = 5;  // Reseta para 5%

    uint slice_r = pwm_gpio_to_slice_num(LED_R);
    uint wrap_r = 125000000 / (PWM_FREQ_R * 16);
    pwm_set_gpio_level(LED_R, (wrap_r * duty_cycle_r) / 100);

    printf("Novo duty cycle do LED Vermelho: %d%%\n", duty_cycle_r);
    return true;  // Continua a interrupção
}

int main() {
    stdio_init_all();
    printf("Iniciando controle dos LEDs...\n");

    // Configuração dos LEDs comuns
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);  // Começa desligado

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 1);  // Começa ligado

    setup_pwm(LED_R, PWM_FREQ_R);  // Configura LED Vermelho com PWM

    struct repeating_timer timer_r, timer_toggle;
    add_repeating_timer_ms(2000, update_red_led, NULL, &timer_r);  // PWM LED Vermelho
    add_repeating_timer_ms(1000, toggle_leds, NULL, &timer_toggle);  // Pisca LED Azul e Verde

    while (1) {
        tight_loop_contents();  // Mantém o loop rodando
    }
}
