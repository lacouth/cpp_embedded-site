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

class Temporizador {
private:
    unsigned long intervalo_ms_;
    unsigned long inicio_ms_;

public:
    explicit Temporizador(unsigned long intervalo_ms)
        : intervalo_ms_(intervalo_ms), inicio_ms_(0) {}

    void reinicia() { inicio_ms_ = millis(); }
    void define_intervalo(unsigned long ms) { intervalo_ms_ = ms; }

    bool expirou() {
        unsigned long agora = millis();
        if (agora - inicio_ms_ >= intervalo_ms_) {
            inicio_ms_ = agora;
            return true;
        }
        return false;
    }
};

class Pisca {
private:
    Led led_;
    Temporizador tempo_;

public:
    Pisca(int pino, unsigned long meio_periodo_ms)
        : led_(pino), tempo_(meio_periodo_ms) {}

    void inicia();
    void atualiza();

    bool esta_ligado() const { return led_.esta_ligado(); }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void Pisca::inicia() {
    // complete aqui
}

void Pisca::atualiza() {
    // complete aqui
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    falso::reinicia();
    falso::avanca_tempo(1000);           // a placa ja estava ligada ha 1 s
    Pisca p(26, 200);
    p.inicia();
    assert(falso::modo(26) == OUTPUT);
    assert(!p.esta_ligado());

    p.atualiza();                        // nada passou desde inicia
    assert(!p.esta_ligado());

    falso::avanca_tempo(200);
    p.atualiza();
    assert(p.esta_ligado());
    assert(falso::nivel(26) == HIGH);

    falso::avanca_tempo(100);
    p.atualiza();
    assert(p.esta_ligado());

    falso::avanca_tempo(100);
    p.atualiza();
    assert(!p.esta_ligado());

    // dois piscas independentes, de 10 em 10 ms durante 1 s
    falso::reinicia();
    Pisca lento(26, 500);
    Pisca rapido(25, 100);
    lento.inicia();
    rapido.inicia();
    int trocas_lento = 0;
    int trocas_rapido = 0;
    for (int i = 0; i < 100; i++) {
        bool antes_l = lento.esta_ligado();
        bool antes_r = rapido.esta_ligado();
        falso::avanca_tempo(10);
        lento.atualiza();
        rapido.atualiza();
        if (lento.esta_ligado() != antes_l) { trocas_lento++; }
        if (rapido.esta_ligado() != antes_r) { trocas_rapido++; }
    }
    assert(trocas_lento == 2);
    assert(trocas_rapido == 10);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
