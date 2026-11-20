// ex02 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX02_H
#define EX02_H

// Quantos dos 32 bits de v valem 1.
int conta_bits_ligados(uint32_t v);

// true se exatamente um bit de v esta ligado. Zero devolve false.
bool eh_potencia_de_dois(uint32_t v);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
int conta_bits_ligados(uint32_t v) {
    // complete aqui
    (void)v;
    return 0;
}

bool eh_potencia_de_dois(uint32_t v) {
    // complete aqui
    (void)v;
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    assert(conta_bits_ligados(0) == 0);
    assert(conta_bits_ligados(0b1011) == 3);
    assert(conta_bits_ligados(0x80000000u) == 1);   // so o bit 31
    assert(conta_bits_ligados(0xFFFFFFFFu) == 32);
    assert(conta_bits_ligados(0xF0F0F0F0u) == 16);

    assert(eh_potencia_de_dois(1));
    assert(eh_potencia_de_dois(64));
    assert(eh_potencia_de_dois(0x80000000u));
    assert(!eh_potencia_de_dois(0));
    assert(!eh_potencia_de_dois(6));
    assert(!eh_potencia_de_dois(0xFFFFFFFFu));
    assert(!eh_potencia_de_dois(0x80000001u));

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
