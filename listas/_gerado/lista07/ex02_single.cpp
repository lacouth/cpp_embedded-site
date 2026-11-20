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

// Calibracao de dois pontos: (bruto_a -> valor_a) e (bruto_b -> valor_b).
// converte() segue a reta entre eles, arredondando, SEM saturar.
class Calibracao {
private:
    const int32_t bruto_a_;
    const int32_t valor_a_;
    const int32_t bruto_b_;
    const int32_t valor_b_;

public:
    Calibracao(int32_t bruto_a, int32_t valor_a, int32_t bruto_b, int32_t valor_b)
        : bruto_a_(bruto_a), valor_a_(valor_a), bruto_b_(bruto_b), valor_b_(valor_b) {}

    int32_t converte(int32_t bruto) const;
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
int32_t Calibracao::converte(int32_t bruto) const {
    // complete aqui
    (void)bruto;
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    Calibracao nivel(410, 0, 3690, 2000);
    assert(nivel.converte(410) == 0);
    assert(nivel.converte(3690) == 2000);
    assert(nivel.converte(2050) == 1000);
    assert(nivel.converte(1000) == 360);      // 359,76

    // fora dos pontos: extrapola, nao satura
    assert(nivel.converte(4095) == 2247);     // 2246,95
    assert(nivel.converte(0) == -250);

    // pontos dados "ao contrario" (bruto_a > bruto_b) dao a mesma reta
    Calibracao invertida(3690, 2000, 410, 0);
    assert(invertida.converte(1000) == 360);
    assert(invertida.converte(0) == -250);

    // grandeza que cai quando o bruto sobe (como o NTC)
    Calibracao ntc(3156, 0, 1081, 500);       // 0 C e 50 C, em decimos
    assert(ntc.converte(2048) == 267);        // a reta erra muito: a curva e o ex03
    assert(ntc.converte(3156) == 0);

    // converte e const: da para usar por referencia constante
    const Calibracao& ref = nivel;
    assert(ref.converte(2050) == 1000);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
