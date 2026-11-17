#include "Vagon.h"
#include <iostream>

Vagon::Vagon(int numero, const std::string& descripcion)
    : numero(numero), descripcion(descripcion) {}

void Vagon::conectar(const std::string& direccion, Vagon* destino) {
    salidas[direccion] = destino;
}

Vagon* Vagon::getSalida(const std::string& direccion) const {
    auto it = salidas.find(direccion);
    if (it == salidas.end()) return nullptr;
    return it->second;
}

void Vagon::agregarEnemigo(Enemigo* enemigo) { enemigos.push_back(enemigo); }

const std::vector<Enemigo*>& Vagon::getEnemigos() const { return enemigos; }

bool Vagon::hayEnemigosVivos() const {
    for (const Enemigo* e : enemigos)
        if (e->estaVivo()) return true;
    return false;
}

void Vagon::agregarObjeto(const Objeto& objeto) { objetos.push_back(objeto); }

bool Vagon::hayObjetos() const { return !objetos.empty(); }

std::vector<Objeto> Vagon::recogerObjetos() {
    std::vector<Objeto> tomados;
    if (hayEnemigosVivos()) return tomados;   // no se puede mientras haya enemigos
    tomados = objetos;
    objetos.clear();
    return tomados;
}

int Vagon::getNumero() const { return numero; }
const std::string& Vagon::getDescripcion() const { return descripcion; }

void Vagon::describir() const {
    std::cout << "=== VAGON " << numero << " ===\n" << descripcion << "\n";
}
