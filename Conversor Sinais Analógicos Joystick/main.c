/*
Aluna: Sabrina Bernardi da Silva

Questão 7 - Como o ADC converte sinais analógicos do joystick em 
valores digitais no exemplo 02

O programa abaixo tem a finalidade de realizar a leitura analógica dos sinais 
do joystick através do método one shot. O joystick é composto por dois eixos: x e y. Além desses dois eixos, 
há um botão que permite que o usuário possa clicar.

Os eixos x e y fornecem valores analógicos de tensão na saída com base na posição que
o usuário movimenta.

Como temos duas saídas analógicas do joystick, precisamos conectar esses sinais em
duas entradas analógicas. Elas serão responsáveis pela leitura individual de casa eixo.

No programa abaixo, configurmos a conexão do eixo x para o canal 0 do conversor 
analógico-digital e o eixo y será conectado ao canal 1 do conversor analógico-digital.

Pensando nisso, foi realizada a inicialização do conversor analógico-digital e,
posteriormente, fazendo a leitura individual de cada canal. 

Primeiro é selecionado o canal 0, é feita a leitura da tensão, conversão e armazenamento 
do valor convertido em uma variável.

Após isso, o processo se repete para o canal 1. O canal será selecionado, a tensão é
lida e seu valor será convertido em digital. Por fim, o valor convertido será armazenado
em uma variável.

Esse é o processo de funcionamento da leitura de tensão da saída do joystick e 
conversão do seu valor de tensão em um valor digital.
*/

#include <stdio.h>             // Biblioteca padrão para entrada e saída
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para manipulação das funções do conversor analógico-digital

// Definições dos pinos para o joystick e botão
#define VRX_PIN 26    // Define o pino GP26 para o eixo X do joystick para usar o canal 0 do ADC (ADC0)
#define VRY_PIN 27    // Define o pino GP27 para o eixo Y do joystick para usar o canal 1 do ADC (ADC1)
#define SW_PIN 22     // Define o pino GP22 para o botão do joystick. Será usado como entrada digital

int main() 
{
    
    stdio_init_all(); //Inicializa a comunicação serial da RP2040. Isso permite utilizar a função printf para imprimir mensagens na serial do PC.

    //Inicializa o circuito interno do conversor analógico-digital da RP2040
    adc_init();

    //Configura o pino GPIO26 para funcionar como entrada analógico. Ele é o canal 0 do ADC (ADC0).
    //Ele será utilizado para leitura do eixo x do joystick.
    adc_gpio_init(VRX_PIN);

    //Configura o pino GPIO26 para funcionar como entrada analógico. Ele é o canal 0 do ADC (ADC0).
    //Ele será utilizado para leitura do eixo x do joystick.
    adc_gpio_init(VRY_PIN);

    //Configuração do pino digital GPIO22 como pull-up para leitura do botão do Joystick
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);


    while (true) 
    {
        
        adc_select_input(0);// Seleciona o canal 0 do ADC (pino GP26) para leitura (eixo x)
        uint16_t vrx_value = adc_read(); //Leitura do valor do canal 0 com valores lidos entre 0 e 4096 (12 bits).

        adc_select_input(1);// Seleciona o canal 0 do ADC (pino GP27) para leitura (eixo y)
        uint16_t vry_value = adc_read(); //Leitura do valor do canal 1 com valores lidos entre 0 e 4096 (12 bits).

        bool sw_value = gpio_get(SW_PIN) == 0; //Leitura do estado do botão e verifica se ele está pressionado.
        //Temos valor lido igual a 0 quando o valor digital lido é 0, pois estamos usando o botão com configuração pull-up.

        //Abaixo temos a apresentação dos valores dos eixos X, Y e estado do botão do joystick na serial do PC.
        printf("VRX: %u, VRY: %u, SW: %d\n", vrx_value, vry_value, sw_value);


        sleep_ms(500); //Aguardo um tempo de 500ms.
    }


    return 0;
}
