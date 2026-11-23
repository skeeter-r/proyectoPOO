#include "Combate.h"
#include "Entrada.h"
#include <iostream>

using namespace std;

Combate::Combate(Jugador& jugador, Vagon& vagon)
    : jugador(jugador), vagon(vagon) {}

vector<bool> Combate::estadoVivos() const {
    vector<bool> vivos;
    for (const Enemigo* e : vagon.getEnemigos()) {
        vivos.push_back(e->estaVivo());
    }
    return vivos;
}

void Combate::revisarCaidos(const vector<bool>& vivosAntes) {
    const vector<Enemigo*>& enemigos = vagon.getEnemigos();
    for (int i = 0; i < static_cast<int>(enemigos.size()); i++) {
        if (vivosAntes[i] && !enemigos[i]->estaVivo()) {
            cout << enemigos[i]->getNombre() << " cae derrotado. (+"
                 << enemigos[i]->getXpRecompensa() << " de exp)\n";
            jugador.ganarExperiencia(enemigos[i]->getXpRecompensa());
        }
    }
}

bool Combate::accionAtacar() {
    if (!vagon.hayEnemigosVivos()) {
        cout << "No hay enemigos en este vagon.\n";
        return false;
    }

    const vector<Objeto>& inv = jugador.getInventario();

    vector<int> armas;
    for (int i = 0; i < static_cast<int>(inv.size()); i++) {
        if (inv[i].esArma()) {
            armas.push_back(i);
        }
    }

    if (armas.empty()) {
        cout << "No tienes armas.\n";
        return false;
    }

    cout << "Con que arma?\n1. Volver\n";
    for (int k = 0; k < static_cast<int>(armas.size()); k++) {
        const Objeto& a = inv[armas[k]];
        cout << (k + 2) << ". " << a.getNombre() << " (dano " << a.getValor()
             << (a.golpeaATodos() ? ", golpea a todos" : "") << ")\n";
    }
    cout << "> ";
    int op = leerOpcion(1, static_cast<int>(armas.size()) + 1);
    if (op == 1) {
        return false;
    }

    int indiceArma = armas[op - 2];
    jugador.seleccionarArma(indiceArma);
    bool golpeaATodos = inv[indiceArma].golpeaATodos();

    const vector<Enemigo*>& enemigos = vagon.getEnemigos();

    vector<int> vivos;
    for (int i = 0; i < static_cast<int>(enemigos.size()); i++) {
        if (enemigos[i]->estaVivo()) {
            vivos.push_back(i);
        }
    }

    int objetivo = vivos[0];
    if (!golpeaATodos && static_cast<int>(vivos.size()) > 1) {
        cout << "A quien?\n1. Volver\n";
        for (int k = 0; k < static_cast<int>(vivos.size()); k++) {
            const Enemigo* e = enemigos[vivos[k]];
            cout << (k + 2) << ". " << e->getNombre() << " (" << e->getVidas() << " vidas)\n";
        }
        cout << "> ";
        int eleccion = leerOpcion(1, static_cast<int>(vivos.size()) + 1);
        if (eleccion == 1) {
            return false;
        }
        objetivo = vivos[eleccion - 2];
    }

    vector<bool> antes = estadoVivos();
    if (golpeaATodos) {
        for (Enemigo* e : enemigos) {
            if (e->estaVivo()) {
                jugador.atacar(*e);
            }
        }
    } else {
        jugador.atacar(*enemigos[objetivo]);
    }

    revisarCaidos(antes);
    return true;
}

void Combate::turnoEnemigos() {
    cout << "\n";
    for (Enemigo* e : vagon.getEnemigos()) {
        if (e->estaVivo() && jugador.estaVivo()) {
            e->atacar(jugador);
        }
    }
}
