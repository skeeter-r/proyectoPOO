#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "Enemigo.h"

// Jefe final: 2 de dano por golpe.
// Vidas segun el nivel: 6 (nivel 1), 9 (nivel 2) y 12 (nivel 3).
class Conductor : public Enemigo {
public:
    Conductor(int vidas = 6);
    void atacar(Personaje& objetivo) override;
    std::string getTipo() const override;
};

#endif
