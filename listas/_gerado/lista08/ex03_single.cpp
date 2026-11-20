// ex03 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX03_H
#define EX03_H

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

// STUB: devolve as leituras programadas, uma por chamada; depois, repete a ultima.
class SensorSequencia : public ISensor {
private:
    static const int MAX = 8;
    Leitura leituras_[MAX];
    int quantas_;
    mutable int proxima_;    // mutable: avanca dentro de le(), que e const
    mutable int lidas_;
    const int32_t minimo_;
    const int32_t maximo_;

public:
    SensorSequencia(int32_t minimo, int32_t maximo)
        : leituras_{}, quantas_(0), proxima_(0), lidas_(0), minimo_(minimo), maximo_(maximo) {}

    void adiciona(int32_t valor);
    void adiciona_invalida();
    int lidas() const { return lidas_; }

    Leitura le() const override;
    int32_t minimo() const override { return minimo_; }
    int32_t maximo() const override { return maximo_; }
    const char* unidade() const override { return "mm"; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void SensorSequencia::adiciona(int32_t valor) {
    // complete aqui
    (void)valor;
}

void SensorSequencia::adiciona_invalida() {
    // complete aqui
}

Leitura SensorSequencia::le() const {
    // complete aqui
    return {0, false};
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
// Quem usa so conhece a interface, e por referencia constante.
int32_t soma_validas(const ISensor& s, int vezes) {
    int32_t soma = 0;
    for (int i = 0; i < vezes; i++) {
        Leitura l = s.le();
        if (l.valida) {
            soma += l.valor;
        }
    }
    return soma;
}

int main() {
    SensorSequencia vazio(0, 2000);
    assert(!vazio.le().valida);
    assert(vazio.lidas() == 1);

    SensorSequencia s(0, 2000);
    s.adiciona(400);
    s.adiciona(300);
    s.adiciona_invalida();
    s.adiciona(1900);

    Leitura a = s.le();
    assert(a.valida && a.valor == 400);
    Leitura b = s.le();
    assert(b.valida && b.valor == 300);
    assert(!s.le().valida);
    Leitura d = s.le();
    assert(d.valida && d.valor == 1900);
    Leitura e = s.le();                       // acabou: repete a ultima
    assert(e.valida && e.valor == 1900);
    assert(s.lidas() == 5);

    // pela interface const: 1900 * 3
    assert(soma_validas(s, 3) == 5700);
    assert(s.lidas() == 8);

    // no maximo 8 leituras programadas; a nona e ignorada
    SensorSequencia cheio(0, 100);
    for (int i = 1; i <= 9; i++) {
        cheio.adiciona(i);
    }
    for (int i = 1; i <= 8; i++) {
        assert(cheio.le().valor == i);
    }
    assert(cheio.le().valor == 8);

    assert(s.minimo() == 0 && s.maximo() == 2000);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
