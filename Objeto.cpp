#include "Objeto.h"

Objeto::Objeto(const std::string& nombre, const std::string& descripcion,
               TipoObjeto tipo, int valor)
    : nombre(nombre), descripcion(descripcion), tipo(tipo), valor(valor) {}

const std::string& Objeto::getNombre() const { return nombre; }
const std::string& Objeto::getDescripcion() const { return descripcion; }
TipoObjeto Objeto::getTipo() const { return tipo; }
int Objeto::getValor() const { return valor; }

bool Objeto::esArma() const {
    return tipo == TipoObjeto::NAVAJA || tipo == TipoObjeto::LATIGO;
}

bool Objeto::golpeaATodos() const { return tipo == TipoObjeto::LATIGO; }

std::string Objeto::serializar() const { return nombre; }

bool Objeto::desdeNombre(const std::string& nombre, Objeto& salida) {
    if (nombre == "Navaja") salida = crearNavaja();
    else if (nombre == "Latigo") salida = crearLatigo();
    else if (nombre == "Chaqueta") salida = crearChaqueta();
    else if (nombre == "Suero vital") salida = crearSueroVital();
    else return false;
    return true;
}

// ---- Fabricas: aqui estan todos los numeros de los objetos ----
Objeto Objeto::crearNavaja() {
    return Objeto("Navaja", "Resta 2 vidas a un enemigo", TipoObjeto::NAVAJA, 2);
}
Objeto Objeto::crearLatigo() {
    return Objeto("Latigo", "Resta 1 vida a todos los enemigos", TipoObjeto::LATIGO, 1);
}
Objeto Objeto::crearChaqueta() {
    return Objeto("Chaqueta", "Reduce 1 el dano de cada golpe (automatica)", TipoObjeto::CHAQUETA, 1);
}
Objeto Objeto::crearSueroVital() {
    return Objeto("Suero vital", "Cura 4 vidas (solo si las recuperas todas)", TipoObjeto::SUERO_VITAL, 4);
}
