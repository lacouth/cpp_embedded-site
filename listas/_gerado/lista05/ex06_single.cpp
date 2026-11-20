// ex06 — versao de arquivo unico (gerada automaticamente,
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
#ifndef EX06_H
#define EX06_H

inline uint32_t mascara(unsigned largura) {
    return largura >= 32 ? 0xFFFFFFFFu : (1u << largura) - 1u;
}

class Registrador32 {
private:
    uint32_t& valor_;

public:
    explicit Registrador32(uint32_t& valor) : valor_(valor) {}
    void liga(unsigned bit) { valor_ |= (1u << bit); }
    void desliga(unsigned bit) { valor_ &= ~(1u << bit); }
    bool testa(unsigned bit) const { return (valor_ >> bit) & 1u; }
    uint32_t campo(unsigned pos, unsigned largura) const {
        return (valor_ >> pos) & mascara(largura);
    }
    void grava_campo(unsigned pos, unsigned largura, uint32_t novo) {
        uint32_t m = mascara(largura) << pos;
        valor_ = (valor_ & ~m) | ((novo << pos) & m);
    }
};

enum class Polaridade : uint32_t { Normal = 0, Invertida = 1 };

// Registrador de controle de um canal de PWM:
//   bit 0       LIGA
//   bit 1       POLARIDADE
//   bits 4-7    RESOLUCAO (1 a 15)
//   bits 16-31  DUTY (0 a 2^resolucao - 1)
class ConfigPwm {
private:
    static constexpr unsigned BIT_LIGA = 0;
    static constexpr unsigned BIT_POLARIDADE = 1;
    static constexpr unsigned POS_RESOLUCAO = 4, LARG_RESOLUCAO = 4;
    static constexpr unsigned POS_DUTY = 16, LARG_DUTY = 16;

    Registrador32 reg_;

public:
    explicit ConfigPwm(uint32_t& registrador) : reg_(registrador) {}

    void liga();
    void desliga();
    bool ligado() const;

    void ajusta_polaridade(Polaridade p);
    Polaridade polaridade() const;

    // Ignora bits fora de 1..15. Ao diminuir, satura o duty que deixou de caber.
    void ajusta_resolucao(unsigned bits);
    unsigned resolucao() const;

    // O maior duty que cabe na resolucao atual: 2^resolucao - 1.
    uint32_t duty_maximo() const;

    // Satura em duty_maximo().
    void ajusta_duty(uint32_t d);
    uint32_t duty() const;
};

#endif

// ============================================================
// ESCREVA SUA SOLUCAO AQUI
// ============================================================
void ConfigPwm::liga() {
    // complete aqui
}

void ConfigPwm::desliga() {
    // complete aqui
}

bool ConfigPwm::ligado() const {
    // complete aqui
    return false;
}

void ConfigPwm::ajusta_polaridade(Polaridade p) {
    // complete aqui
    (void)p;
}

Polaridade ConfigPwm::polaridade() const {
    // complete aqui
    return Polaridade::Normal;
}

void ConfigPwm::ajusta_resolucao(unsigned bits) {
    // complete aqui
    (void)bits;
}

unsigned ConfigPwm::resolucao() const {
    // complete aqui
    return 0;
}

uint32_t ConfigPwm::duty_maximo() const {
    // complete aqui
    return 0;
}

void ConfigPwm::ajusta_duty(uint32_t d) {
    // complete aqui
    (void)d;
}

uint32_t ConfigPwm::duty() const {
    // complete aqui
    return 0;
}

// ============================================================
// NAO ALTERE — testes
// ============================================================
int main() {
    uint32_t r = 0;
    ConfigPwm pwm(r);

    // liga/desliga mexe so no bit 0
    pwm.liga();
    assert(pwm.ligado());
    assert(r == 0x1);
    pwm.desliga();
    assert(!pwm.ligado());
    assert(r == 0x0);

    // polaridade no bit 1
    pwm.ajusta_polaridade(Polaridade::Invertida);
    assert(r == 0x2);
    assert(pwm.polaridade() == Polaridade::Invertida);
    pwm.ajusta_polaridade(Polaridade::Normal);
    assert(r == 0x0);
    assert(pwm.polaridade() == Polaridade::Normal);

    // resolucao nos bits 4-7, so de 1 a 15
    pwm.ajusta_resolucao(8);
    assert(pwm.resolucao() == 8);
    assert(r == 0x80);
    assert(pwm.duty_maximo() == 255);
    pwm.ajusta_resolucao(0);                 // ignorado
    pwm.ajusta_resolucao(16);                // ignorado
    assert(pwm.resolucao() == 8);
    assert(r == 0x80);

    // duty nos bits 16-31, saturado
    pwm.ajusta_duty(100);
    assert(pwm.duty() == 100);
    assert(r == 0x00640080u);
    pwm.ajusta_duty(1000);
    assert(pwm.duty() == 255);

    // o exemplo do enunciado
    pwm.ajusta_polaridade(Polaridade::Invertida);
    pwm.liga();
    assert(r == 0x00FF0083u);

    // diminuir a resolucao satura o duty que deixou de caber
    pwm.ajusta_resolucao(4);
    assert(pwm.duty_maximo() == 15);
    assert(pwm.duty() == 15);
    assert(pwm.ligado());                    // o resto nao mudou
    assert(pwm.polaridade() == Polaridade::Invertida);

    // aumentar a resolucao nao mexe no duty
    pwm.ajusta_resolucao(15);
    assert(pwm.duty() == 15);
    assert(pwm.duty_maximo() == 32767);
    pwm.ajusta_duty(40000);
    assert(pwm.duty() == 32767);
    assert(r == 0x7FFF00F3u);

    std::cout << "Todos os testes passaram!\n";
    return 0;
}
