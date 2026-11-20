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

// Leva x da faixa [de_min, de_max] para [para_min, para_max], arredondando
// para o inteiro mais proximo e saturando fora da faixa de entrada.
// A faixa de saida pode ser invertida. Supoe de_min < de_max.
int32_t escala(int32_t x, int32_t de_min, int32_t de_max, int32_t para_min, int32_t para_max);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
int32_t escala(int32_t x, int32_t de_min, int32_t de_max, int32_t para_min, int32_t para_max) {
    // complete aqui
    (void)x; (void)de_min; (void)de_max; (void)para_min; (void)para_max;
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    // as pontas e o meio
    assert(escala(0, 0, 4095, 0, 1000) == 0);
    assert(escala(4095, 0, 4095, 0, 1000) == 1000);
    assert(escala(2048, 0, 4095, 0, 1000) == 500);
    assert(escala(2047, 0, 4095, 0, 1000) == 500);   // 499,88: arredonda, nao trunca
    assert(escala(4, 0, 4095, 0, 1000) == 1);        // 0,98

    // saturacao
    assert(escala(-50, 0, 4095, 0, 1000) == 0);
    assert(escala(4200, 0, 4095, 0, 1000) == 1000);

    // faixa de saida invertida (numerador negativo)
    assert(escala(0, 0, 1000, 180, 0) == 180);
    assert(escala(333, 0, 1000, 180, 0) == 120);     // 120,06
    assert(escala(500, 0, 1000, 180, 0) == 90);
    assert(escala(1000, 0, 1000, 180, 0) == 0);

    // faixa de entrada que nao comeca em zero
    assert(escala(0, -200, 1000, 0, 180) == 30);
    assert(escala(250, -200, 1000, 0, 180) == 68);   // 67,5 arredonda para longe do zero

    // o produto do meio passa de 2^31
    assert(escala(40000, 0, 65535, 0, 60000) == 36622);

    // graus -> microssegundos do servo
    assert(escala(90, 0, 180, 544, 2400) == 1472);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
