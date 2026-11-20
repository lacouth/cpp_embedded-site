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

// largura bits ligados a partir do bit 0. mascara(0) == 0, mascara(32) == 0xFFFFFFFF.
uint32_t mascara(unsigned largura);

// O numero guardado nos bits pos .. pos+largura-1 de valor.
uint32_t extrai_campo(uint32_t valor, unsigned pos, unsigned largura);

// valor com o campo trocado por novo; o que nao cabe no campo e descartado.
uint32_t grava_campo(uint32_t valor, unsigned pos, unsigned largura, uint32_t novo);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
uint32_t mascara(unsigned largura) {
    // complete aqui
    (void)largura;
    return 0;
}

uint32_t extrai_campo(uint32_t valor, unsigned pos, unsigned largura) {
    // complete aqui
    (void)valor;
    (void)pos;
    (void)largura;
    return 0;
}

uint32_t grava_campo(uint32_t valor, unsigned pos, unsigned largura, uint32_t novo) {
    // complete aqui
    (void)pos;
    (void)largura;
    (void)novo;
    return valor;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    assert(mascara(0) == 0);
    assert(mascara(1) == 0x1);
    assert(mascara(4) == 0xF);
    assert(mascara(8) == 0xFF);
    assert(mascara(31) == 0x7FFFFFFFu);
    assert(mascara(32) == 0xFFFFFFFFu);

    assert(extrai_campo(0x00000A00u, 8, 8) == 0x0A);
    assert(extrai_campo(0x0000000Cu, 2, 2) == 0x3);
    assert(extrai_campo(0xF0000000u, 28, 4) == 0xF);
    assert(extrai_campo(0x12345678u, 0, 32) == 0x12345678u);

    // troca o campo sem mexer no resto
    assert(grava_campo(0x00000301u, 8, 8, 4) == 0x00000401u);
    assert(grava_campo(0xFFFFFFFFu, 8, 8, 0) == 0xFFFF00FFu);
    assert(grava_campo(0, 2, 2, 2) == 0x8);
    // o que nao cabe e descartado, sem invadir o vizinho
    assert(grava_campo(0x00000301u, 8, 8, 0x1FF) == 0x0000FF01u);
    assert(grava_campo(0, 0, 4, 0x13) == 0x3);
    // campo no topo e o registrador inteiro
    assert(grava_campo(0, 28, 4, 0xA) == 0xA0000000u);
    assert(grava_campo(0x11111111u, 0, 32, 0xCAFEBABEu) == 0xCAFEBABEu);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
