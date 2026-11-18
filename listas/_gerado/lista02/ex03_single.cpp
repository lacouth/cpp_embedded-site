// ex03 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>

// ============================================================
// NAO ALTERE — Arduino de mentira (no Wokwi, use #include <Arduino.h>)
// ============================================================
// arduino_falso.h -- um <Arduino.h> de mentira, para rodar no computador (ou num
// compilador online) o mesmo codigo de classe que roda no ESP32 do Wokwi.
//
// Nao e um simulador. E so o suficiente para:
//   * o compilador aceitar pinMode/digitalWrite/digitalRead/millis/Serial;
//   * os testes perguntarem "o que a classe fez com o pino?" e "o que ela
//     faria se o tempo andasse 500 ms?".
//
// Como funciona: cada pino guarda o ultimo nivel escrito e o ultimo modo
// configurado; millis() devolve um relogio que so anda quando o teste manda
// (falso::avanca_tempo). digitalRead devolve o que o teste pos no pino com
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

inline unsigned long& relogio() {
    static unsigned long agora_ms = 0;
    return agora_ms;
}

// Para os testes: espiar e mexer no mundo externo.
inline int nivel(int numero) { return pino(numero).nivel; }
inline int modo(int numero) { return pino(numero).modo; }
inline int escritas(int numero) { return pino(numero).escritas; }
inline void poe_nivel(int numero, int nivel) { pino(numero).nivel = nivel; }
inline void avanca_tempo(unsigned long ms) { relogio() += ms; }

// Volta tudo ao estado de "placa recem-ligada".
inline void reinicia() {
    for (int i = 0; i < NUM_PINOS; i++) {
        pino(i) = Pino{};
    }
    relogio() = 0;
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

inline unsigned long millis() {
    return falso::relogio();
}

inline void delay(unsigned long ms) {
    falso::relogio() += ms;
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

class Botao {
private:
    const int pino_;
    bool anterior_;

public:
    explicit Botao(int pino) : pino_(pino), anterior_(false) {}

    void inicia() { pinMode(pino_, INPUT_PULLUP); }
    bool esta_apertado() const { return digitalRead(pino_) == LOW; }

    bool foi_apertado();
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
bool Botao::foi_apertado() {
    // complete aqui
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    falso::reinicia();
    Botao b(32);
    b.inicia();

    assert(b.foi_apertado() == false);         // solto

    falso::poe_nivel(32, LOW);
    assert(b.foi_apertado() == true);          // acabou de apertar
    assert(b.foi_apertado() == false);         // continua apertado
    assert(b.foi_apertado() == false);

    falso::poe_nivel(32, HIGH);
    assert(b.foi_apertado() == false);         // soltar nao e clique

    falso::poe_nivel(32, LOW);
    assert(b.foi_apertado() == true);          // segundo clique

    // conta 3 cliques numa sequencia mais longa
    falso::reinicia();
    Botao c(33);
    c.inicia();
    int dedo[10] = {0, 1, 1, 0, 0, 1, 0, 1, 1, 1};
    int cliques = 0;
    for (int i = 0; i < 10; i++) {
        falso::poe_nivel(33, dedo[i] ? LOW : HIGH);
        if (c.foi_apertado()) {
            cliques++;
        }
    }
    assert(cliques == 3);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
