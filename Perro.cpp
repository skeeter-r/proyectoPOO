#include "Perro.h"
#include <iostream>

Perro::Perro() : Enemigo("Perro", 4, 1, 15), infeccionPendiente(false) {}

void Perro::atacar(Personaje& objetivo) {
    // 1) la infeccion de la mordida anterior
    if (infeccionPendiente) {
        objetivo.recibirDanio(1);
        std::cout << "La infeccion te quita 1 vida.\n";
        if (!objetivo.estaVivo()) return;
    }

    // 2) la mordida nueva
    objetivo.recibirDanio(danio);
    std::cout << "El Perro te muerde: -1 vida. Tu herida se infecta.\n";
    infeccionPendiente = true;
}

std::string Perro::getTipo() const { return "Perro"; }
