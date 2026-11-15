#include "Combate.h"
#include <cstdlib>
#include <iostream>

using namespace std;

namespace {
// Lee un numero entre minimo y maximo. Repite hasta que sea valido.
int leerOpcion(int minimo, int maximo) {
    int opcion;
    while (true) {
        if (cin >> opcion && opcion >= minimo && opcion <= maximo) return opcion;
        if (cin.eof()) {
            cout << "\nFin de la entrada. Saliendo.\n";
            exit(0);
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Opcion invalida. Intenta de nuevo: ";
    }
}
}

Combate::Combate(Jugador& jugador, vector<Enemigo*>& enemigos)
    : jugador(jugador), enemigos(enemigos) {}

bool Combate::hayEnemigosVivos() const {
    for (const Enemigo* e : enemigos)
        if (e->estaVivo()) return true;
    return false;
}

vector<bool> Combate::estadoVivos() const {
    vector<bool> vivos;
    for (const Enemigo* e : enemigos) vivos.push_back(e->estaVivo());
    return vivos;
}

void Combate::revisarCaidos(const vector<bool>& vivosAntes) {
    for (size_t i = 0; i < enemigos.size(); i++) {
        if (vivosAntes[i] && !enemigos[i]->estaVivo()) {
            cout << enemigos[i]->getNombre() << " cae derrotado. (+"
                 << enemigos[i]->getXpRecompensa() << " de experiencia)\n";
            jugador.ganarExperiencia(enemigos[i]->getXpRecompensa());
        }
    }
}

void Combate::mostrarEstado() const {
    cout << "\n----------------------------------------\n";
    cout << "Tus vidas: " << jugador.getVidas() << "/" << jugador.getVidasMax();
    if (jugador.getChaquetaPuesta()) cout << "   (chaqueta puesta)";
    cout << "\nEnemigos:\n";
    for (size_t i = 0; i < enemigos.size(); i++) {
        if (enemigos[i]->estaVivo())
            cout << "  - " << enemigos[i]->getNombre()
                 << " (" << enemigos[i]->getVidas() << " vidas)\n";
    }
}

bool Combate::accionAtacar() {
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
        cout << "\n";
    }
    cout << " 0) Volver\n> ";
    int op = leerOpcion(0, static_cast<int>(armas.size()));
    if (op == 0) return false;

    int indiceArma = armas[op - 1];
    jugador.seleccionarArma(indiceArma);
    bool golpeaATodos = inv[indiceArma].golpeaATodos();

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

bool Combate::accionUsarObjeto() {
    const vector<Objeto>& inv = jugador.getInventario();

    // Indices del inventario que NO son armas (chaqueta, suero)
    vector<int> usables;
    for (size_t i = 0; i < inv.size(); i++)
        if (!inv[i].esArma()) usables.push_back(static_cast<int>(i));

    if (usables.empty()) {
        cout << "No tienes objetos para usar.\n";
        return false;
    }

    cout << "Que objeto usas?\n";
    for (size_t k = 0; k < usables.size(); k++)
        cout << " " << k + 1 << ") " << inv[usables[k]].getNombre()
             << " - " << inv[usables[k]].getDescripcion() << "\n";
    cout << " 0) Volver\n> ";
    int op = leerOpcion(0, static_cast<int>(usables.size()));
    if (op == 0) return false;

    return jugador.usarObjeto(usables[op - 1]);
}

bool Combate::turnoJugador() {
    while (true) {
        cout << "\nQue haces?\n 1) Atacar\n 2) Usar objeto\n 3) Huir\n> ";
        int opcion = leerOpcion(1, 3);
        if (opcion == 1) {
            if (accionAtacar()) return false;      // ataco: termina su turno
        } else if (opcion == 2) {
            if (accionUsarObjeto()) return false;  // uso un objeto: termina su turno
        } else {
            cout << "Huyes del combate.\n";
            return true;
        }
    }
}

void Combate::turnoEnemigos() {
    cout << "\n";
    for (Enemigo* e : enemigos) {
        if (e->estaVivo() && jugador.estaVivo()) e->atacar(jugador);   // polimorfismo
    }
}

ResultadoCombate Combate::iniciar() {
    cout << "=== COMBATE ===\n";
    while (jugador.estaVivo() && hayEnemigosVivos()) {
        mostrarEstado();
        if (turnoJugador()) return ResultadoCombate::HUIDA;
        if (hayEnemigosVivos()) turnoEnemigos();
    }
    return jugador.estaVivo() ? ResultadoCombate::VICTORIA : ResultadoCombate::DERROTA;
}
