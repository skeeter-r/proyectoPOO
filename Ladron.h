#ifndef LADRON_H
#define LADRON_H

#include "Enemigo.h"

// 4 vidas. Desde su 2.o turno intenta robar las nudilleras del jugador.
// Si el jugador no las tiene, le hace 1 de dano.
class Ladron : public Enemigo {
private:
    int turnosActivo;   // el primer turno solo se acerca
public:
    Ladron();
    void atacar(Personaje& objetivo) override;
    std::string getTipo() const override;
};

#endif
