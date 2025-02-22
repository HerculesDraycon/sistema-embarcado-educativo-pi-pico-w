#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "pio_matrix.pio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "lib/frames.h"

#define I2C_PORT i2c1         // Porta de inicializacao do display
#define ENDERECO 0x3C         // Endereco de inicializacao do display
#define I2C_SDA 14            // Pino SDA do display
#define I2C_SCL 15            // Pino SCL do display
#define BTN_A 5               // Pino do botao A
#define BTN_B 6               // Pino do botao B
#define OUT_PINO 7            // Pino da matriz de LED's
#define GREEN_PINO 11         // Pino do LED verde do RGB
#define BLUE_PINO 12          // Pino do LED azul do RGB
#define RED_PINO 13           // Pino do LED vermelho do RGB
#define NUMERO_DE_LEDS 25     // Numero de LED's na matriz
#define DEBOUNCE_LINE 300000  // 300ms em microsegundos

bool cor = false;                   // Flag de controle da cor representativa do monitor
ssd1306_t ssd;                      // Inicializa a estrutura do display
volatile int estado_op = 0;         // Variavel de controle dos estados operantes na execucao
volatile int btn_a_acionado = 0;    // Variavel de controle do 'botao a' pressioando
volatile int btn_b_acionado = 0;    // Variavel de controle do 'botao b' pressioando
uint32_t tempo_msg = 0;             // Armazena o tempo de inicio da exibicao
bool mensagem_tela = false;         // Indica se a mensagem esta ativa

// Funcao responsavel por estruturar o funcionamento da matriz de leds usada no projeto
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Funcao responsavel por exibir em LED's o desenho matricial passado como parametro
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUMERO_DE_LEDS; i++) {
            valor_led = matrix_rgb(desenho[24 - i], 0.0, 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
}
// Funcao que recebe strings como parametro e as exibem no ssd
void exibir_ssd(char *f1, char *f2, char *f3){

    ssd1306_fill(&ssd, cor);  // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);  // Desenha um retangulo
    ssd1306_draw_string(&ssd, f1, 8, 10);  // Desenha uma string
    ssd1306_draw_string(&ssd, f2, 8, 30);  // Desenha uma string
    ssd1306_draw_string(&ssd, f3, 8, 48);  // Desenha uma string      
    ssd1306_send_data(&ssd);  // Atualiza o display

}
// Funcao com rotina de interrupcao (IRQ) que detecta e controla os botoes pressionados
void button_callback(uint gpio, uint32_t events){
    // Obtencao do tempo real em microsegundos numa variavel uint
    uint32_t marco = time_us_32();  
    //Utilizacao de debounce controlado por sowtware para fazer com que o botao pressionado execute a instrucao somente uma vez
    if(gpio == BTN_A && (marco -  btn_a_acionado) > DEBOUNCE_LINE){

        btn_a_acionado = marco;

        exibir_ssd("Preparando", "para o Desafio", "dos Indicadores");
        tempo_msg = to_ms_since_boot(get_absolute_time());
        mensagem_tela = true;

        estado_op = 1;

    } else if(gpio == BTN_B && (marco -  btn_b_acionado) > DEBOUNCE_LINE){

        btn_b_acionado = marco;

        exibir_ssd("Preparando", "para o Desafio", "do Bip");
        tempo_msg = to_ms_since_boot(get_absolute_time());
        mensagem_tela = true;

        estado_op = 2;

    }
    
}
// Funcao que recebe os limites e sorteia um numero nesse intervalo
int rand_num(int min, int max){
    int s_num = min + rand() % (max - min + 1);
}

int main(){

    srand(time(NULL));

    PIO pio = pio0;                      // Declaracao incial pio
    uint32_t valor_led = 0;              // Declaracao incial valor LED
    double r = 0.0, b = 0.0, g = 0.0;    // Declaração incial valores RGB

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PINO);

    i2c_init(I2C_PORT, 400 * 1000);             // Inicializacao do I2C com 400KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Seta a funcao do GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Seta a funcao do GPIO para I2C
    gpio_pull_up(I2C_SDA);                      // Ativa a transmissao de dados
    gpio_pull_up(I2C_SCL);                      // Ativa o clock
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);  // Inicializa o display
    ssd1306_config(&ssd);                       // Configura o display
    ssd1306_send_data(&ssd);                    // Envia os dados para o display
    // Limpa o display para que inicie com todos os pixels apagados
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    // Inicializa a biblioteca padrao
    stdio_init_all();
    // Iniciacao dos pinos do LED RGB
    gpio_init(RED_PINO);
    gpio_init(GREEN_PINO);
    gpio_init(BLUE_PINO);
    // Configuracao dos pinos do LED RGB
    gpio_set_dir(RED_PINO, GPIO_OUT);
    gpio_set_dir(GREEN_PINO, GPIO_OUT);
    gpio_set_dir(BLUE_PINO, GPIO_OUT);
    // Iniciacao dos botoes 'a' e 'b'
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    // Configuracao dos botoes 'a' e 'b'
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);
    // Definicao de resistores pull-up via software dos botoes 'a' e 'b'
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);
    // Chamada de funcao que executa instrucoes com as rotinas de interrupcao habilitadas para cada botao
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    // Exibicao previa da tela ja no inicio do sistema
    exibir_ssd("Pressione A ou", "B para seu", "referido teste");

    while (true) {

        if(mensagem_tela && (to_ms_since_boot(get_absolute_time()) - tempo_msg >= 2000)){
            exibir_ssd("Pressione A ou", "B para seu", "referido teste");
            mensagem_tela = false;
        }
        
        sleep_ms(10);

    }
    
}
