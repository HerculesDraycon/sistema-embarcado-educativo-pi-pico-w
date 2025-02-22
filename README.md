# Sistema Embarcado Educativo de Foco e Cognição

**LINK DO VÍDEO ENSAIO:** 

# Documentação do Código

## **Descrição Geral**

Esse é o projeto de um sistema embarcado para fins educativos que visa explorar os horizontes de diagnóstico e tratamento de pacientes portadores de divergência cognitiva. O desenvolvimento une metodologias atualmente usada em esportes de alta performance com microcontroladores e seus periféricos para que entreque uma excelente experiência de interação do paciente com atividades descontraídas mas com alto teor terapêutico.

## **Bibliotecas Utilizadas**

### **Padrão do C**
- `<stdio.h>`: Manipulação de entrada/saída.

### **Pico SDK**
- `pico/stdlib.h`: Configuração padrão para Raspberry Pi Pico. Pôde apresentar toda a estrutura necessária para que esse código funcionasse corretamente, sem a necessidade de mais bibliotecas de *hardware*.
- `hardware/clocks.h`: Biblioteca que realiza controle de clock do sistema.
- `hardware/pio.h`: Manipulação de entradas e saídas programáveis.
- `hardware/i2c.h`: Configuração e comunicação do dispositivo I2C com o sistema.

### **Customizadas**
- `"pio_matrix.pio.h"`: Configuração do PIO para controle da matriz de LEDs.
- `lib/frames.h`: Frames que representam os números exibidos na matriz de LEDs.

## **Constantes**
- `I2C_PORT`: Porta de inicialização do display.
- `ENDERECO`: Enderoço de inicialização do display.
- `I2C_SDA`: Pino SDA do display.
- `I2C_SCL`: Pino SCL do display.
- `OUT_PINO`: Pino da matriz de LEDs.
- `NUMERO_DE_LEDS`: Define o numero de LEDs na matriz.
- `GREEN_PINO`: Define o pino de saída utilizado pelo LED verde do RGB.
- `BLUE_PINO`: Define o pino de saída utilizado pelo LED azul do RGB.
- `RED_PINO`: Define o pino de saída utilizado pelo LED vermelho do RGB.
- `BTN_A`: Define o pino de entrada utilizado pelo Botão A.
- `BTN_B`: Define o pino de entrada utilizado pelo Botão B.
- `DEBOUNCE_LINE`: Define em microsegundos o tempo a ser utilizado no debounce do botão.

## **Funções**

### **1. Manipulação de LEDs**
#### `uint32_t matrix_rgb(double r, double g, double b)`
- **Descrição**: Converte os valores de vermelho (r), verde (g), e azul (b) para um formato hexadecimal de 32 bits.
- **Parâmetros**:
  - `r`, `g`, `b`: Intensidades de vermelho, verde e azul (0.0 a 1.0).
- **Retorno**: Valor RGB no formato hexadecimal.
- **Uso**: Criação de cores para o LED.

---

#### `void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)`
- **Descrição**: Atualiza os LEDs com base em um padrão (`desenho`), aplicando uma cor uniforme.
- **Parâmetros**:
  - `desenho`: Array com o padrão dos LEDs.
  - `valor_led`: Valor RGB.
  - `pio`: Instância do PIO.
  - `sm`: State Machine do PIO.
  - `r`, `g`, `b`: Intensidades de vermelho, verde e azul.

### **2. Controle dos Botões**

#### `void button_callback(uint gpio, uint32_t events)`
- **Descrição**: Realiza as rotinas de interrupção (IRQ) e executa a sequência de instruções ao respectivo botão pressionado.
- **Parâmetros**:
  - `gpio`: uint que corresponde ao pino GPIO em que o botão está conectado e gerou a interrupção.
  - `events`: Passa as informações sobre que tipo evento na GPIO ocorreu.

### **3. Controle de Exibição no Display**

### `void exibir_ssd(char *f1, char *f2, char *f3)`
- **Descrição**: Baseado na Strings (arrays de chars) recebidas por parâmetro, as exibem de forma subsequente no display, o que dinamiza a operação em toda vez que uma mensagem precisar de ser exibida.
- **Parâmetros**:
  - `f1`, `f2` e `f3`: Arrays de chars referenciadas por ponteiro que passam as strings para a função.

### **4. Funções de otimização do código**

### `int rand_num(int min, int max)`
- **Descrição**: Baseado nos dois ints passados como limites de um intervalo, a função retorna um número sorteado naquele intervalo;
- **Parâmetros**:
  - `min` e `max`: ints passados como limites dos intervalos.

### **5. Função Principal (`main`)**
- **Descrição**:
  - Configura e inicializa periféricos.
  - Inicializa a matriz de LEDs desligados.
  - Entra em um loop infinito que:
    1. Formata a mensagem inicial exibida no SSD1306.
    2. Inicia a sequência de estados de execução do programa baseado na escolha realizada pelo usuário.
    3. Exibe os símbolos e mensagens em sequência nos estados transicionados para que o usuário possa interagir.
- **Destaques**:
  - Inicializa o PIO com a configuração `pio_matrix`.

## **Fluxo do programa**

- O Display SSD1306 inicialmente apresenta uma mensagem padrão e a matriz de LEDs 5x5 está apagada.

- Ao acionar um dos botões, um dos modos de operação do sistema se inicia.

- O usuário prossegue interagindo com os botões de acordo a cordenação do software que exibirá mensagens no monitor e na matriz de LEDs.
