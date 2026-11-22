#include "Juego.h"
#include "Combate.h"
#include "Entrada.h"
#include "Perro.h"
#include "Asesino.h"
#include "Conductor.h"
#include <iostream>

using namespace std;

Juego::Juego()
    : jugador("Heroe"), vagonActual(0), intentoActivo(false), partidaGanada(false) {
    for (int i = 0; i < NUM_VAGONES; i++) completado[i] = false;
}

Juego::~Juego() { liberar(); }

void Juego::liberar() {
    for (Vagon* v : vagones) {
        for (Enemigo* e : v->vaciarEnemigos()) delete e;
        delete v;
    }
    vagones.clear();
}

void Juego::crearVagones() {
    liberar();

    const string descripciones[NUM_VAGONES] = {
        "Abres los ojos en el ultimo vagon. El tren avanza sin control.\n"
        "Un perro rabioso ladra con furia entre los asientos.",
        "Las luces apenas funcionan. Entre las sombras, algo metalico brilla: un cuchillo.\n"
        "Sobre un asiento hay una chaqueta de cuero.",
        "Dos siluetas te bloquean el paso. Las dos afilan una hoja.",
        "Un perro rabioso vigila junto a un hombre armado.\n"
        "Al fondo hay una caja con una cruz roja.",
        "La cabina tiembla. Frente a los controles, el Conductor ni siquiera se voltea:\n"
        "\"Llegaste tarde. Nadie baja de este tren\"."
    };

    for (int i = 0; i < NUM_VAGONES; i++) vagones.push_back(new Vagon(i + 1, descripciones[i]));

    // "adelante" lleva al vagon siguiente (hacia la cabina); "atras" al anterior
    for (int i = 0; i < NUM_VAGONES; i++) {
        if (i > 0) vagones[i]->conectar("atras", vagones[i - 1]);
        if (i < NUM_VAGONES - 1) vagones[i]->conectar("adelante", vagones[i + 1]);
    }

    // Objetos: la chaqueta en el vagon 2 y el suero en el vagon 4
    vagones[1]->agregarObjeto(Objeto::crearChaqueta());
    vagones[3]->agregarObjeto(Objeto::crearSueroVital());
}

void Juego::vaciarVagon(int indice) {
    for (Enemigo* e : vagones[indice]->vaciarEnemigos()) delete e;
}

void Juego::crearEnemigos(int indice) {
    vaciarVagon(indice);
    Vagon* v = vagones[indice];
    switch (indice) {
        case 0: v->agregarEnemigo(new Perro()); break;
        case 1: v->agregarEnemigo(new Asesino()); break;
        case 2: v->agregarEnemigo(new Asesino()); v->agregarEnemigo(new Asesino()); break;
        case 3: v->agregarEnemigo(new Perro()); v->agregarEnemigo(new Asesino()); break;
        case 4: v->agregarEnemigo(new Conductor()); break;
    }
}

void Juego::mostrarTutorial() {
    cout << "\n=== VAGON 1 de " << NUM_VAGONES << " ===\n"
         << vagones[0]->getDescripcion() << "\n\n"
         << "REGLAS\n"
         << " - Cada vagon es un nivel. Tienes 8 vidas al empezar cada uno.\n"
         << " - Si pierdes todas tus vidas, vuelves al vagon anterior y pierdes la experiencia de ese intento.\n"
         << " - No puedes avanzar hasta derrotar a todos los enemigos del vagon.\n"
         << " - Puedes retroceder cuando quieras, pero perderas la experiencia de ese intento.\n"
         << " - Los vagones que ya completaste se pueden volver a jugar para ganar mas experiencia.\n"
         << " - En el vagon 5 esta el Conductor, que quiere estrellar el tren. Detenlo.\n\n"
         << "INVENTARIO\n"
         << " Recibes: Navaja (dano 2) y Latigo (dano 1 a todos los enemigos).\n"
         << " Mas adelante encontraras una chaqueta y un suero vital.\n"
         << " 1) Continuar\n> ";
    leerOpcion(1, 1);
}

void Juego::nuevaPartida() {
    crearVagones();
    for (int i = 0; i < NUM_VAGONES; i++) completado[i] = false;
    partidaGanada = false;
    intentoActivo = false;
    jugador.reiniciar();

    mostrarTutorial();
    jugador.agregarObjeto(Objeto::crearNavaja());
    jugador.agregarObjeto(Objeto::crearLatigo());

    vagonActual = 0;
    comenzarIntento(0);
}

void Juego::comenzarIntento(int indice) {
    vagonActual = indice;
    crearEnemigos(indice);
    jugador.iniciarNivel();
    intentoActivo = true;
}

void Juego::entrarAVagon(int indice) {
    vagonActual = indice;
    cout << "\n=== VAGON " << indice + 1 << " de " << NUM_VAGONES << " ===\n"
         << vagones[indice]->getDescripcion() << "\n";

    if (completado[indice]) {
        cout << "\nYa completaste este vagon. Quieres volver a jugarlo? (suma experiencia)\n"
             << " 1) No, solo entrar\n"
             << " 2) Si, jugarlo de nuevo\n> ";
        if (leerOpcion(1, 2) == 2) {
            comenzarIntento(indice);
        } else {
            vaciarVagon(indice);       // entra a un vagon vacio
            intentoActivo = false;
        }
    } else {
        comenzarIntento(indice);
    }
}

void Juego::completarNivel() {
    completado[vagonActual] = true;
    intentoActivo = false;
    cout << "\nVagon " << vagonActual + 1 << " completado.\n";
    if (vagonActual == NUM_VAGONES - 1) {
        partidaGanada = true;
    } else {
        cout << "Ya puedes avanzar al vagon " << vagonActual + 2 << ".\n";
    }
}

void Juego::perderNivel() {
    cout << "\nTodo se oscurece... Te quedaste sin vidas.\n"
         << "Pierdes la experiencia de este intento.\n";
    jugador.descartarIntento();
    intentoActivo = false;
    vaciarVagon(vagonActual);

    if (vagonActual == 0) {
        cout << "Reintentas el vagon 1.\n";
        entrarAVagon(0);
    } else {
        cout << "Vuelves al vagon anterior.\n";
        entrarAVagon(vagonActual - 1);
    }
}

void Juego::mostrarEstado() const {
    const Vagon* vagon = vagones[vagonActual];

    cout << "\n----------------------------------------\n";
    cout << "Vagon " << vagon->getNumero() << " de " << NUM_VAGONES
         << " | Vidas: " << jugador.getVidas() << "/" << jugador.getVidasMax()
         << " | Experiencia: " << jugador.getExperiencia() << "\n";

    cout << "Inventario:";
    const vector<Objeto>& inv = jugador.getInventario();
    for (size_t i = 0; i < inv.size(); i++) cout << (i == 0 ? " " : ", ") << inv[i].getNombre();
    if (inv.empty()) cout << " (vacio)";
    cout << "\n";

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
        case 3: cambiarDeVagon(); return;   // puede cambiar de vagon: no seguir con el anterior
        case 4: recogerObjetos(); break;
    }

    // Si el jugador gasto su turno y quedan enemigos, ellos responden
    if (gastoTurno && vagon->hayEnemigosVivos()) combate.turnoEnemigos();

    if (!jugador.estaVivo()) {
        perderNivel();
    } else if (intentoActivo && !vagon->hayEnemigosVivos()) {
        completarNivel();
    }
}

void Juego::cambiarDeVagon() {
    Vagon* actual = vagones[vagonActual];

    cout << "Adonde vas?\n 1) Volver\n";
    cout << " 2) Vagon anterior";
    if (vagonActual > 0) cout << " (vagon " << vagonActual << (completado[vagonActual - 1] ? ", completado" : "") << ")";
    else cout << " (no hay)";
    cout << "\n 3) Vagon siguiente";
    if (vagonActual < NUM_VAGONES - 1)
        cout << " (vagon " << vagonActual + 2 << (completado[vagonActual + 1] ? ", completado" : "") << ")";
    else cout << " (no hay)";
    cout << "\n> ";
    int opcion = leerOpcion(1, 3);

    if (opcion == 1) return;

    if (opcion == 2) {
        if (vagonActual == 0) {
            cout << "No hay un vagon anterior.\n";
            return;
        }
        // Retroceder en medio de una pelea = abandonar el intento
        if (intentoActivo && actual->hayEnemigosVivos()) {
            cout << "Si retrocedes ahora perderas la experiencia de este intento y el vagon se reiniciara.\n"
                 << " 1) Volver\n 2) Retroceder\n> ";
            if (leerOpcion(1, 2) == 1) return;
            jugador.descartarIntento();
            intentoActivo = false;
            vaciarVagon(vagonActual);
        }
        entrarAVagon(vagonActual - 1);
        return;
    }

    // opcion == 3: avanzar
    if (vagonActual == NUM_VAGONES - 1) {
        cout << "No hay mas vagones adelante.\n";
        return;
    }
    if (actual->hayEnemigosVivos()) {
        cout << "Primero derrota a tus enemigos para cambiar de vagon.\n";
        return;
    }
    entrarAVagon(vagonActual + 1);
}

void Juego::recogerObjetos() {
    vector<Objeto> tomados = vagones[vagonActual]->recogerObjetos();
    for (const Objeto& o : tomados) {
        jugador.agregarObjeto(o);
        cout << "Recoges: " << o.getNombre() << " (" << o.getDescripcion() << ")\n";
    }
}

int Juego::jugarPartida() {
    while (!partidaGanada) {
        mostrarEstado();
        ejecutarMenu();
    }

    cout << "\n========================================\n"
         << "El Conductor cae. Tiras de la palanca y el tren frena hasta detenerse.\n"
         << "Ganaste! Experiencia total: " << jugador.getExperiencia() << "\n"
         << " 1) Volver al menu principal\n"
         << " 2) Salir\n> ";
    return leerOpcion(1, 2);
}

void Juego::iniciar() {
    while (true) {
        cout << "\n=== ULTIMO VAGON ===\n"
             << " 1) Nueva partida\n"
             << " 2) Cargar partida\n"
             << " 3) Salir\n> ";
        int opcion = leerOpcion(1, 3);

        if (opcion == 3) break;
        if (opcion == 2) {
            cout << "Todavia no esta disponible.\n";
            continue;
        }

        nuevaPartida();
        if (jugarPartida() == 2) break;
    }
    cout << "Hasta luego!\n";
}
