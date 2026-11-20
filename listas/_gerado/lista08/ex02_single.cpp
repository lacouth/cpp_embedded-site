// ex02 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>
#include <cstdint>

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX02_H
#define EX02_H

class IRelogio {
public:
    virtual ~IRelogio() = default;
    virtual uint32_t agora_ms() const = 0;
};

// FAKE: um relogio que so anda quando o teste manda.
class RelogioFalso : public IRelogio {
private:
    uint32_t agora_ms_;

public:
    RelogioFalso() : agora_ms_(0) {}

    void avanca(uint32_t ms);
    uint32_t agora_ms() const override;
};

// O Temporizador do capitulo 3, com o relogio injetado.
class Temporizador {
private:
    const IRelogio& relogio_;
    const uint32_t intervalo_ms_;
    uint32_t inicio_ms_;

public:
    Temporizador(const IRelogio& relogio, uint32_t intervalo_ms)
        : relogio_(relogio), intervalo_ms_(intervalo_ms), inicio_ms_(0) {}

    void reinicia();
    bool expirou();
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void RelogioFalso::avanca(uint32_t ms) {
    // complete aqui
    (void)ms;
}

uint32_t RelogioFalso::agora_ms() const {
    // complete aqui
    return 0;
}

void Temporizador::reinicia() {
    // complete aqui
}

bool Temporizador::expirou() {
    // complete aqui
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    RelogioFalso relogio;
    assert(relogio.agora_ms() == 0);
    relogio.avanca(250);
    relogio.avanca(250);
    assert(relogio.agora_ms() == 500);

    Temporizador meio_segundo(relogio, 500);
    meio_segundo.reinicia();                  // conta a partir de 500
    relogio.avanca(499);
    assert(!meio_segundo.expirou());
    relogio.avanca(1);
    assert(meio_segundo.expirou());
    assert(!meio_segundo.expirou());          // avisou uma vez so
    relogio.avanca(500);
    assert(meio_segundo.expirou());           // periodico

    // dez minutos sem esperar dez minutos
    Temporizador dez_minutos(relogio, 10u * 60 * 1000);
    dez_minutos.reinicia();
    relogio.avanca(10u * 60 * 1000 - 1);
    assert(!dez_minutos.expirou());
    relogio.avanca(1);
    assert(dez_minutos.expirou());

    // o relogio de 32 bits da a volta no meio do intervalo
    RelogioFalso perto_do_fim;
    perto_do_fim.avanca(4294967000u);         // faltam 296 ms para 2^32
    Temporizador t(perto_do_fim, 1000);
    t.reinicia();
    perto_do_fim.avanca(999);                 // agora_ms() = 703
    assert(perto_do_fim.agora_ms() == 703);
    assert(!t.expirou());
    perto_do_fim.avanca(1);
    assert(t.expirou());

    // dois temporizadores no mesmo relogio, cada um no seu ritmo
    RelogioFalso r;
    Temporizador rapido(r, 100), lento(r, 300);
    rapido.reinicia();
    lento.reinicia();
    int rapidos = 0, lentos = 0;
    for (int i = 0; i < 600; i++) {
        r.avanca(1);
        if (rapido.expirou()) { rapidos++; }
        if (lento.expirou()) { lentos++; }
    }
    assert(rapidos == 6 && lentos == 2);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
