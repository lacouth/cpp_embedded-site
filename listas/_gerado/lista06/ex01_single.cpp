// ex01 — versao de arquivo unico (gerada automaticamente,
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
//   * o compilador aceitar pinMode/digitalWrite/digitalRead/millis/micros/Serial;
//   * os testes perguntarem "o que a classe fez com o pino?" e "o que ela
//     faria se o tempo andasse 500 ms?".
//
// Como funciona: cada pino guarda o ultimo nivel escrito e o ultimo modo
// configurado; millis() e micros() leem um relogio que so anda quando o teste
// manda (falso::avanca_tempo em ms, falso::avanca_micros em us). digitalRead devolve o que o teste pos no pino com
// falso::poe_nivel -- e assim que se "aperta um botao" sem botao.
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

// ---- Estado do "hardware" falso -------------------------------------------
namespace falso {

const int NUM_PINOS = 40;   // o ESP32 tem GPIO 0 a 39

struct Pino {
    int nivel = LOW;
    int modo = -1;          // -1: ninguem chamou pinMode ainda
    int escritas = 0;       // quantas vezes digitalWrite foi chamado neste pino
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
#ifndef EX01_H
#define EX01_H

struct BancoGpio {
    volatile uint32_t& saida_liga;      // W1TS: escrever 1 num bit LIGA aquele pino
    volatile uint32_t& saida_desliga;   // W1TC: escrever 1 num bit DESLIGA aquele pino
    volatile uint32_t& entrada;         // IN: o nivel atual de cada pino
};

// Um LED pelos registradores W1TS/W1TC. Mesma interface do Led do capitulo 1.
class LedRapido {
private:
    BancoGpio& banco_;
    const int pino_;
    const uint32_t mascara_;
    bool ligado_;

public:
    LedRapido(BancoGpio& banco, int pino)
        : banco_(banco), pino_(pino), mascara_(1u << (pino % 32)), ligado_(false) {}

    void inicia() { pinMode(pino_, OUTPUT); desliga(); }

    void liga();
    void desliga();
    void alterna();
    bool esta_ligado() const { return ligado_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void LedRapido::liga() {
    // complete aqui
}

void LedRapido::desliga() {
    // complete aqui
}

void LedRapido::alterna() {
    // complete aqui
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    falso::reinicia();
    volatile uint32_t w1ts = 0, w1tc = 0, in = 0;
    BancoGpio gpio0{w1ts, w1tc, in};
    LedRapido led(gpio0, 26);

    led.inicia();
    assert(falso::modo(26) == OUTPUT);
    assert(w1tc == (1u << 26));            // inicia() desliga
    assert(!led.esta_ligado());

    // = e nao |=: o valor que estava la antes nao pode sobrar
    w1ts = 0xFFFF;
    led.liga();
    assert(w1ts == (1u << 26));
    assert(led.esta_ligado());

    w1tc = 0xFFFF;
    led.desliga();
    assert(w1tc == (1u << 26));
    assert(!led.esta_ligado());

    // alterna usa liga/desliga
    w1ts = 0;
    w1tc = 0;
    led.alterna();
    assert(led.esta_ligado());
    assert(w1ts == (1u << 26) && w1tc == 0);
    led.alterna();
    assert(!led.esta_ligado());
    assert(w1tc == (1u << 26));

    // o Proxy nao usa digitalWrite: o pino 26 do Arduino falso nunca foi escrito
    assert(falso::escritas(26) == 0);

    // pino do banco 1: 33 e o bit 1
    w1ts = 0;
    volatile uint32_t w1ts1 = 0, w1tc1 = 0, in1 = 0;
    BancoGpio gpio1{w1ts1, w1tc1, in1};
    LedRapido outro(gpio1, 33);
    outro.inicia();
    outro.liga();
    assert(w1ts1 == 0x2);
    assert(w1ts == 0);                     // o banco 0 nao foi tocado

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
