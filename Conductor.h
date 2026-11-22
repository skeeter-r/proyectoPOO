#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "Enemigo.h"

// Jefe final: 9 vidas y 3 de dano por golpe (2 si el jugador lleva la chaqueta).
class Conductor : public Enemigo {
public:
    Conductor(int vidas = 9);
    void atacar(Personaje& objetivo) override;
    std::string getTipo() const override;
};

#endif
