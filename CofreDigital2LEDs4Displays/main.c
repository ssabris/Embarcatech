/*
* Aluna: Sabrina Bernardi da Silva
* Projeto: Cofre Digital 2 LEDs 4 Displays 
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>

//-----------------------TECLADO MATRICIAL-----------------------//

#define SET 1
#define RESET 0
#define COMP 4

char senha[COMP + 1] = {0};

// Define os pinos GPIO para as linhas e para as colunas
const uint8_t linha[] = {2, 3, 4, 5};
const uint8_t coluna[] = {6, 7, 8, 9};

// Mapeamento das teclas em uma matriz 4x4
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//--------------------------DISPLAYS------------------------------//

// Definição dos pinos dos segmentos do display e os controles dos displays 1 e 2
const uint8_t segmentos[] = {28, 27, 26, 22, 21, 20, 19}; // Pinos dos leds dos segmentos a,b,c,d,e,f,g
const uint8_t display[] = {12, 13, 14, 15}; // Pinos de controle do display

// Mapeamento dos dígitos para os segmentos do display
const uint8_t digits[13][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 0, 0, 1, 1}, // 9
    {1, 0, 0, 1, 1, 1, 1}, // E
    {1, 1, 1, 0, 1, 1, 1}, // R
    {0, 0, 0, 0, 0, 0, 1}  // -
};

//---------------------------LEDS-----------------------------------//

// Define os pinos GPIO para as cores do LED RGB
#define LDVRM 10
#define LDVRD 11

//-------------------------INICIALIZAÇÃO DOS PINOS---------------------//

// Inicializando os pinos GPIO 
void setup() {
    for (int i = 0; i < 4; i++) {
        // Configura as linhas do teclado matricial como saídas
        gpio_init(linha[i]);
        gpio_set_dir(linha[i], GPIO_OUT);
        gpio_put(linha[i], RESET);
        // Configura as colunas do teclado matricial como entradas
        gpio_init(coluna[i]);
        gpio_set_dir(coluna[i], GPIO_IN);
        gpio_pull_down(coluna[i]); // Força as colunas para zero
    }

    // Inicializa e configura os pinos dos displays como saídas
    for (int i = 0; i < 7; i++) {
        gpio_init(segmentos[i]);
        gpio_set_dir(segmentos[i], GPIO_OUT);
    }
    for (int i = 0; i < 4; i++) {
        gpio_init(display[i]);
        gpio_set_dir(display[i], GPIO_OUT);
        gpio_put(display[i], 1); // Desativa inicialmente os displays
    }

    // Inicializa os pinos dos LEDs e os configura como saída
    gpio_init(LDVRM);
    gpio_init(LDVRD);
    gpio_set_dir(LDVRM, GPIO_OUT); // Define o pino do LED vermelho como saída
    gpio_set_dir(LDVRD, GPIO_OUT); // Define o pino do LED verde como saída
}

//-------------------------------FUNÇÕES AUXILIARES-------------------------------//

// Controla os estados dos LEDs RGB (liga ou desliga cada cor)
void led_rgb_put(bool vm, bool vd) {
    gpio_put(LDVRM, vm); // Define o estado do LED vermelho
    gpio_put(LDVRD, vd); // Define o estado do LED verde
}

// Função para exibir um dígito em um display específico
void mostrar_digito(uint8_t disp, uint8_t digito) {
    // Desativa todos os displays
    for (int i = 0; i < 4; i++) {
        gpio_put(display[i], 1);
    }

    // Ativa os segmentos correspondentes ao dígito
    for (int i = 0; i < 7; i++) {
        gpio_put(segmentos[i], digits[digito][i]);
    }

    // Ativa o display desejado
    gpio_put(display[disp], 0);
}

// Função para exibir os quatro dígitos nos displays usando multiplexação
void mostrar_displays(char *digitos) {
    for (int i = 0; i < 4; i++) {
        mostrar_digito(i, digitos[i] - '0'); // Exibe o dígito no display i
        sleep_ms(5); // Pequeno delay para multiplexação
    }
}

// Função para exibir quatro traços nos displays
void mostrar_tracos() {
    for (int i = 0; i < 4; i++) {
        mostrar_digito(i, 12); // Exibe traço no display i
    }
}

void mux_mostrar_tracos(){
  for (int i = 0; i < 4; i++){
    mostrar_tracos();
    sleep_ms(5);
  }
}

// Função para exibir a mensagem de erro "erro"
void mostrar_erro() {
    // Exibe 'E' no segundo display
    mostrar_digito(0, 10); // E
    // Exibe 'R' no terceiro display
    mostrar_digito(1, 11); // R
    // Exibe 'R' no quarto display
    mostrar_digito(2, 11); // R    
    // Exibe '0' no quarto display
    mostrar_digito(3, 0); // 0
}

void mux_mostrar_erro(){
  for (int i = 0; i < 4; i++){
    mostrar_erro();
    sleep_ms(5);
  }
}

// Função para piscar os displays com traços
void piscar_tracos() {
    for (int i = 0; i < 3; i++) {
        mostrar_tracos();
        sleep_ms(500);
        for (int j = 0; j < 4; j++) {
            mostrar_digito(j, 13); // Desliga o display j
        }
        sleep_ms(500);
    }
}

char leitura_teclado() {
    for (int row = 0; row < 4; row++) {
        // Coloca a linha atual em nível alto
        gpio_put(linha[row], SET);

        for (int col = 0; col < 4; col++) {
            if (gpio_get(coluna[col])) {
                // Espera um tempo para estabilização da tecla
                sleep_ms(50);
                while (gpio_get(coluna[col])); // Espera até a tecla ser solta
                gpio_put(linha[row], RESET); // Põe a linha atual em nível baixo
                return teclas[row][col];
            }
        }
        gpio_put(linha[row], RESET); // Põe a linha atual em nível baixo
    }
    return 0;
}

// Função para pegar a senha digitada
void get_senha(char *senha, int tam) {
    int index = 0; // Índice para o vetor senha[]
    char digitos[4] = {' ', ' ', ' ', ' '}; // Armazena os dígitos para exibição

    while (index < tam) { // Mantém o loop enquanto o vetor senha[] não for completado
        char key = leitura_teclado(); // A variável key recebe a tecla pressionada

        if (key != 0 && key != '*' && key != '#' && key !='A' && key != 'B' && key != 'C' && key != 'D') { // Ignora algumas teclas
            printf("%c", key); // Mostra a tecla pressionada
            senha[index] = key; // Grava a tecla pressionada no vetor senha[]
            digitos[index] = key; // Atualiza o dígito correspondente
            index = index + 1; // Passa para o próximo índice
        }

        // Atualiza os displays com os dígitos atuais
        mostrar_displays(digitos);
        sleep_ms(100);
    }
    senha[tam] = '\0';
}

// Função para comparar duas senhas
bool compare_senhas(const char *senha, const char *senha2) {
    return strcmp(senha, senha2) == 0;
}



//----------------------------FUNÇÃO PRINCIPAL---------------------------------//

int main() {
    stdio_init_all();
    setup();

    printf("CADASTRE SUA SENHA DO COFRE COM 4 DÍGITOS! \n");
    get_senha(senha, COMP); // Chama a função get_senha
    printf("\nSenha gravada!\n");

    int cont = 0;
    bool acesso = 1;

    while (acesso) {
        printf("Digite a senha do cofre de 4 dígitos!\n");
        char input[COMP + 1] = {0}; // Define o vetor input
        get_senha(input, COMP); // Chama a função get_senha para completar o vetor input

        if (compare_senhas(input, senha)) { // Compara o vetor input com o vetor senha (senha digitada x senha cadastrada)
            printf("\nSenha correta: cofre aberto!\n");
            led_rgb_put(0, 1); // Acende o LED verde
            for (int i = 0; i < 100; i++){
              mux_mostrar_tracos(); // Mostra quatro traços nos displays
              sleep_ms(5);
            }
            for (int i = 0; i < 4; i++) {//desliga os displays após a indicação
              gpio_put(display[i], 1);
            }
            acesso = !acesso; // Sair do loop caso a senha seja igual à senha gravada
        } else {
            printf("\nSenha incorreta: acesso negado!\n");
            led_rgb_put(1, 0); // Acende o LED vermelho
            for (int i = 0; i < 100; i++){
              mux_mostrar_erro(); // Mostra a mensagem de erro "-err"
              sleep_ms(5);
            }
            cont = cont + 1;
        }
        sleep_ms(50); // Estabilização

        if (cont > 2) {
            printf("\nTrês tentativas incorretas. Cofre bloqueado por 10 segundos.\n");
            led_rgb_put(1, 0); // Mantém o LED vermelho aceso
            for (int i = 0; i < 4; i++) {//desliga os displays após a indicação
              gpio_put(display[i], 1);
            }
            sleep_ms(10000); // Tempo de bloqueio do cofre
            cont = 0; // Reseta a contagem de tentativas
        }
    }
}