// ex03 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>
#include <cstdint>

// ============================================================
// NAO ALTERE — Arduino de mentira (no Wokwi, use #include <Arduino.h>)
// ============================================================
// arduino_falso.h -- um <Arduino.h> de mentira, para rodar no computador (ou num
// compilador online) o mesmo codigo de classe que roda no ESP32 do Wokwi.
//
// Nao e um simulador. E so o suficiente para:
//   * o compilador aceitar pinMode/digitalWrite/digitalRead/millis/micros/Serial,
//     analogRead e o PWM do ESP32 (ledcAttach/ledcWrite/ledcWriteTone);
//   * os testes perguntarem "o que a classe fez com o pino?" e "o que ela
//     faria se o tempo andasse 500 ms?".
//
// Como funciona: cada pino guarda o ultimo nivel escrito e o ultimo modo
// configurado; millis() e micros() leem um relogio que so anda quando o teste
// manda (falso::avanca_tempo em ms, falso::avanca_micros em us). digitalRead devolve o que o teste pos no pino com
// falso::poe_nivel -- e assim que se "aperta um botao" sem botao. analogRead devolve
// o que o teste pos com falso::poe_analogico, e o PWM guarda frequencia, bits e duty.
//
// No Wokwi, troque  #include "arduino_falso.h"  por  #include <Arduino.h>
// e o resto do codigo continua igual.
#ifndef ARDUINO_FALSO_H
#define ARDUINO_FALSO_H

// ---- Constantes com os mesmos nomes do Arduino ----------------------------
const int LOW = 0;
const int HIGH = 1;

const int INPUT = 0;
const int OUTPUT = 1;
const int INPUT_PULLUP = 2;

// Atenuacao do ADC, com os nomes do core 3.x (o padrao do chip e ADC_11db).
enum adc_attenuation_t { ADC_0db, ADC_2_5db, ADC_6db, ADC_11db, ADC_ATTENDB_MAX };

// ---- Estado do "hardware" falso -------------------------------------------
namespace falso {

const int NUM_PINOS = 40;   // o ESP32 tem GPIO 0 a 39

struct Pino {
    int nivel = LOW;
    int modo = -1;          // -1: ninguem chamou pinMode ainda
    int escritas = 0;       // quantas vezes digitalWrite foi chamado neste pino
    int analogico = 0;      // o que analogRead devolve (0 a 4095, como o ADC de 12 bits)
    int atenuacao = -1;     // -1: ninguem chamou analogSetPinAttenuation
    bool pwm = false;       // ledcAttach foi chamado?
    uint32_t pwm_freq = 0;  // Hz
    int pwm_bits = 0;       // resolucao do duty
    uint32_t pwm_duty = 0;  // ultimo valor de ledcWrite
};

inline Pino& pino(int numero) {
    static Pino pinos[NUM_PINOS];
    return pinos[numero % NUM_PINOS];
}

// O relogio guarda MICROSSEGUNDOS; millis() e so ele dividido por 1000.
inline unsigned long long& relogio_us() {
    static unsigned long long agora_us = 0;
    return agora_us;
}

// Para os testes: espiar e mexer no mundo externo.
inline int nivel(int numero) { return pino(numero).nivel; }
inline int modo(int numero) { return pino(numero).modo; }
inline int escritas(int numero) { return pino(numero).escritas; }
inline void poe_nivel(int numero, int nivel) { pino(numero).nivel = nivel; }
inline void poe_analogico(int numero, int bruto) { pino(numero).analogico = bruto; }
inline int atenuacao(int numero) { return pino(numero).atenuacao; }
inline bool pwm(int numero) { return pino(numero).pwm; }
inline uint32_t frequencia(int numero) { return pino(numero).pwm_freq; }
inline int bits_pwm(int numero) { return pino(numero).pwm_bits; }
inline uint32_t duty(int numero) { return pino(numero).pwm_duty; }
inline void avanca_tempo(unsigned long ms) { relogio_us() += 1000ull * ms; }
inline void avanca_micros(unsigned long us) { relogio_us() += us; }

// Volta tudo ao estado de "placa recem-ligada".
inline void reinicia() {
    for (int i = 0; i < NUM_PINOS; i++) {
        pino(i) = Pino{};
    }
    relogio_us() = 0;
}

}  // namespace falso

// ---- As funcoes do Arduino --------------------------------------------------
inline void pinMode(int numero, int modo) {
    falso::pino(numero).modo = modo;
    // Com pull-up interno, um pino de entrada solto le HIGH -- igual ao real.
    if (modo == INPUT_PULLUP) {
        falso::pino(numero).nivel = HIGH;
    }
}

inline void digitalWrite(int numero, int nivel) {
    falso::pino(numero).nivel = nivel;
    falso::pino(numero).escritas++;
}

inline int digitalRead(int numero) {
    return falso::pino(numero).nivel;
}

// Como no ESP32, os dois sao de 32 bits e dao a volta (millis em ~49 dias,
// micros em ~71 minutos).
inline unsigned long millis() {
    return static_cast<uint32_t>(falso::relogio_us() / 1000);
}

inline unsigned long micros() {
    return static_cast<uint32_t>(falso::relogio_us());
}

inline void delay(unsigned long ms) {
    falso::relogio_us() += 1000ull * ms;
}

inline void delayMicroseconds(unsigned int us) {
    falso::relogio_us() += us;
}

// ---- ADC e PWM (LEDC) do ESP32, com as assinaturas do core 3.x -------------
inline void analogSetPinAttenuation(int numero, adc_attenuation_t atenuacao) {
    falso::pino(numero).atenuacao = atenuacao;
}

inline uint16_t analogRead(int numero) {
    return static_cast<uint16_t>(falso::pino(numero).analogico);
}

inline bool ledcAttach(int numero, uint32_t freq, uint8_t bits) {
    falso::Pino& p = falso::pino(numero);
    p.pwm = true;
    p.pwm_freq = freq;
    p.pwm_bits = bits;
    p.pwm_duty = 0;
    return true;
}

inline bool ledcWrite(int numero, uint32_t duty) {
    falso::Pino& p = falso::pino(numero);
    if (!p.pwm) {
        return false;   // como no real: sem ledcAttach, nada acontece
    }
    p.pwm_duty = duty;
    return true;
}

// Como no core 3.x: frequencia 0 so zera o duty; senao o pino passa a 10 bits
// e duty 0x1FF (quase 50%), uma onda quadrada na frequencia pedida.
inline uint32_t ledcWriteTone(int numero, uint32_t freq) {
    falso::Pino& p = falso::pino(numero);
    if (!p.pwm) {
        return 0;
    }
    if (freq == 0) {
        p.pwm_duty = 0;
        return 0;
    }
    p.pwm_freq = freq;
    p.pwm_bits = 10;
    p.pwm_duty = 0x1FF;
    return freq;
}

// ---- Serial: imprime no terminal ------------------------------------------
// Como no Arduino, bool sai como 1/0.
struct SerialFalso {
    void begin(unsigned long) {}

    template <typename T>
    void print(const T& valor) { std::cout << valor; }

    template <typename T>
    void println(const T& valor) { std::cout << valor << "\n"; }

    void println() { std::cout << "\n"; }
};

inline SerialFalso Serial;

#endif

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX03_H
#define EX03_H

struct BancoGpio {
    volatile uint32_t& saida_liga;      // W1TS: escrever 1 num bit LIGA aquele pino
    volatile uint32_t& saida_desliga;   // W1TC: escrever 1 num bit DESLIGA aquele pino
    volatile uint32_t& entrada;         // IN: o nivel atual de cada pino
};

// Tres LEDs do mesmo banco, mudados juntos. bit 0 do valor -> primeiro pino.
class GrupoDeLeds {
private:
    BancoGpio& banco_;
    const int pinos_[3];
    uint32_t mascaras_[3];      // o bit de cada pino no banco
    uint32_t mascara_total_;    // os tres juntos
    unsigned valor_;

public:
    GrupoDeLeds(BancoGpio& banco, int p0, int p1, int p2);

    void inicia() {
        for (int i = 0; i < 3; i++) {
            pinMode(pinos_[i], OUTPUT);
        }
        mostra(0);
    }

    void mostra(unsigned valor);

    unsigned valor() const { return valor_; }
    uint32_t mascara_total() const { return mascara_total_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
GrupoDeLeds::GrupoDeLeds(BancoGpio& banco, int p0, int p1, int p2)
    : banco_(banco), pinos_{p0, p1, p2}, mascaras_{0, 0, 0}, mascara_total_(0), valor_(0) {
    // complete aqui: preencha mascaras_ e mascara_total_
}

void GrupoDeLeds::mostra(unsigned valor) {
    // complete aqui
    (void)valor;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    falso::reinicia();
    volatile uint32_t w1ts = 0, w1tc = 0, in = 0;
    BancoGpio gpio0{w1ts, w1tc, in};
    GrupoDeLeds barra(gpio0, 25, 26, 27);

    const uint32_t B25 = 1u << 25, B26 = 1u << 26, B27 = 1u << 27;
    assert(barra.mascara_total() == (B25 | B26 | B27));

    barra.inicia();
    assert(falso::modo(26) == OUTPUT);
    assert(w1tc == (B25 | B26 | B27));      // mostra(0): apaga os tres
    assert(w1ts == 0);

    barra.mostra(0b101);
    assert(w1ts == (B25 | B27));
    assert(w1tc == B26);
    assert(barra.valor() == 0b101);

    barra.mostra(0b111);
    assert(w1ts == (B25 | B26 | B27));
    assert(w1tc == 0);

    barra.mostra(0b010);
    assert(w1ts == B26);
    assert(w1tc == (B25 | B27));

    // so os 3 bits de baixo contam
    barra.mostra(0b1001);
    assert(w1ts == B25);
    assert(w1tc == (B26 | B27));
    assert(barra.valor() == 0b001);

    // a ordem dos pinos no construtor e a ordem dos bits
    GrupoDeLeds invertida(gpio0, 27, 26, 25);
    invertida.mostra(0b001);
    assert(w1ts == B27);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
