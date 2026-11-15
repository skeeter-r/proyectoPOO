#ifndef ASESINO_H
#define ASESINO_H

#include "Enemigo.h"

// 4 vidas. Apunala: 2 de dano por golpe.
class Asesino : public Enemigo {
public:
    Asesino();
    void atacar(Personaje& objetivo) override;
    std::string getTipo() const override;
};

#endif
