#include "Jugador.h"
#include <iostream>

namespace {
// Indice de la primera arma del inventario, o -1 si no hay ninguna
int primerArma(const std::vector<Objeto>& inv) {
    for (size_t i = 0; i < inv.size(); i++)
        if (inv[i].esArma()) return static_cast<int>(i);
    return -1;
}
}

Jugador::Jugador(const std::string& nombre)
    : Personaje(nombre, 8), nivel(1), experiencia(0),
      chaquetaPuesta(false), armaSeleccionada(0) {}

void Jugador::atacar(Personaje& objetivo) {
    // Si el arma seleccionada ya no es valida, usa la primera que haya
    if (armaSeleccionada < 0 || armaSeleccionada >= static_cast<int>(inventario.size()) ||
        !inventario[armaSeleccionada].esArma()) {
        armaSeleccionada = primerArma(inventario);
    }
    if (armaSeleccionada < 0) {
        std::cout << "No tienes ningun arma.\n";
        return;
    }

    Objeto arma = inventario[armaSeleccionada];   // copia, por si se elimina
    int antes = objetivo.getVidas();
    objetivo.recibirDanio(arma.getValor());
    int quitadas = antes - objetivo.getVidas();
    std::cout << "Golpeas a " << objetivo.getNombre() << " con " << arma.getNombre()
              << ": -" << quitadas << " vidas.\n";

    if (arma.esUsoUnico()) {
        inventario.erase(inventario.begin() + armaSeleccionada);
        armaSeleccionada = primerArma(inventario);
        std::cout << "(" << arma.getNombre() << " ya no se puede volver a usar.)\n";
    }
}

void Jugador::recibirDanio(int cantidad) {
    if (chaquetaPuesta) {
        cantidad -= 1;
        if (cantidad < 1) cantidad = 1;   // minimo 1
    }
    Personaje::recibirDanio(cantidad);
}

void Jugador::agregarObjeto(const Objeto& obj) { inventario.push_back(obj); }

bool Jugador::usarObjeto(int indice) {
    if (indice < 0 || indice >= static_cast<int>(inventario.size())) return false;

    Objeto obj = inventario[indice];
    switch (obj.getTipo()) {
        case TipoObjeto::CHAQUETA:
            chaquetaPuesta = true;
            std::cout << "Te pones la chaqueta: recibiras 1 de dano menos por golpe.\n";
            break;
        case TipoObjeto::SUERO_VITAL:
            curar(obj.getValor());
            std::cout << "Usas el suero vital. Vidas: " << vidas << "/" << vidasMax << "\n";
            break;
        default:
            std::cout << "Las armas se usan atacando.\n";
            return false;
    }
    inventario.erase(inventario.begin() + indice);
    armaSeleccionada = primerArma(inventario);
    return true;
}

bool Jugador::seleccionarArma(int indice) {
    if (indice < 0 || indice >= static_cast<int>(inventario.size())) return false;
    if (!inventario[indice].esArma()) return false;
    armaSeleccionada = indice;
    return true;
}

bool Jugador::tieneNudilleras() const {
    for (const Objeto& o : inventario)
        if (o.getTipo() == TipoObjeto::NUDILLERAS) return true;
    return false;
}

bool Jugador::quitarNudilleras() {
    for (size_t i = 0; i < inventario.size(); i++) {
        if (inventario[i].getTipo() == TipoObjeto::NUDILLERAS) {
            inventario.erase(inventario.begin() + i);
            armaSeleccionada = primerArma(inventario);
            return true;
        }
    }
    return false;
}

const std::vector<Objeto>& Jugador::getInventario() const { return inventario; }

void Jugador::ganarExperiencia(int xp) { experiencia += xp; }

void Jugador::subirNivel() {
    nivel++;
    vidasMax++;   // 8, 9 y 10 vidas en los niveles 1, 2 y 3
}

void Jugador::iniciarNivel() {
    vidas = vidasMax;
    chaquetaPuesta = false;
    inventario.clear();
    inventario.push_back(Objeto::crearNavaja());
    inventario.push_back(Objeto::crearLatigo());
    inventario.push_back(Objeto::crearNudilleras());
    armaSeleccionada = 0;
}

int Jugador::getNivel() const { return nivel; }
int Jugador::getExperiencia() const { return experiencia; }
bool Jugador::getChaquetaPuesta() const { return chaquetaPuesta; }
