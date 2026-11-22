#include "Conductor.h"
#include <iostream>

Conductor::Conductor(int vidas) : Enemigo("Conductor", vidas, 3, 50) {}

void Conductor::atacar(Personaje& objetivo) {
    int antes = objetivo.getVidas();
    objetivo.recibirDanio(danio);
    std::cout << "El Conductor te golpea con fuerza: -" << (antes - objetivo.getVidas()) << " vida(s).\n";
}

std::string Conductor::getTipo() const { return "Conductor"; }
