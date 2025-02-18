# Atividade 8 Capítulo 4 - Conversores ADC

## Visão Geral 🌐

Tarefa prática do programa de residência EmbarcaTech. Este projeto tem como objetivo explorar as funcionalidades dos conversores analógico-digitais (ADC) presentes no microtrolador Raspberry Pi Pico W.

## Vídeo de apresentação 🎥

[Vídeo de apresentação do projeto](https://drive.google.com/file/d/1XZvW8hK3Y7LITWp9lEv-nS9K51GHDu37/view?usp=drive_link)

## Funcionalidades 📌

- Controle da intensidade dos LEDs vermelho e azul com PWM, utilizando um joystick presente no kit de desenvolvimento.
- Protocolo de comunicação I2C na integreação do display OLED.
- Manipulação de um quadrado no display OLED com o joystick.
- Tratamento de interrupções com o uso de botões presentes no kit de desenvolvimento.

## Hardware 🛠️

1. Placa de desenvolvimento BitDogLab

2. Display OLED SSD1306 (com comunicação I2C)

   - GPIO 14 (SDA)
   - GPIO 15 (SCL)

3. Joystick

   - GPIO 26 (Y)
   - GPIO 27 (X)
   - GPIO 22 (BTN)

4. Botão A

- GPIO 5

5. Resistores

6. LEDs RGB

   - GPIO 13 (R)
   - GPIO 11 (G)
   - GPIO 12 (B)

## Fluxo de Execução 🚀

1. **Inicialização:**

   - Configuração dos periféricos (ADC, PWM, I2C).
   - Estabelecimento dos pinos de entrada e saída.

2. **Leitura do ADC:**

   - Os valores dos ADC são lidos periodicamente.
   - O joystick é monitorado para captar comandos do usuário.

3. **Processamento dos Dados:**

   - Os valores lidos são processados para determinar o nível de PWM a ser aplicado aos LEDs.
   - A posição do joystick é mapeada para mover o objeto no display OLED.

4. **Atualização da Interface:**

   - O display OLED é atualizado via I2C para apresentar as mudanças visuais.
   - As interrupções de botão são tratadas para realizar operações imediatas, como pausa ou reinicialização de alguma função.

5. **Loop Principal:**
   - O código roda num loop contínuo, verificando as entradas e atualizando as saídas conforme as mudanças.

## Funcionalides do Projeto 🎯

- O LED azul tem o brilho ajustado pelo eixo Y do joystick.
- O LED vermelho tem o brilho ajustado pelo eixo X do joystick.
- O display OLED mostra um quadrado 8x8 que pode ser movido com o joystick.
- O botão A pode ser acionado para desligar os LEDs vermelho e azul, além de adicionar um retângulo no display OLED.
- O botão do joystick alterna o estado do LED verde.

## Como executar o projeto 🛠️

1.  **Configuração do Ambiente:**

    - Certifique-se de ter o SDK do Raspberry Pi Pico instalado e configurado corretamente.
    - Configure as ferramentas de compilação (CMake, etc.).

2.  **Clone o repositório**
    ```
    git clone https://github.com/jacks0nsilva/Tarefa-Conversores-U4C8
    ```
3.  **Instale a extensão do Raspberry Pi Pico no seu VsCode**
4.  **Usando a extensão do Raspberry Pi Pico, siga os passos:**
5.  **Clean CMake: Para garantir que o projeto será compilado do zero**

6.  **Compile Project: Compilação dos binários**

7.  **Run Project [USB]: Compila e copia o firmware para a placa automaticamente**
