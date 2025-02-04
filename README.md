Visão Geral
Este repositório contém um programa escrito em C para o Raspberry Pi Pico W, utilizando o Pico SDK. O código implementa:

One Shot (temporizador de disparo com botão) - Um sistema em que, ao pressionar um botão, três LEDs acendem simultaneamente e se apagam um a um a cada 3 segundos.
O código foi testado com o simulador Wokwi e a ferramenta educacional BitDogLab.

Como Funciona
Quando o botão é pressionado, os três LEDs acendem simultaneamente. A cada 3 segundos, um LED é apagado na seguinte sequência:

O LED Azul é desligado primeiro.
O LED Vermelho é desligado em seguida.
O LED Verde é o último a ser desligado.
O botão só pode ser pressionado novamente depois que todos os LEDs estiverem apagados. Um sistema de debounce é utilizado para evitar leituras incorretas do botão.

Vídeo do resultado final na placa física: https://drive.google.com/file/d/1r-H9kxK9NMPltdlSgtHPChQ7WoNDlPfF/view?usp=sharing
