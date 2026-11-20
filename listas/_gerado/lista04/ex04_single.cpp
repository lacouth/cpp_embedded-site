// ex04 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX04_H
#define EX04_H

class Led {
private:
    const int pino_;
    bool ligado_;

public:
    explicit Led(int pino) : pino_(pino), ligado_(false) {}
    void inicia() { pinMode(pino_, OUTPUT); desliga(); }
    void liga() { digitalWrite(pino_, HIGH); ligado_ = true; }
    void desliga() { digitalWrite(pino_, LOW); ligado_ = false; }
    void alterna() { if (ligado_) { desliga(); } else { liga(); } }
    bool esta_ligado() const { return ligado_; }
};

const unsigned long TEMPO_VERDE_MS = 5000;
const unsigned long TEMPO_VERDE_MINIMO_MS = 1000;
const unsigned long TEMPO_AMARELO_MS = 1000;
const unsigned long TEMPO_VERMELHO_MS = 3000;

enum class Fase { Verde, Amarelo, Vermelho };

class Semaforo {
private:
    Led verde_;
    Led amarelo_;
    Led vermelho_;
    Fase fase_;
    unsigned long entrou_ms_;
    bool pedido_;

    void entra(Fase nova) {
        fase_ = nova;
        entrou_ms_ = millis();
        verde_.desliga();
        amarelo_.desliga();
        vermelho_.desliga();
        switch (nova) {
            case Fase::Verde:    verde_.liga();    break;
            case Fase::Amarelo:  amarelo_.liga();  break;
            case Fase::Vermelho: vermelho_.liga(); pedido_ = false; break;
        }
    }

public:
    Semaforo(int pino_verde, int pino_amarelo, int pino_vermelho)
        : verde_(pino_verde), amarelo_(pino_amarelo), vermelho_(pino_vermelho),
          fase_(Fase::Verde), entrou_ms_(0), pedido_(false) {}

    void inicia() {
        verde_.inicia();
        amarelo_.inicia();
        vermelho_.inicia();
        entra(Fase::Verde);
    }

    void pede_travessia() { pedido_ = true; }

    void atualiza();

    Fase fase() const { return fase_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void Semaforo::atualiza() {
    // complete aqui
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
void roda(Semaforo& s, unsigned long ms) {
    for (unsigned long t = 0; t < ms; t += 100) {
        falso::avanca_tempo(100);
        s.atualiza();
    }
}

int main() {
    falso::reinicia();
    Semaforo s(25, 27, 26);
    s.inicia();

    // sem pedido: comportamento so de tempo
    roda(s, 4900);
    assert(s.fase() == Fase::Verde);
    roda(s, 100);
    assert(s.fase() == Fase::Amarelo);
    roda(s, 4000);                               // amarelo + vermelho
    assert(s.fase() == Fase::Verde);             // verde em t=9000

    // pedido cedo: respeita o minimo de 1 s
    roda(s, 300);
    s.pede_travessia();
    s.atualiza();
    assert(s.fase() == Fase::Verde);
    roda(s, 600);                                // 900 ms de verde
    assert(s.fase() == Fase::Verde);
    roda(s, 100);                                // 1000 ms de verde
    assert(s.fase() == Fase::Amarelo);

    // pedido durante o amarelo: esquecido ao entrar no vermelho
    s.pede_travessia();
    roda(s, 1000);
    assert(s.fase() == Fase::Vermelho);
    roda(s, 3000);
    assert(s.fase() == Fase::Verde);
    roda(s, 4900);                               // verde inteiro, sem pedido
    assert(s.fase() == Fase::Verde);
    roda(s, 100);
    assert(s.fase() == Fase::Amarelo);

    // pedido durante o vermelho: guardado para o proximo verde
    roda(s, 1000);
    assert(s.fase() == Fase::Vermelho);
    s.pede_travessia();
    roda(s, 3000);
    assert(s.fase() == Fase::Verde);
    roda(s, 1000);                               // so o minimo de 1 s
    assert(s.fase() == Fase::Amarelo);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
