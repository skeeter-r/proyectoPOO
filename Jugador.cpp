#include "Jugador.h"
#include <iostream>

namespace {
// Indice de la primera arma del inventario, o -1 si no hay ninguna
int primerArma(const std::vector<Objeto>& inv) {
    for (size_t i = 0; i < inv.size(); i++)
        if (inv[i].esArma()) return static_cast<int>(i);
    return -1;
}

// Indice del primer objeto de ese tipo, o -1 si no hay
int buscar(const std::vector<Objeto>& inv, TipoObjeto tipo) {
    for (size_t i = 0; i < inv.size(); i++)
        if (inv[i].getTipo() == tipo) return static_cast<int>(i);
    return -1;
}
}

Jugador::Jugador(const std::string& nombre)
    : Personaje(nombre, 8), experiencia(0), experienciaAlEmpezar(0),
      armaSeleccionada(0), sueroDesbloqueado(false) {}

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

    const Objeto& arma = inventario[armaSeleccionada];
    int antes = objetivo.getVidas();
    objetivo.recibirDanio(arma.getValor());
    int quitadas = antes - objetivo.getVidas();
    std::cout << "Golpeas a " << objetivo.getNombre() << " con " << arma.getNombre()
              << ": -" << quitadas << " vida(s).\n";
}

void Jugador::recibirDanio(int cantidad) {
    if (tieneChaqueta()) {
        cantidad -= 1;
        if (cantidad < 1) cantidad = 1;   // minimo 1
    }
    Personaje::recibirDanio(cantidad);
}

void Jugador::agregarObjeto(const Objeto& obj) {
    inventario.push_back(obj);
    if (obj.getTipo() == TipoObjeto::SUERO_VITAL) sueroDesbloqueado = true;
}

bool Jugador::usarSuero() {
    int indice = buscar(inventario, TipoObjeto::SUERO_VITAL);
    if (indice < 0) {
        std::cout << "No tienes suero vital.\n";
        return false;
    }

    int cura = inventario[indice].getValor();
    if (vidas > vidasMax - cura) {   // curaria menos de lo que otorga
        std::cout << "Tus vidas no estan suficientemente bajas para usar el suero vital.\n";
        return false;
    }

    curar(cura);
    inventario.erase(inventario.begin() + indice);
    armaSeleccionada = primerArma(inventario);
    std::cout << "Usas el suero vital. Vidas: " << vidas << "/" << vidasMax << "\n";
    return true;
}

bool Jugador::seleccionarArma(int indice) {
    if (indice < 0 || indice >= static_cast<int>(inventario.size())) return false;
    if (!inventario[indice].esArma()) return false;
    armaSeleccionada = indice;
    return true;
}

bool Jugador::tieneChaqueta() const { return buscar(inventario, TipoObjeto::CHAQUETA) >= 0; }
bool Jugador::tieneSuero() const { return buscar(inventario, TipoObjeto::SUERO_VITAL) >= 0; }

const std::vector<Objeto>& Jugador::getInventario() const { return inventario; }

void Jugador::ganarExperiencia(int xp) { experiencia += xp; }

void Jugador::iniciarNivel() {
    vidas = vidasMax;
    // Si ya desbloqueo el suero, empieza cada intento con uno
    if (sueroDesbloqueado && !tieneSuero()) inventario.push_back(Objeto::crearSueroVital());
    armaSeleccionada = primerArma(inventario);
    experienciaAlEmpezar = experiencia;
}

void Jugador::descartarIntento() {
    experiencia = experienciaAlEmpezar;
    vidas = vidasMax;
}

void Jugador::reiniciar() {
    vidas = vidasMax;
    experiencia = 0;
    experienciaAlEmpezar = 0;
    armaSeleccionada = 0;
    sueroDesbloqueado = false;
    inventario.clear();
}

int Jugador::getExperiencia() const { return experiencia; }
