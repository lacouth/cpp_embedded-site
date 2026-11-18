// ex06 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX06_H
#define EX06_H

int conta_bordas(const int leituras[], int n);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
int conta_bordas(const int leituras[], int n) {
    // complete aqui
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    int a[6] = {0, 1, 1, 0, 1, 1};
    assert(conta_bordas(a, 6) == 2);

    int b[3] = {1, 1, 1};
    assert(conta_bordas(b, 3) == 1);

    int c[1] = {0};
    assert(conta_bordas(c, 0) == 0);
    assert(conta_bordas(c, 1) == 0);

    int d[8] = {1, 0, 1, 0, 1, 0, 1, 0};
    assert(conta_bordas(d, 8) == 4);

    int e[5] = {0, 0, 0, 0, 0};
    assert(conta_bordas(e, 5) == 0);

    // so olha as n primeiras
    assert(conta_bordas(d, 2) == 1);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
