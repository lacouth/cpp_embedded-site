// ex01 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX01_H
#define EX01_H

// Cada funcao devolve v com o bit n modificado; os outros bits ficam como estavam.
// n vai de 0 (bit menos significativo) a 31.
uint32_t liga_bit(uint32_t v, unsigned n);
uint32_t desliga_bit(uint32_t v, unsigned n);
uint32_t inverte_bit(uint32_t v, unsigned n);
bool testa_bit(uint32_t v, unsigned n);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
uint32_t liga_bit(uint32_t v, unsigned n) {
    // complete aqui
    (void)n;
    return v;
}

uint32_t desliga_bit(uint32_t v, unsigned n) {
    // complete aqui
    (void)n;
    return v;
}

uint32_t inverte_bit(uint32_t v, unsigned n) {
    // complete aqui
    (void)n;
    return v;
}

bool testa_bit(uint32_t v, unsigned n) {
    // complete aqui
    (void)v;
    (void)n;
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    // liga
    assert(liga_bit(0x00, 3) == 0x08);
    assert(liga_bit(0x08, 3) == 0x08);              // ja ligado: nao muda
    assert(liga_bit(0xF0, 0) == 0xF1);              // os outros bits ficam
    assert(liga_bit(0, 31) == 0x80000000u);

    // desliga
    assert(desliga_bit(0xFF, 0) == 0xFE);
    assert(desliga_bit(0xF0, 0) == 0xF0);           // ja desligado: nao muda
    assert(desliga_bit(0xFFFFFFFFu, 31) == 0x7FFFFFFFu);

    // inverte
    assert(inverte_bit(0x0F, 7) == 0x8F);
    assert(inverte_bit(0x8F, 7) == 0x0F);
    assert(inverte_bit(inverte_bit(0x1234u, 5), 5) == 0x1234u);

    // testa
    assert(testa_bit(0x80000000u, 31));
    assert(!testa_bit(0x7FFFFFFFu, 31));
    assert(testa_bit(0x04, 2));
    assert(!testa_bit(0x04, 1));
    assert(!testa_bit(0x04, 3));

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
