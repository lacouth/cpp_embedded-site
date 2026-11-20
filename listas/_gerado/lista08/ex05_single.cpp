// ex05 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX05_H
#define EX05_H

struct Leitura {
    int32_t valor;
    bool valida;
};

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual Leitura le() const = 0;
    virtual int32_t minimo() const = 0;
    virtual int32_t maximo() const = 0;
    virtual const char* unidade() const = 0;
};

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

// ---- Dubles prontos (os dos exercicios anteriores) ----

class SensorProgramado : public ISensor {
private:
    Leitura leitura_;

public:
    SensorProgramado() : leitura_{0, true} {}
    void poe(int32_t valor) { leitura_ = {valor, true}; }
    void poe_invalida() { leitura_ = {0, false}; }
    Leitura le() const override { return leitura_; }
    int32_t minimo() const override { return 0; }
    int32_t maximo() const override { return 2000; }
    const char* unidade() const override { return "mm"; }
};

class RelogioFalso : public IRelogio {
private:
    uint32_t agora_ms_;

public:
    RelogioFalso() : agora_ms_(0) {}
    void avanca(uint32_t ms) { agora_ms_ += ms; }
    uint32_t agora_ms() const override { return agora_ms_; }
};

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
    void liga() override {
        ligada_ = true;
        if (quantos_ < MAX) { eventos_[quantos_++] = {true, relogio_.agora_ms()}; }
    }
    void desliga() override {
        ligada_ = false;
        if (quantos_ < MAX) { eventos_[quantos_++] = {false, relogio_.agora_ms()}; }
    }
    bool ligada() const override { return ligada_; }
    int quantos() const { return quantos_; }
    Evento evento(int i) const { return eventos_[i]; }
};

// A bomba, primeira versao: liga no nivel baixo, desliga no alto (histerese),
// e desliga por seguranca se a leitura ficar invalida com ela ligada.
class ControleBomba {
private:
    const ISensor& nivel_;
    ISaida& bomba_;
    const int32_t liga_abaixo_;
    const int32_t desliga_acima_;
    bool bombeando_;

public:
    ControleBomba(const ISensor& nivel, ISaida& bomba, int32_t liga_abaixo, int32_t desliga_acima)
        : nivel_(nivel), bomba_(bomba), liga_abaixo_(liga_abaixo), desliga_acima_(desliga_acima),
          bombeando_(false) {}

    void atualiza();
    bool bombeando() const { return bombeando_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void ControleBomba::atualiza() {
    // complete aqui
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    RelogioFalso relogio;
    SensorProgramado nivel;
    SaidaEspia bomba(relogio);
    ControleBomba controle(nivel, bomba, 500, 1800);

    nivel.poe(1200);                     // no meio: nada
    controle.atualiza();
    assert(bomba.quantos() == 0);

    nivel.poe(500);                      // no limite: liga
    for (int i = 0; i < 10; i++) {
        controle.atualiza();
    }
    assert(bomba.ligada());
    assert(controle.bombeando());
    assert(bomba.quantos() == 1);        // uma vez so

    nivel.poe(1799);                     // subindo, ainda abaixo: continua
    controle.atualiza();
    assert(bomba.ligada());

    nivel.poe(1800);                     // chegou: desliga
    controle.atualiza();
    controle.atualiza();
    assert(!bomba.ligada());
    assert(bomba.quantos() == 2);

    nivel.poe(501);                      // descendo, ainda acima: nao religa
    controle.atualiza();
    assert(!bomba.ligada());

    // parada + invalida: nao liga
    nivel.poe_invalida();
    controle.atualiza();
    assert(!bomba.ligada());
    assert(bomba.quantos() == 2);

    // bombeando + invalida: desliga
    nivel.poe(300);
    controle.atualiza();
    assert(bomba.ligada());
    nivel.poe_invalida();
    controle.atualiza();
    assert(!bomba.ligada());
    assert(!controle.bombeando());
    assert(bomba.quantos() == 4);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
