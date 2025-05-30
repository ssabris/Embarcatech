/*
Projeto controle de led RGB com dois botoes
Aluna: Sabrina Bernardi da Silva
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define os pinos GPIO para as cores do LED RGB
#define LED_R_PIN 13 //Vermelho
#define LED_G_PIN 11 //Verde
#define LED_B_PIN 12 //Azul
#define BTN_A_PIN 5
#define BTN_B_PIN 6

// Controla os estados dos LEDs RGB (liga ou desliga cada cor)
void led_rgb_put(bool r, bool g, bool b) {
  gpio_put(LED_R_PIN, r); //define o estado do LED vermelho
  gpio_put(LED_G_PIN, g); //define o estado do LED verde
  gpio_put(LED_B_PIN, b); //define o estado do LED azul
}

int main() {
  
    stdio_init_all();
   
    // Inicializa os pinos GPIO e os configura como saída
    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT); //define o pino de LED vermelho como saída

    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT); //define o pino de LED verde como saída

    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT); //define o pino de LED azul como saída

    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);

    // Loop principal
    while (true) {

        if(gpio_get(BTN_A_PIN) && gpio_get(BTN_B_PIN)){
        // Liga apenas o LED verde por 100 ms
        led_rgb_put(0, 1, 0);
        sleep_ms(100);
        } 

        else if (!gpio_get(BTN_A_PIN)){
        // Liga apenas o LED azul por 100 ms
        led_rgb_put(0, 0, 1);
        sleep_ms(100);
        }
        
        else if (!gpio_get(BTN_B_PIN)){
        // Liga apenas o LED vermelho por 100 ms
        led_rgb_put(1, 0, 0);
        sleep_ms(100);
        }

        else {
        led_rgb_put(0, 0, 0);
        sleep_ms(100);
        }   
    }
}
