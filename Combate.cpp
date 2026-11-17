#include "Combate.h"
#include "Entrada.h"
#include <iostream>

using namespace std;

Combate::Combate(Jugador& jugador, Vagon& vagon) : jugador(jugador), vagon(vagon) {}

vector<bool> Combate::estadoVivos() const {
    vector<bool> vivos;
    for (const Enemigo* e : vagon.getEnemigos()) vivos.push_back(e->estaVivo());
    return vivos;
}

void Combate::revisarCaidos(const vector<bool>& vivosAntes) {
    const vector<Enemigo*>& enemigos = vagon.getEnemigos();
    for (size_t i = 0; i < enemigos.size(); i++) {
        if (vivosAntes[i] && !enemigos[i]->estaVivo()) {
            cout << enemigos[i]->getNombre() << " cae derrotado. (+"
                 << enemigos[i]->getXpRecompensa() << " de experiencia)\n";
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

    // Indices del inventario que son armas
    vector<int> armas;
    for (size_t i = 0; i < inv.size(); i++)
        if (inv[i].esArma()) armas.push_back(static_cast<int>(i));

    if (armas.empty()) {
        cout << "No tienes armas.\n";
        return false;
    }

    cout << "Con que arma?\n";
    for (size_t k = 0; k < armas.size(); k++) {
        const Objeto& a = inv[armas[k]];
        cout << " " << k + 1 << ") " << a.getNombre() << " (dano " << a.getValor() << ")";
        if (a.golpeaATodos()) cout << ", golpea a todos";
        if (a.esUsoUnico()) cout << ", un solo uso";
        cout << "\n";
    }
    cout << " 0) Volver\n> ";
    int op = leerOpcion(0, static_cast<int>(armas.size()));
    if (op == 0) return false;

    int indiceArma = armas[op - 1];
    jugador.seleccionarArma(indiceArma);
    bool golpeaATodos = inv[indiceArma].golpeaATodos();

    const vector<Enemigo*>& enemigos = vagon.getEnemigos();
    vector<bool> antes = estadoVivos();

    if (golpeaATodos) {
        for (Enemigo* e : enemigos)
            if (e->estaVivo()) jugador.atacar(*e);
    } else {
        // Elegir objetivo entre los enemigos vivos
        vector<int> vivos;
        for (size_t i = 0; i < enemigos.size(); i++)
            if (enemigos[i]->estaVivo()) vivos.push_back(static_cast<int>(i));

        int objetivo = vivos[0];
        if (vivos.size() > 1) {
            cout << "A quien?\n";
            for (size_t k = 0; k < vivos.size(); k++)
                cout << " " << k + 1 << ") " << enemigos[vivos[k]]->getNombre() << "\n";
            cout << "> ";
            objetivo = vivos[leerOpcion(1, static_cast<int>(vivos.size())) - 1];
        }
        jugador.atacar(*enemigos[objetivo]);
    }

    revisarCaidos(antes);
    return true;
}

void Combate::turnoEnemigos() {
    cout << "\n";
    for (Enemigo* e : vagon.getEnemigos()) {
        if (e->estaVivo() && jugador.estaVivo()) e->atacar(jugador);   // polimorfismo
    }
}
