#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <string>
#include "Personaje.h"

// Clase abstracta intermedia: atributos comunes a todos los enemigos
class Enemigo : public Personaje {
protected:
    int danio;          // vidas que quita al jugador
    int xpRecompensa;   // experiencia que da al ser derrotado

public:
    Enemigo(const std::string& nombre, int vidas, int danio, int xpRecompensa);

    // Sigue siendo virtual puro: cada enemigo define su ataque
    void atacar(Personaje& objetivo) override = 0;

    int getDanio() const;
    int getXpRecompensa() const;

    // Para guardar/cargar: "Ladron", "Asesino" o "Conductor"
    virtual std::string getTipo() const = 0;
};

#endif
