// ex06 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX06_H
#define EX06_H

// A do exercicio anterior, pronta.
class EstatisticaTempo {
private:
    unsigned long quantas_;
    unsigned long minimo_;
    unsigned long maximo_;
    unsigned long long soma_;

public:
    EstatisticaTempo() : quantas_(0), minimo_(0), maximo_(0), soma_(0) {}

    void adiciona(unsigned long us) {
        if (quantas_ == 0 || us < minimo_) { minimo_ = us; }
        if (quantas_ == 0 || us > maximo_) { maximo_ = us; }
        soma_ += us;
        quantas_++;
    }
    unsigned long quantas() const { return quantas_; }
    unsigned long minimo() const { return minimo_; }
    unsigned long maximo() const { return maximo_; }
    unsigned long media() const { return quantas_ == 0 ? 0 : static_cast<unsigned long>(soma_ / quantas_); }
    unsigned long jitter() const { return maximo_ - minimo_; }
};

// Mede o periodo de uma onda quadrada entre bordas de subida.
class MedidorDePeriodo {
private:
    const int pino_;
    int nivel_anterior_;
    bool teve_borda_;
    unsigned long ultima_borda_us_;
    unsigned long ultimo_periodo_us_;
    EstatisticaTempo estatistica_;

public:
    explicit MedidorDePeriodo(int pino)
        : pino_(pino), nivel_anterior_(LOW), teve_borda_(false),
          ultima_borda_us_(0), ultimo_periodo_us_(0) {}

    void inicia() {
        pinMode(pino_, INPUT);
        nivel_anterior_ = digitalRead(pino_);
    }

    // Chame a cada volta do loop().
    void atualiza();

    unsigned long ultimo_periodo_us() const;
    unsigned long frequencia_hz() const;
    const EstatisticaTempo& estatistica() const { return estatistica_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void MedidorDePeriodo::atualiza() {
    // complete aqui
}

unsigned long MedidorDePeriodo::ultimo_periodo_us() const {
    // complete aqui
    return 0;
}

unsigned long MedidorDePeriodo::frequencia_hz() const {
    // complete aqui
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
// Gera meia onda: poe o nivel no pino e chama atualiza() a cada 10 us
// durante 'duracao_us'.
void meia_onda(MedidorDePeriodo& m, int pino, int nivel, unsigned long duracao_us) {
    falso::poe_nivel(pino, nivel);
    for (unsigned long t = 0; t < duracao_us; t += 10) {
        m.atualiza();
        falso::avanca_micros(10);
    }
}

int main() {
    falso::reinicia();
    MedidorDePeriodo m(34);
    m.inicia();
    assert(falso::modo(34) == INPUT);
    assert(m.ultimo_periodo_us() == 0);
    assert(m.frequencia_hz() == 0);

    // a primeira borda nao gera periodo
    meia_onda(m, 34, LOW, 300);
    meia_onda(m, 34, HIGH, 500);
    assert(m.estatistica().quantas() == 0);
    assert(m.ultimo_periodo_us() == 0);

    // onda perfeita de 1000 us
    for (int i = 0; i < 5; i++) {
        meia_onda(m, 34, LOW, 500);
        meia_onda(m, 34, HIGH, 500);
    }
    assert(m.estatistica().quantas() == 5);
    assert(m.ultimo_periodo_us() == 1000);
    assert(m.frequencia_hz() == 1000);
    assert(m.estatistica().jitter() == 0);
    assert(m.estatistica().media() == 1000);

    // um periodo mais longo: aparece no ultimo e no jitter
    meia_onda(m, 34, LOW, 800);
    meia_onda(m, 34, HIGH, 500);
    assert(m.ultimo_periodo_us() == 1300);
    assert(m.frequencia_hz() == 769);      // 1000000 / 1300
    assert(m.estatistica().maximo() == 1300);
    assert(m.estatistica().minimo() == 1000);
    assert(m.estatistica().jitter() == 300);

    // ficar em HIGH muito tempo nao gera borda nova
    unsigned long antes = m.estatistica().quantas();
    meia_onda(m, 34, HIGH, 5000);
    assert(m.estatistica().quantas() == antes);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
