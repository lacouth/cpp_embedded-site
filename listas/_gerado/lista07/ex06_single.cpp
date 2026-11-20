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

// Media das ultimas N leituras, num array fixo (sem heap).
class MediaMovel {
private:
    static const int N = 8;
    int32_t amostras_[N];
    int proxima_;       // onde vai a proxima leitura
    int quantas_;       // quantas leituras ha (no maximo N)
    int64_t soma_;      // soma das leituras guardadas

public:
    MediaMovel() : amostras_{}, proxima_(0), quantas_(0), soma_(0) {}

    void adiciona(int32_t x);
    int32_t media() const;
    bool cheia() const;
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void MediaMovel::adiciona(int32_t x) {
    // complete aqui
    (void)x;
}

int32_t MediaMovel::media() const {
    // complete aqui
    return 0;
}

bool MediaMovel::cheia() const {
    // complete aqui
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    MediaMovel m;
    assert(m.media() == 0);
    assert(!m.cheia());

    m.adiciona(10);
    assert(m.media() == 10);
    m.adiciona(13);
    assert(m.media() == 12);                 // 11,5

    // enche com 1..8 numa media nova
    MediaMovel n;
    for (int i = 1; i <= 8; i++) {
        n.adiciona(i);
    }
    assert(n.cheia());
    assert(n.media() == 5);                  // 36 / 8 = 4,5

    // a nona leitura tira a primeira (1): 2..8 e 100
    n.adiciona(100);
    assert(n.cheia());
    assert(n.media() == 17);                 // 135 / 8 = 16,875

    // depois de 8 leituras iguais, so sobram elas
    for (int i = 0; i < 8; i++) {
        n.adiciona(2048);
    }
    assert(n.media() == 2048);

    // negativos arredondam para longe do zero
    MediaMovel neg;
    neg.adiciona(-3);
    neg.adiciona(-4);
    assert(neg.media() == -4);               // -3,5
    neg.adiciona(-4);
    assert(neg.media() == -4);               // -3,67

    // media() e const
    const MediaMovel& ref = n;
    assert(ref.media() == 2048);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
