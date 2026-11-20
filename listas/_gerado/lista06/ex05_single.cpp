// ex05 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX05_H
#define EX05_H

// Estatisticas de um conjunto de medidas de tempo, sem guardar as medidas.
class EstatisticaTempo {
private:
    unsigned long quantas_;
    unsigned long minimo_;
    unsigned long maximo_;
    unsigned long long soma_;

public:
    EstatisticaTempo() : quantas_(0), minimo_(0), maximo_(0), soma_(0) {}

    void adiciona(unsigned long us);

    unsigned long quantas() const;
    unsigned long minimo() const;
    unsigned long maximo() const;
    unsigned long media() const;
    unsigned long jitter() const;
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void EstatisticaTempo::adiciona(unsigned long us) {
    // complete aqui
    (void)us;
}

unsigned long EstatisticaTempo::quantas() const {
    // complete aqui
    return 0;
}

unsigned long EstatisticaTempo::minimo() const {
    // complete aqui
    return 0;
}

unsigned long EstatisticaTempo::maximo() const {
    // complete aqui
    return 0;
}

unsigned long EstatisticaTempo::media() const {
    // complete aqui
    return 0;
}

unsigned long EstatisticaTempo::jitter() const {
    // complete aqui
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    EstatisticaTempo vazia;
    assert(vazia.quantas() == 0);
    assert(vazia.minimo() == 0);
    assert(vazia.maximo() == 0);
    assert(vazia.media() == 0);
    assert(vazia.jitter() == 0);

    EstatisticaTempo e;
    e.adiciona(100);
    assert(e.quantas() == 1);
    assert(e.minimo() == 100 && e.maximo() == 100);
    assert(e.media() == 100 && e.jitter() == 0);

    e.adiciona(130);
    e.adiciona(97);
    assert(e.quantas() == 3);
    assert(e.minimo() == 97);
    assert(e.maximo() == 130);
    assert(e.media() == 109);               // 327 / 3
    assert(e.jitter() == 33);

    // a primeira medida grande nao pode virar o minimo para sempre
    EstatisticaTempo f;
    f.adiciona(5000);
    f.adiciona(10);
    assert(f.minimo() == 10);
    assert(f.maximo() == 5000);

    // o caso do Serial.print: muitas rapidas e uma lenta
    EstatisticaTempo s;
    for (int i = 0; i < 49; i++) {
        s.adiciona(69);
    }
    s.adiciona(11136);
    assert(s.quantas() == 50);
    assert(s.media() == 290);               // (49*69 + 11136) / 50
    assert(s.jitter() == 11067);

    // soma que nao cabe em 32 bits
    EstatisticaTempo g;
    g.adiciona(4000000000u);
    g.adiciona(4000000000u);
    assert(g.media() == 4000000000u);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
