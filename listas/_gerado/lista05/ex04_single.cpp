// ex04 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX04_H
#define EX04_H

// Pixel RGB565: bits 15-11 vermelho, 10-5 verde, 4-0 azul.
// r, g, b vem com 8 bits; fica so a parte mais significativa de cada um.
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);

// O campo de cada cor, como esta no pixel.
uint8_t vermelho565(uint16_t p);   // 0 a 31
uint8_t verde565(uint16_t p);      // 0 a 63
uint8_t azul565(uint16_t p);       // 0 a 31

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    // complete aqui
    (void)r;
    (void)g;
    (void)b;
    return 0;
}

uint8_t vermelho565(uint16_t p) {
    // complete aqui
    (void)p;
    return 0;
}

uint8_t verde565(uint16_t p) {
    // complete aqui
    (void)p;
    return 0;
}

uint8_t azul565(uint16_t p) {
    // complete aqui
    (void)p;
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    assert(rgb565(0, 0, 0) == 0x0000);
    assert(rgb565(255, 255, 255) == 0xFFFF);
    assert(rgb565(255, 0, 0) == 0xF800);
    assert(rgb565(0, 255, 0) == 0x07E0);
    assert(rgb565(0, 0, 255) == 0x001F);
    // so os bits de cima contam: 7 (0000 0111) some no vermelho de 5 bits
    assert(rgb565(7, 0, 0) == 0x0000);
    assert(rgb565(8, 0, 0) == 0x0800);
    assert(rgb565(0x12, 0x34, 0x56) == 0x11AA);

    assert(vermelho565(0xF800) == 31);
    assert(verde565(0xF800) == 0);
    assert(verde565(0x07E0) == 63);
    assert(azul565(0x001F) == 31);
    assert(azul565(0x07E0) == 0);

    uint16_t p = rgb565(200, 100, 50);
    assert(vermelho565(p) == (200 >> 3));
    assert(verde565(p) == (100 >> 2));
    assert(azul565(p) == (50 >> 3));

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
