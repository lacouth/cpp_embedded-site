// ex08 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX08_H
#define EX08_H

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

// A bomba do exercicio 5, com um pedido novo: depois de desligar, descansar
// pelo menos descanso_ms antes de partir de novo. Os testes foram escritos
// ANTES deste codigo.
class ControleBomba {
private:
    const ISensor& nivel_;
    ISaida& bomba_;
    const IRelogio& relogio_;
    const int32_t liga_abaixo_;
    const int32_t desliga_acima_;
    const uint32_t descanso_ms_;
    bool bombeando_;
    bool ja_desligou_;          // a primeira partida nao espera descanso nenhum
    uint32_t desligou_em_ms_;

public:
    ControleBomba(const ISensor& nivel, ISaida& bomba, const IRelogio& relogio,
                  int32_t liga_abaixo, int32_t desliga_acima, uint32_t descanso_ms)
        : nivel_(nivel), bomba_(bomba), relogio_(relogio), liga_abaixo_(liga_abaixo),
          desliga_acima_(desliga_acima), descanso_ms_(descanso_ms), bombeando_(false),
          ja_desligou_(false), desligou_em_ms_(0) {}

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
// A especificacao, escrita antes do codigo.

void primeira_partida_nao_espera() {
    RelogioFalso relogio;
    SensorProgramado nivel;
    SaidaEspia bomba(relogio);
    ControleBomba controle(nivel, bomba, relogio, 500, 1800, 60000);
    nivel.poe(400);
    controle.atualiza();
    assert(bomba.ligada());
}

void descansa_depois_de_desligar() {
    RelogioFalso relogio;
    SensorProgramado nivel;
    SaidaEspia bomba(relogio);
    ControleBomba controle(nivel, bomba, relogio, 500, 1800, 60000);
    nivel.poe(400);
    controle.atualiza();                     // liga em 0
    relogio.avanca(30000);
    nivel.poe(1900);
    controle.atualiza();                     // desliga em 30 s
    nivel.poe(100);                          // esvaziou de repente
    relogio.avanca(59999);
    controle.atualiza();                     // 89,999 s: ainda descansando
    assert(!bomba.ligada());
    relogio.avanca(1);
    controle.atualiza();                     // 90 s: pode
    assert(bomba.ligada());
    assert(bomba.quantos() == 3);
    assert(bomba.evento(2).em_ms == 90000);
}

void descanso_conta_tambem_depois_de_leitura_invalida() {
    RelogioFalso relogio;
    SensorProgramado nivel;
    SaidaEspia bomba(relogio);
    ControleBomba controle(nivel, bomba, relogio, 500, 1800, 60000);
    nivel.poe(400);
    controle.atualiza();
    nivel.poe_invalida();
    controle.atualiza();                     // desligou pela leitura invalida em 0
    nivel.poe(400);
    relogio.avanca(1000);
    controle.atualiza();
    assert(!bomba.ligada());                 // descansando
    relogio.avanca(59000);
    controle.atualiza();
    assert(bomba.ligada());
}

void descanso_cruza_a_volta_do_relogio() {
    RelogioFalso relogio;
    relogio.avanca(4294967295u - 20000);     // 20 s antes do relogio dar a volta
    SensorProgramado nivel;
    SaidaEspia bomba(relogio);
    ControleBomba controle(nivel, bomba, relogio, 500, 1800, 60000);
    nivel.poe(400);
    controle.atualiza();
    nivel.poe(1900);
    controle.atualiza();                     // desliga perto do fim
    nivel.poe(400);
    relogio.avanca(59999);                   // ja deu a volta
    controle.atualiza();
    assert(!bomba.ligada());
    relogio.avanca(1);
    controle.atualiza();
    assert(bomba.ligada());
}

void regras_antigas_continuam() {
    RelogioFalso relogio;
    SensorProgramado nivel;
    SaidaEspia bomba(relogio);
    ControleBomba controle(nivel, bomba, relogio, 500, 1800, 60000);
    nivel.poe(400);
    for (int i = 0; i < 5; i++) {
        controle.atualiza();
    }
    assert(bomba.quantos() == 1);            // liga uma vez so
    nivel.poe(1000);
    controle.atualiza();
    assert(bomba.ligada());                  // histerese
}

int main() {
    primeira_partida_nao_espera();
    descansa_depois_de_desligar();
    descanso_conta_tambem_depois_de_leitura_invalida();
    descanso_cruza_a_volta_do_relogio();
    regras_antigas_continuam();
    std::cout << "Todos os testes passaram!\n";
    return 0;
}
