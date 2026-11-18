// ex01 — versao de arquivo unico (gerada automaticamente,
// nao edite este arquivo direto: edite os fontes em exercicios/ e rode
// tools/gerar_single.py --todos de novo).
//
// Cole este arquivo inteiro no seu compilador online e rode.

#include <cassert>
#include <iostream>
#include <string>

// ============================================================
// Declaracoes do exercicio
// ============================================================
#ifndef EX01_H
#define EX01_H

enum class Fase { Verde, Amarelo, Vermelho };

std::string nome_da_fase(Fase f);

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
std::string nome_da_fase(Fase f) {
    // complete aqui
    return "";
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    assert(nome_da_fase(Fase::Verde) == "verde");
    assert(nome_da_fase(Fase::Amarelo) == "amarelo");
    assert(nome_da_fase(Fase::Vermelho) == "vermelho");

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
