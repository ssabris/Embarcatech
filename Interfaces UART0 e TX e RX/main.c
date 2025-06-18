/*
Aluna: Sabrina Bernardi da Silva
Enunciado da questão 3:
Elabore um código utilizando a interfaces UART0 e conecte os fios TX e RX 
atribuídos à essa interface entre. Essa estrutura envia dados e recebe os 
dados na mesma interface, apenas para verificar seu funcionamento. Utilize 
a função scanf da biblioteca stdio para enviar via console um dado à placa, 
em seguida, transmita da UART0 para a UART1, e por fim, transmita o dado 
input para o console utilizando o printf.
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Configuração das UARTs
#define UART0_ID uart0
#define UART1_ID uart1
#define BAUD_RATE 115200

// Pinos GPIO para UART0 e UART1
#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_TX_PIN 4
#define UART1_RX_PIN 5

void setup_uart() {
    uart_init(UART0_ID, BAUD_RATE);
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);

    uart_init(UART1_ID, BAUD_RATE);
    gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);
}

int main() {
    stdio_init_all();
    setup_uart();

    char input[100];

    while (true) {
        printf("Digite mensagem para enviar via UART 0: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove o '\n' do final

        uart_puts(UART0_ID, input); // Envia a mensagem pela UART0

        sleep_ms(100); // Aguarda a transmissão

        if (uart_is_readable(UART1_ID)) {
            char received[100];
            int i = 0;

            while (uart_is_readable(UART1_ID) && i < sizeof(received) - 1) {
                received[i++] = uart_getc(UART1_ID);
            }
            received[i] = '\0'; // Finaliza a string

            printf("\nMensagem recebida da UART1: %s\n", received);
        }
        else {
        printf("Nenhuma mensagem recebida da UART1.\n");
    } 
    }
}
