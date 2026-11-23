#include "Tren.h"
#include "Perro.h"
#include "Asesino.h"
#include "Conductor.h"
#include <string>

using namespace std;

Tren::Tren() {}

Tren::~Tren() {
    liberar();
}

void Tren::liberar() {
    for (Vagon* v : vagones) {
        for (Enemigo* e : v->vaciarEnemigos()) {
            delete e;
        }
        delete v;
    }
    vagones.clear();
}

void Tren::crear() {
    liberar();

    for (int i = 0; i < NUM_VAGONES; i++) {
        string desc = "";
        switch (i) {
            case 0:
                desc = "Despiertas en el ultimo vagon. El tren avanza sin control.\nUn perro rabioso ladra con furia entre los asientos.";
                break;
            case 1:
                desc = "Las luces apenas funcionan. Entre las sombras brilla un cuchillo.\nSobre un asiento hay una chaqueta de cuero.";
                break;
            case 2:
                desc = "Dos personas te cierran el paso. Las dos afilan una hoja.";
                break;
            case 3:
                desc = "Un perro rabioso vigila junto a un tipo armado.\nAl fondo hay una caja con una cruz roja.";
                break;
            case 4:
                desc = "El vagon vibra duro. El Conductor sigue en los controles sin voltear:\n\"Llegaste tarde. Nadie baja de este tren\".";
                break;
        }
        vagones.push_back(new Vagon(i + 1, desc));
    }

    // adelante va hacia la cabina y atras vuelve al vagon anterior
    for (int i = 0; i < NUM_VAGONES; i++) {
        if (i > 0) {
            vagones[i]->conectar("atras", vagones[i - 1]);
        }
        if (i < NUM_VAGONES - 1) {
            vagones[i]->conectar("adelante", vagones[i + 1]);
        }
    }

    vagones[1]->agregarObjeto(Objeto::crearChaqueta());
    vagones[3]->agregarObjeto(Objeto::crearSueroVital());
}

void Tren::vaciarEnemigos(int indice) {
    for (Enemigo* e : vagones[indice]->vaciarEnemigos()) {
        delete e;
    }
}

void Tren::crearEnemigos(int indice) {
    vaciarEnemigos(indice);
    Vagon* v = vagones[indice];
    switch (indice) {
        case 0:
            v->agregarEnemigo(new Perro());
            break;
        case 1:
            v->agregarEnemigo(new Asesino());
            break;
        case 2:
            v->agregarEnemigo(new Asesino());
            v->agregarEnemigo(new Asesino());
            break;
        case 3:
            v->agregarEnemigo(new Perro());
            v->agregarEnemigo(new Asesino());
            break;
        case 4:
            v->agregarEnemigo(new Conductor());
            break;
    }
}

Vagon* Tren::getVagon(int indice) const {
    return vagones[indice];
}

int Tren::getNumVagones() const {
    return static_cast<int>(vagones.size());
}
