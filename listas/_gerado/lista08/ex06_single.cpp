// ex06 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX06_H
#define EX06_H

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

// A bomba completa do capitulo: com tempo maximo ligada, alarme e rearme.
class ControleBomba {
public:
    enum class Estado { Parada, Bombeando, Bloqueada };

    struct Config {
        int32_t liga_abaixo;
        int32_t desliga_acima;
        uint32_t tempo_max_ms;
    };

private:
    const ISensor& nivel_;
    ISaida& bomba_;
    ISaida& alarme_;
    const IRelogio& relogio_;
    const Config config_;
    Estado estado_;
    uint32_t ligou_em_ms_;

public:
    ControleBomba(const ISensor& nivel, ISaida& bomba, ISaida& alarme,
                  const IRelogio& relogio, Config config)
        : nivel_(nivel), bomba_(bomba), alarme_(alarme), relogio_(relogio),
          config_(config), estado_(Estado::Parada), ligou_em_ms_(0) {}

    void atualiza();
    void rearma();
    Estado estado() const { return estado_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void ControleBomba::atualiza() {
    // complete aqui
}

void ControleBomba::rearma() {
    // complete aqui
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
const ControleBomba::Config CONFIG = {500, 1800, 10u * 60 * 1000};

int main() {
    RelogioFalso relogio;
    SensorProgramado nivel;
    SaidaEspia bomba(relogio), alarme(relogio);
    ControleBomba controle(nivel, bomba, alarme, relogio, CONFIG);

    // encher normalmente
    relogio.avanca(5000);
    nivel.poe(400);
    controle.atualiza();
    assert(controle.estado() == ControleBomba::Estado::Bombeando);
    relogio.avanca(120000);
    nivel.poe(1800);
    controle.atualiza();
    assert(controle.estado() == ControleBomba::Estado::Parada);
    assert(bomba.quantos() == 2 && bomba.evento(1).em_ms == 125000);
    assert(alarme.quantos() == 0);

    // bomba seca: liga em t0 e o nivel nao sobe
    nivel.poe(300);
    controle.atualiza();
    uint32_t t0 = relogio.agora_ms();
    relogio.avanca(10u * 60 * 1000 - 1);
    controle.atualiza();
    assert(bomba.ligada());                          // 9 min 59,999 s
    relogio.avanca(1);
    controle.atualiza();                             // 10 min
    assert(!bomba.ligada());
    assert(alarme.ligada());
    assert(controle.estado() == ControleBomba::Estado::Bloqueada);
    assert(bomba.evento(3).em_ms - t0 == 600000);

    // bloqueada: nada religa
    nivel.poe(100);
    relogio.avanca(60000);
    controle.atualiza();
    assert(!bomba.ligada());
    assert(bomba.quantos() == 4);

    // rearma: alarme apaga, e o proximo atualiza() tenta de novo
    controle.rearma();
    assert(!alarme.ligada());
    assert(controle.estado() == ControleBomba::Estado::Parada);
    controle.atualiza();
    assert(bomba.ligada());

    // rearma() fora de Bloqueada nao faz nada
    controle.rearma();
    assert(controle.estado() == ControleBomba::Estado::Bombeando);
    assert(alarme.quantos() == 2);

    // invalida com a bomba ligada: desliga sem alarme
    nivel.poe_invalida();
    controle.atualiza();
    assert(!bomba.ligada());
    assert(!alarme.ligada());
    assert(controle.estado() == ControleBomba::Estado::Parada);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
