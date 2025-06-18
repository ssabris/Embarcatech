/* 
Aluna: Sabrina Bernardi da Silva
Enunciado da questão 4:
Já para a comunicação I2C, iremos utilizar o DS1307, que é um Real Time Clock – RTC 
disponível no simulador Wokwi. O endereço I2C do DS1307 é 0x68. Um RTC é um hardware 
que garante a contagem de tempo na unidade de segundos. Muitos microcontroladores 
possuem RTC internos, mas alguns fazem uso de hardware externos. Para ler os valores, 
é necessário inicialmente configurar um valor de data e hora que deve, por exemplo, 
ser configurado manualmente pelo usuário. Nessa questão você deverá configurar o RTC 
para 24/09/2024 – 13:27:00 e em seguida, realizar a leitura do mesmo a cada 5 segundos, 
e imprimir na tela do console (Serial USB) o valor lido. Na tabela a seguir são 
apresentados os principais endereços do RTC DS1307.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SDA_PIN 20
#define SCL_PIN 21
#define DS1307_ADDRESS 0x68

uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

uint8_t bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

// Configuração inicial do RTC DS1307
void set_rtc_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year) {
    uint8_t data[8];
    data[0] = 0x00;
    data[1] = dec_to_bcd(sec);   
    data[2] = dec_to_bcd(min);   
    data[3] = dec_to_bcd(hour);  
    data[4] = dec_to_bcd(day);   
    data[5] = dec_to_bcd(date);  
    data[6] = dec_to_bcd(month); 
    data[7] = dec_to_bcd(year);  

    i2c_write_blocking(i2c0, DS1307_ADDRESS, data, 8, false);
}

// Leitura do RTC DS1307
void read_rtc_time() {
    uint8_t reg = 0x00;
    uint8_t data[7];

    i2c_write_blocking(i2c0, DS1307_ADDRESS, &reg, 1, true);

    i2c_read_blocking(i2c0, DS1307_ADDRESS, data, 7, false);

    uint8_t sec = bcd_to_dec(data[0] & 0x7F); // Máscara para ignorar o bit CH
    uint8_t min = bcd_to_dec(data[1]);
    uint8_t hour = bcd_to_dec(data[2] & 0x3F); // Máscara para formato 24h
    uint8_t day = bcd_to_dec(data[3]);
    uint8_t date = bcd_to_dec(data[4]);
    uint8_t month = bcd_to_dec(data[5]);
    uint8_t year = bcd_to_dec(data[6]);

    printf("Data/Hora: %02d/%02d/20%02d - %02d:%02d:%02d\n", date, month, year, hour, min, sec);
}

int main() {
    stdio_init_all();
    i2c_init(i2c0, 100 * 1000); // Frequência de 100 kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("Iniciando RTC DS1307...\n");

    // Configurar o RTC para 24/09/2024 - 13:27:00
    set_rtc_time(0, 27, 13, 2, 24, 9, 24);
    printf("RTC configurado para 24/09/2024 - 13:27:00\n");

    while (true) {
        // Sem uso de timers para a leitura do RTC ja que o foco é o SPI
        read_rtc_time();
        sleep_ms(5000);
    }
}