// ex04 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX04_H
#define EX04_H

class ISaida {
public:
    virtual ~ISaida() = default;
    virtual void liga() = 0;
    virtual void desliga() = 0;
    virtual bool ligada() const = 0;
};

class IRelogio {
public:
    virtual ~IRelogio() = default;
    virtual uint32_t agora_ms() const = 0;
};

// FAKE (pronto).
class RelogioFalso : public IRelogio {
private:
    uint32_t agora_ms_;

public:
    RelogioFalso() : agora_ms_(0) {}
    void avanca(uint32_t ms) { agora_ms_ += ms; }
    uint32_t agora_ms() const override { return agora_ms_; }
};

// SPY: funciona como saida e anota cada chamada, com a hora.
class SaidaEspia : public ISaida {
public:
    struct Evento {
        bool ligou;
        uint32_t em_ms;
    };

private:
    static const int MAX = 16;
    const IRelogio& relogio_;
    Evento eventos_[MAX];
    int quantos_;
    bool ligada_;

public:
    explicit SaidaEspia(const IRelogio& relogio)
        : relogio_(relogio), eventos_{}, quantos_(0), ligada_(false) {}

    void liga() override;
    void desliga() override;
    bool ligada() const override;

    int quantos() const;
    Evento evento(int i) const;
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void SaidaEspia::liga() {
    // complete aqui
}

void SaidaEspia::desliga() {
    // complete aqui
}

bool SaidaEspia::ligada() const {
    // complete aqui
    return false;
}

int SaidaEspia::quantos() const {
    // complete aqui
    return 0;
}

SaidaEspia::Evento SaidaEspia::evento(int i) const {
    // complete aqui
    (void)i;
    return {false, 0};
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
// A "logica" so conhece ISaida.
void pisca_duas_vezes(ISaida& s, RelogioFalso& r) {
    for (int i = 0; i < 2; i++) {
        s.liga();
        r.avanca(100);
        s.desliga();
        r.avanca(400);
    }
}

int main() {
    RelogioFalso relogio;
    SaidaEspia led(relogio);
    assert(led.quantos() == 0);
    assert(!led.ligada());

    pisca_duas_vezes(led, relogio);
    assert(led.quantos() == 4);
    assert(led.evento(0).ligou && led.evento(0).em_ms == 0);
    assert(!led.evento(1).ligou && led.evento(1).em_ms == 100);
    assert(led.evento(2).ligou && led.evento(2).em_ms == 500);
    assert(!led.evento(3).ligou && led.evento(3).em_ms == 600);
    assert(!led.ligada());

    // anota chamadas, nao mudancas: ligar o que ja esta ligado tambem conta
    SaidaEspia bomba(relogio);
    bomba.liga();
    bomba.liga();
    assert(bomba.quantos() == 2);
    assert(bomba.ligada());

    // depois de 16 eventos, continua funcionando mas nao anota
    SaidaEspia muitos(relogio);
    for (int i = 0; i < 10; i++) {
        muitos.liga();
        muitos.desliga();
    }
    assert(muitos.quantos() == 16);
    muitos.liga();
    assert(muitos.ligada());
    assert(muitos.quantos() == 16);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
