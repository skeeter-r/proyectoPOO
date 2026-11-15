#include "Asesino.h"
#include <iostream>

Asesino::Asesino() : Enemigo("Asesino", 4, 2, 20) {}

void Asesino::atacar(Personaje& objetivo) {
    int antes = objetivo.getVidas();
    objetivo.recibirDanio(danio);
    std::cout << "El Asesino te apunala: -" << (antes - objetivo.getVidas()) << " vidas.\n";
}

std::string Asesino::getTipo() const { return "Asesino"; }
