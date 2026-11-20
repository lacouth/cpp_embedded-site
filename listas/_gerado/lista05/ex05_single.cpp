// ex05 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX05_H
#define EX05_H

inline uint32_t mascara(unsigned largura) {
    return largura >= 32 ? 0xFFFFFFFFu : (1u << largura) - 1u;
}

// Um registrador de 32 bits visto por metodos com nome.
// Guarda uma REFERENCIA: mexe na variavel de fora, nao numa copia.
class Registrador32 {
private:
    uint32_t& valor_;

public:
    explicit Registrador32(uint32_t& valor) : valor_(valor) {}

    void liga(unsigned bit);
    void desliga(unsigned bit);
    void inverte(unsigned bit);
    bool testa(unsigned bit) const;

    uint32_t campo(unsigned pos, unsigned largura) const {
        return (valor_ >> pos) & mascara(largura);
    }
    void grava_campo(unsigned pos, unsigned largura, uint32_t novo) {
        uint32_t m = mascara(largura) << pos;
        valor_ = (valor_ & ~m) | ((novo << pos) & m);
    }

    uint32_t valor() const { return valor_; }
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void Registrador32::liga(unsigned bit) {
    // complete aqui
    (void)bit;
}

void Registrador32::desliga(unsigned bit) {
    // complete aqui
    (void)bit;
}

void Registrador32::inverte(unsigned bit) {
    // complete aqui
    (void)bit;
}

bool Registrador32::testa(unsigned bit) const {
    // complete aqui
    (void)bit;
    return false;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    uint32_t gpio = 0;
    Registrador32 reg(gpio);

    // o efeito aparece na variavel de fora
    reg.liga(26);
    assert(gpio == 0x04000000u);
    assert(reg.testa(26));
    assert(!reg.testa(25));

    // ligar outro bit nao apaga o primeiro
    reg.liga(2);
    assert(gpio == 0x04000004u);

    reg.desliga(26);
    assert(gpio == 0x00000004u);
    reg.desliga(26);                       // desligar de novo nao muda nada
    assert(gpio == 0x00000004u);

    reg.inverte(31);
    assert(gpio == 0x80000004u);
    reg.inverte(31);
    assert(gpio == 0x00000004u);
    assert(!reg.testa(31));

    // mudanca feita por fora tambem aparece pela classe
    gpio = 0xFFFFFFFFu;
    assert(reg.testa(0) && reg.testa(31));
    reg.desliga(0);
    assert(gpio == 0xFFFFFFFEu);

    // dois objetos sobre a mesma palavra enxergam a mesma coisa
    Registrador32 outro(gpio);
    outro.desliga(31);
    assert(!reg.testa(31));
    assert(reg.valor() == 0x7FFFFFFEu);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
