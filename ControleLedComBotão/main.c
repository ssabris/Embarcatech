/**
 Projeto controle de led RGB com botão
 Autoria: Sabrina Bernardi da Silva
 Data:  14 de janeiro de 2015 as 20h24
*/
#include <stdio.h>  
#include "pico/stdlib.h"  

#define LED_PIN 11  
#define BTN_PIN 5   

int main()   
{   
  // Inicializa o pino do LED e configura como saída
    gpio_init (LED_PIN);  
    gpio_set_dir (LED_PIN, GPIO_OUT);  

  // Inicializa o pino do botão e configura como entrada
    gpio_init(BTN_PIN);  
    gpio_set_dir(BTN_PIN, GPIO_IN);  

  // Loop infinito para verificar o estado do botão e controlar o LED  
    while (true)  
    {  
       while(gpio_get(BTN_PIN))
       {
        // Acende o LED se o botão estiver pressionado
        gpio_put(LED_PIN, 1);
       }
        // Apaga o LED se o botão não estiver pressionado
        gpio_put(LED_PIN, 0);
    }  

    return 0;  
}