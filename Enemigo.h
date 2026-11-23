#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <string>
#include "Personaje.h"

class Enemigo : public Personaje {
protected:
    int danio;
    int xpRecompensa;

public:
    Enemigo(const std::string& nombre, int vidas, int danio, int xpRecompensa);

    void atacar(Personaje& objetivo) override = 0;

    int getDanio() const;
    int getXpRecompensa() const;

    // Devuelve "Perro", "Asesino" o "Conductor"
    virtual std::string getTipo() const = 0;
};

#endif