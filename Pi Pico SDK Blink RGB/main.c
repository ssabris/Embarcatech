/*
*
Projeto: Pi Pico SDK Blink RGB
Aluna: Sabrina Bernardi da Silva
*
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define os pinos GPIO para as cores do LED RGB
#define LEDR 14 //Vermelho
#define LEDG 13 //Verde
#define LEDB 12 //Azul

// Controla os estados dos LEDs RGB (liga ou desliga cada cor)
void led_rgb_put(bool r, bool g, bool b) {
  gpio_put(LEDR, r); //define o estado do LED vermelho
  gpio_put(LEDG, g); //define o estado do LED verde
  gpio_put(LEDB, b); //define o estado do LED azul
}

int main() {
  
    stdio_init_all();
   
    // Inicializa os pinos GPIO e os configura como saída
    gpio_init(LEDR);
    gpio_init(LEDG);
    gpio_init(LEDB);
    gpio_set_dir(LEDR,true); //define o pino de LED vermelho como saída
    gpio_set_dir(LEDG,true); //define o pino de LED verde como saída
    gpio_set_dir(LEDB,true); //define o pino de LED azul como saída
    
    while (true) {
        // Liga apenas o LED vermelho por 500 ms
        led_rgb_put(true, false, false);
        sleep_ms(500);

        // Liga apenas o LED verde por 500 ms
        led_rgb_put(false, true, false);
        sleep_ms(500);

        // Liga apenas o LED azul por 500 ms
        led_rgb_put(false, false, true);
        sleep_ms(500);

        // Liga apenas o LED branco por 500 ms
        led_rgb_put(true, true, true);
        sleep_ms(500);
        
    }
}
