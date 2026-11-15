#include "Enemigo.h"

Enemigo::Enemigo(const std::string& nombre, int vidas, int danio, int xpRecompensa)
    : Personaje(nombre, vidas), danio(danio), xpRecompensa(xpRecompensa) {}

int Enemigo::getDanio() const { return danio; }
int Enemigo::getXpRecompensa() const { return xpRecompensa; }
