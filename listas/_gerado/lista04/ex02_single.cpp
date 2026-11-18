// ex02 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX02_H
#define EX02_H

enum class Fase { Verde, Amarelo, Vermelho };

Fase proxima_fase(Fase f);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
Fase proxima_fase(Fase f) {
    // complete aqui
    return f;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    assert(proxima_fase(Fase::Verde) == Fase::Amarelo);
    assert(proxima_fase(Fase::Amarelo) == Fase::Vermelho);
    assert(proxima_fase(Fase::Vermelho) == Fase::Verde);

    // tres passos voltam ao comeco
    Fase f = Fase::Amarelo;
    for (int i = 0; i < 3; i++) {
        f = proxima_fase(f);
    }
    assert(f == Fase::Amarelo);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
