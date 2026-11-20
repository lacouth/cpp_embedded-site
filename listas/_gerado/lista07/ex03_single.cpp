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

struct Ponto {
    int32_t bruto;
    int32_t valor;
};

struct Leitura {
    int32_t valor;
    bool valida;
};

// Tabela com bruto DECRESCENTE. Entre dois pontos: reta, arredondada.
// Fora da tabela: valida == false.
Leitura interpola(const Ponto tabela[], int n, int32_t bruto);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
Leitura interpola(const Ponto tabela[], int n, int32_t bruto) {
    // complete aqui
    (void)tabela; (void)n; (void)bruto;
    return {0, false};
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    const Ponto simples[] = {{1000, 0}, {500, 100}, {0, 300}};
    assert(interpola(simples, 3, 1000).valida);
    assert(interpola(simples, 3, 1000).valor == 0);
    assert(interpola(simples, 3, 750).valor == 50);
    assert(interpola(simples, 3, 500).valor == 100);
    assert(interpola(simples, 3, 250).valor == 200);
    assert(interpola(simples, 3, 333).valor == 167);     // 166,8
    assert(interpola(simples, 3, 0).valor == 300);
    assert(!interpola(simples, 3, 1001).valida);
    assert(!interpola(simples, 3, -1).valida);

    // uma tabela em que o valor CAI (o numerador fica negativo)
    const Ponto caindo[] = {{90, 60}, {0, 0}};
    assert(interpola(caindo, 2, 31).valor == 21);        // 20,67
    const Ponto negativa[] = {{90, 0}, {0, -60}};
    assert(interpola(negativa, 2, 59).valor == -21);     // -20,67

    // a tabela do NTC do capitulo
    const Ponto ntc[] = {
        {3740, -200}, {3495, -100}, {3156, 0}, {2738, 100}, {2278, 200},
        {1825, 300}, {1419, 400}, {1081, 500}, {815, 600}, {613, 700},
        {462, 800}, {350, 900}, {267, 1000},
    };
    assert(interpola(ntc, 13, 2048).valor == 251);
    assert(interpola(ntc, 13, 3156).valor == 0);
    assert(interpola(ntc, 13, 267).valor == 1000);
    assert(!interpola(ntc, 13, 4095).valida);            // fio solto do lado do GND
    assert(!interpola(ntc, 13, 0).valida);               // curto

    // tabela de um ponto so: nada cai "entre dois pontos"
    const Ponto um[] = {{100, 7}};
    assert(!interpola(um, 1, 100).valida);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
