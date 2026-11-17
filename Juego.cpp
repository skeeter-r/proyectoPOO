#include "Juego.h"
#include "Combate.h"
#include "Entrada.h"
#include "Ladron.h"
#include "Asesino.h"
#include "Conductor.h"
#include <iostream>

using namespace std;

Juego::Juego() : jugador("Heroe"), vagonActual(3) {}

Juego::~Juego() { liberar(); }

void Juego::liberar() {
    for (Vagon* v : vagones) {
        for (Enemigo* e : v->getEnemigos()) delete e;
        delete v;
    }
    vagones.clear();
}

void Juego::crearNivel() {
    liberar();

    const string descripciones[4] = {
        "La cabina tiembla. Frente a los controles, el Conductor ni siquiera se voltea:\n"
        "\"Llegaste tarde. Nadie baja de este tren\".",
        "Dos figuras te esperan. Uno hace sonar sus llaves; el otro afila una hoja.",
        "Las luces apenas funcionan. Entre las sombras, algo metalico brilla: un cuchillo.",
        "Abres los ojos en el ultimo vagon. El tren vibra con fuerza y las luces parpadean.\n"
        "Un hombre encorvado revisa los asientos y, al verte, sonrie y se acerca a tus bolsillos."
    };

    for (int i = 0; i < 4; i++) vagones.push_back(new Vagon(i + 1, descripciones[i]));

    // "adelante" lleva hacia la cabina del Conductor (vagon 1); "atras" hacia el vagon 4
    for (int i = 0; i < 4; i++) {
        if (i > 0) vagones[i]->conectar("adelante", vagones[i - 1]);
        if (i < 3) vagones[i]->conectar("atras", vagones[i + 1]);
    }

    // Nivel 1
    vagones[3]->agregarEnemigo(new Ladron());
    vagones[3]->agregarObjeto(Objeto::crearChaqueta());

    vagones[2]->agregarEnemigo(new Asesino());

    vagones[1]->agregarEnemigo(new Ladron());
    vagones[1]->agregarEnemigo(new Asesino());
    vagones[1]->agregarObjeto(Objeto::crearSueroVital());

    vagones[0]->agregarEnemigo(new Conductor(6));
}

void Juego::mostrarEstado() const {
    const Vagon* vagon = vagones[vagonActual];

    cout << "\n----------------------------------------\n";
    cout << "Vagon " << vagon->getNumero()
         << " | Vidas: " << jugador.getVidas() << "/" << jugador.getVidasMax()
         << " | Chaqueta: " << (jugador.tieneChaqueta() ? "si" : "no") << "\n";

    if (vagon->hayEnemigosVivos()) {
        cout << "Enemigos:\n";
        for (const Enemigo* e : vagon->getEnemigos())
            if (e->estaVivo())
                cout << "  - " << e->getNombre() << " (" << e->getVidas() << " vidas)\n";
    } else {
        cout << "No hay enemigos en este vagon.\n";
        if (vagon->hayObjetos()) cout << "Hay objetos para recoger.\n";
    }
}

void Juego::ejecutarMenu() {
    Vagon* vagon = vagones[vagonActual];
    bool puedeRecoger = vagon->hayObjetos() && !vagon->hayEnemigosVivos();

    cout << "\nQue haces?\n 1) Atacar\n 2) Usar suero vital\n 3) Cambiar de vagon\n";
    if (puedeRecoger) cout << " 4) Recoger objetos\n";
    cout << "> ";
    int opcion = leerOpcion(1, puedeRecoger ? 4 : 3);

    Combate combate(jugador, *vagon);
    bool gastoTurno = false;

    switch (opcion) {
        case 1: gastoTurno = combate.accionAtacar(); break;
        case 2: gastoTurno = jugador.usarSuero(); break;
        case 3: cambiarDeVagon(); break;
        case 4: recogerObjetos(); break;
    }

    // Si el jugador gasto su turno y quedan enemigos, ellos responden
    if (gastoTurno && vagon->hayEnemigosVivos()) combate.turnoEnemigos();
}

void Juego::cambiarDeVagon() {
    Vagon* actual = vagones[vagonActual];

    cout << "Adonde vas?\n"
         << " 1) Vagon anterior (hacia la cola)\n"
         << " 2) Vagon siguiente (hacia la cabina del Conductor)\n"
         << " 0) Volver\n> ";
    int opcion = leerOpcion(0, 2);
    if (opcion == 0) return;

    Vagon* destino = nullptr;
    if (opcion == 1) {
        destino = actual->getSalida("atras");
        if (destino == nullptr) {
            cout << "No hay un vagon anterior.\n";
            return;
        }
    } else {
        if (actual->hayEnemigosVivos()) {
            cout << "Primero derrota a tus enemigos para cambiar de vagon.\n";
            return;
        }
        destino = actual->getSalida("adelante");
        if (destino == nullptr) {
            cout << "No hay mas vagones adelante.\n";
            return;
        }
    }

    vagonActual = destino->getNumero() - 1;
    cout << "\n";
    destino->describir();
}

void Juego::recogerObjetos() {
    vector<Objeto> tomados = vagones[vagonActual]->recogerObjetos();
    for (const Objeto& o : tomados) {
        jugador.agregarObjeto(o);
        cout << "Recoges: " << o.getNombre() << " (" << o.getDescripcion() << ")\n";
    }
}

void Juego::iniciar() {
    cout << "ULTIMO VAGON\n";
    cout << "Un tren avanza sin control. En el primer vagon, el Conductor quiere estrellarlo.\n";
    cout << "Recorre los vagones, sobrevive y detenlo.\n\n";

    jugador.iniciarNivel();
    crearNivel();
    vagonActual = 3;   // empieza en el vagon 4
    vagones[vagonActual]->describir();

    // El nivel termina cuando el Conductor (vagon 1) cae, o cuando el jugador muere
    while (jugador.estaVivo() && vagones[0]->hayEnemigosVivos()) {
        mostrarEstado();
        ejecutarMenu();
    }

    cout << "\n========================================\n";
    if (jugador.estaVivo()) {
        cout << "El Conductor cae. Tiras de la palanca y el tren frena hasta detenerse.\n";
        cout << "Ganaste! Experiencia total: " << jugador.getExperiencia() << "\n";
    } else {
        cout << "Todo se oscurece... Perdiste.\n";
    }
}
