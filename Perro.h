#ifndef PERRO_H
#define PERRO_H

#include "Enemigo.h"

// 4 vidas. Muerde (1 de dano) y la mordida se INFECTA: en su siguiente turno
// quita 1 vida mas. Cada golpe vale 1, asi que la chaqueta no lo reduce.
class Perro : public Enemigo {
private:
    bool infeccionPendiente;   // true si la mordida anterior dejo infeccion
public:
    Perro();
    void atacar(Personaje& objetivo) override;
    std::string getTipo() const override;
};

#endif
