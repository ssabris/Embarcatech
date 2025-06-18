/* 
Aluna: Sabrina Bernardi da Silva
Refaça o programa prático 01 presente no Ebook do Capítulo de ADC
mude a unidade de medida da temperatura de celsius para fahrenheit
*/

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>

// Função para ler a temperatura do sensor
float lerTemperatura() {
    adc_select_input(0); // Seleciona o canal ADC 0
    uint16_t sensorValue = adc_read(); // Ler valor do sensor no ADC0
    float voltage = sensorValue * (3.3 / 4095.0); // Converter valor lido para tensão (0-3.3V)
    float temperatureC = (voltage - 0.5) * 100.0; // Converter tensão para temperatura em Celsius
    return temperatureC;
}

int main() {
    // Inicializa a comunicação serial
    stdio_init_all();

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(26); // Inicializa o pino GPIO 26 (ADC0)

    while (true) {
        float celsius = lerTemperatura();
        float fahrenheit = celsius * 9.0 / 5.0 + 32.0; // Converter Celsius para Fahrenheit
        
        printf("Temperatura: %.2f °C\n", celsius);
        printf("Temperatura em Fahrenheit: %.2f °F\n", fahrenheit);

        sleep_ms(1000); // Aguardar 1 segundo antes de ler novamente
    }

    return 0;
}
