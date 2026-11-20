// ex07 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX07_H
#define EX07_H

class ISaida {
public:
    virtual ~ISaida() = default;
    virtual void liga() = 0;
    virtual void desliga() = 0;
    virtual bool ligada() const = 0;
};

// MOCK: sabe de antemao a sequencia de chamadas e confere cada uma.
class SaidaMock : public ISaida {
private:
    static const int MAX = 8;
    bool esperado_[MAX];     // true = liga, false = desliga
    int esperados_;
    int recebidos_;
    int primeiro_erro_;      // -1: nenhum
    bool ligada_;

    void recebe(bool ligou);

public:
    SaidaMock() : esperado_{}, esperados_(0), recebidos_(0), primeiro_erro_(-1), ligada_(false) {}

    void espera_liga();
    void espera_desliga();

    void liga() override { recebe(true); }
    void desliga() override { recebe(false); }
    bool ligada() const override { return ligada_; }

    bool conferiu() const;
    int primeiro_erro() const { return primeiro_erro_; }
    int recebidos() const { return recebidos_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void SaidaMock::espera_liga() {
    // complete aqui
}

void SaidaMock::espera_desliga() {
    // complete aqui
}

void SaidaMock::recebe(bool ligou) {
    // complete aqui
    (void)ligou;
}

bool SaidaMock::conferiu() const {
    // complete aqui
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    // nada esperado, nada recebido: conferiu
    SaidaMock quieto;
    assert(quieto.conferiu());
    assert(quieto.primeiro_erro() == -1);

    // sequencia certa
    SaidaMock certo;
    certo.espera_liga();
    certo.espera_desliga();
    ISaida& s = certo;
    s.liga();
    assert(!certo.conferiu());               // falta o desliga
    s.desliga();
    assert(certo.conferiu());
    assert(!certo.ligada());

    // chamada a mais
    SaidaMock a_mais;
    a_mais.espera_liga();
    a_mais.liga();
    a_mais.liga();
    assert(!a_mais.conferiu());
    assert(a_mais.primeiro_erro() == 1);
    assert(a_mais.recebidos() == 2);

    // ordem trocada: o erro e na primeira chamada, e os seguintes nao mudam o registro
    SaidaMock trocado;
    trocado.espera_liga();
    trocado.espera_desliga();
    trocado.desliga();
    trocado.liga();
    assert(!trocado.conferiu());
    assert(trocado.primeiro_erro() == 0);
    assert(trocado.ligada());                // o estado segue as chamadas

    // chamada a menos
    SaidaMock a_menos;
    a_menos.espera_liga();
    a_menos.espera_desliga();
    a_menos.espera_liga();
    a_menos.liga();
    a_menos.desliga();
    assert(!a_menos.conferiu());
    assert(a_menos.primeiro_erro() == -1);   // nada errado, so incompleto

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
