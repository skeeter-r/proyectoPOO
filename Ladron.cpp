#include "Ladron.h"
#include "Jugador.h"
#include <iostream>

Ladron::Ladron() : Enemigo("Ladron", 4, 1, 10), turnosActivo(0) {}

void Ladron::atacar(Personaje& objetivo) {
    turnosActivo++;

    // Primer turno: solo se acerca
    if (turnosActivo < 2) {
        std::cout << "El Ladron se acerca a tus bolsillos.\n";
        return;
    }

    // Desde el segundo turno intenta robar las nudilleras (solo el Jugador las tiene)
    Jugador* jugador = dynamic_cast<Jugador*>(&objetivo);
    if (jugador != nullptr && jugador->quitarNudilleras()) {
        std::cout << "El Ladron te roba las nudilleras!\n";
        return;
    }

    // Si no hay nada que robar, golpea
    int antes = objetivo.getVidas();
    objetivo.recibirDanio(danio);
    std::cout << "El Ladron te golpea: -" << (antes - objetivo.getVidas()) << " vidas.\n";
}

std::string Ladron::getTipo() const { return "Ladron"; }
