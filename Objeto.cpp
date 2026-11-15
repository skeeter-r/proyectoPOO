#include "Objeto.h"

Objeto::Objeto(const std::string& nombre, const std::string& descripcion,
               TipoObjeto tipo, int valor)
    : nombre(nombre), descripcion(descripcion), tipo(tipo), valor(valor) {}

const std::string& Objeto::getNombre() const { return nombre; }
const std::string& Objeto::getDescripcion() const { return descripcion; }
TipoObjeto Objeto::getTipo() const { return tipo; }
int Objeto::getValor() const { return valor; }

bool Objeto::esArma() const {
    return tipo == TipoObjeto::NAVAJA || tipo == TipoObjeto::LATIGO ||
           tipo == TipoObjeto::NUDILLERAS;
}

bool Objeto::golpeaATodos() const { return tipo == TipoObjeto::LATIGO; }
bool Objeto::esUsoUnico() const { return tipo == TipoObjeto::NUDILLERAS; }

std::string Objeto::serializar() const { return nombre; }

// ---- Fabricas: aqui estan todos los numeros de los objetos ----
Objeto Objeto::crearNavaja() {
    return Objeto("Navaja", "Resta 2 vidas a un enemigo", TipoObjeto::NAVAJA, 2);
}
Objeto Objeto::crearLatigo() {
    return Objeto("Latigo", "Resta 1 vida a todos los enemigos", TipoObjeto::LATIGO, 1);
}
Objeto Objeto::crearNudilleras() {
    return Objeto("Nudilleras", "Resta 4 vidas. Un solo uso", TipoObjeto::NUDILLERAS, 4);
}
Objeto Objeto::crearChaqueta() {
    return Objeto("Chaqueta", "Reduce 1 el dano de cada golpe", TipoObjeto::CHAQUETA, 1);
}
Objeto Objeto::crearSueroVital() {
    return Objeto("Suero vital", "Cura 4 vidas", TipoObjeto::SUERO_VITAL, 4);
}
