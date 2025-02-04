#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_VERMELHO 13
#define LED_AZUL 12
#define LED_VERDE 11
#define BOTAO 5
#define INTERVALO 3000
#define TEMPO_DEBOUNCE_US 200000  

volatile int estado_led = 0;
volatile bool em_execucao = false;  // Impede novas execuções enquanto os LEDs estão apagando
volatile uint32_t ultimo_tempo_botao = 0;  // Armazena o último tempo do botão

// Callback para desligar LEDs em sequência
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    if (estado_led == 0) {
        gpio_put(LED_AZUL, 0);  // Desliga o vermelho
        printf("LED Azul desligado\n");
    } else if (estado_led == 1) {
        gpio_put(LED_VERMELHO, 0); // Desliga o amarelo
        printf("LED Vermelho desligado\n");
    } else if (estado_led == 2) {
        gpio_put(LED_VERDE, 0); // Desliga o verde
        printf("LED Verde desligado\n");
        em_execucao = false; // Libera para um novo acionamento
        return 0;
    }

    estado_led++;
    add_alarm_in_ms(INTERVALO, turn_off_callback, NULL, false);
    return 0;
}

// Callback do botão com debounce
void button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (!em_execucao && (current_time - ultimo_tempo_botao > TEMPO_DEBOUNCE_US)) {  
        ultimo_tempo_botao = current_time; // Atualiza tempo do último clique
        em_execucao = true;
        estado_led = 0;

        // Acende todos os LEDs
        gpio_put(LED_VERMELHO, 1);
        gpio_put(LED_AZUL, 1);
        gpio_put(LED_VERDE, 1);
        printf("Botão pressionado! LEDs acesos.\n");

        // Inicia temporizador para apagar os LEDs
        add_alarm_in_ms(INTERVALO, turn_off_callback, NULL, false);
    }
}

int main() {
    stdio_init_all();

    // Configura LEDs como saída
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // Configura botão como entrada com pull-up
    gpio_init(BOTAO);
    gpio_set_dir(BOTAO, GPIO_IN);
    gpio_pull_up(BOTAO);

    // Configura interrupção no botão (bordas de descida)
    gpio_set_irq_enabled_with_callback(BOTAO, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    while (true) {
        tight_loop_contents();  // Mantém o loop rodando sem sobrecarregar o processador
    }

    return 0;
}
