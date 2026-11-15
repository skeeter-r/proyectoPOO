#include "Personaje.h"

Personaje::Personaje(const std::string& nombre, int vidasMax)
    : nombre(nombre), vidas(vidasMax), vidasMax(vidasMax) {}

void Personaje::recibirDanio(int cantidad) {
    vidas -= cantidad;
    if (vidas < 0) vidas = 0;
}

void Personaje::curar(int cantidad) {
    vidas += cantidad;
    if (vidas > vidasMax) vidas = vidasMax;
}

bool Personaje::estaVivo() const { return vidas > 0; }

const std::string& Personaje::getNombre() const { return nombre; }
int Personaje::getVidas() const { return vidas; }
int Personaje::getVidasMax() const { return vidasMax; }
