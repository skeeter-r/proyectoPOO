#include "Juego.h"
#include "Combate.h"
#include "Entrada.h"
#include "Guardado.h"
#include <iostream>
#include <vector>

using namespace std;

namespace {
const string RUTA_GUARDADO = "partida.txt";

enum Accion { ATACAR, SUERO, CAMBIAR, RECOGER, GUARDAR, SALIR };

const char* etiqueta(Accion accion) {
    switch (accion) {
        case ATACAR:  return "Atacar";
        case SUERO:   return "Usar suero vital";
        case CAMBIAR: return "Moverse de vagon";
        case RECOGER: return "Recoger objetos";
        case GUARDAR: return "Guardar partida";
        case SALIR:   return "Salir al menu principal";
    }
    return "";
}
}

Juego::Juego()
    : jugador("Heroe"), vagonActual(0), intentoActivo(false),
      partidaGanada(false), salirAlMenu(false) {
    for (int i = 0; i < Tren::NUM_VAGONES; i++) {
        completado[i] = false;
    }
}

void Juego::mostrarTutorial() {
    cout << "\nVagon 1 de " << Tren::NUM_VAGONES << "\n"
         << tren.getVagon(0)->getDescripcion() << "\n\n"
         << "Como funciona:\n"
         << "- Empiezas cada vagon con 8 vidas.\n"
         << "- Si mueres, regresas al vagon anterior y pierdes la exp de ese intento.\n"
         << "- Tienes que matar a todos los enemigos para poder avanzar.\n"
         << "- Puedes devolverte cuando quieras, pero pierdes la exp del intento.\n"
         << "- Puedes repetir vagones ya pasados para sacar mas exp.\n"
         << "- Puedes guardar cuando el vagon no tenga enemigos.\n"
         << "- En el vagon 5 esta el Conductor; hay que frenarlo.\n\n"
         << "Empiezas con: Navaja (dano 2) y Latigo (dano 1 a todos).\n"
         << "Luego hay una chaqueta y un suero vital.\n"
         << "1. Seguir\n> ";
    leerOpcion(1, 1);
}

void Juego::nuevaPartida() {
    tren.crear();
    for (int i = 0; i < Tren::NUM_VAGONES; i++) {
        completado[i] = false;
    }
    partidaGanada = false;
    intentoActivo = false;
    salirAlMenu = false;
    jugador.reiniciar();

    mostrarTutorial();
    jugador.agregarObjeto(Objeto::crearNavaja());
    jugador.agregarObjeto(Objeto::crearLatigo());

    comenzarIntento(0);
}

bool Juego::cargarPartida() {
    if (!Guardado::cargar(*this, RUTA_GUARDADO)) {
        cout << "No se pudo cargar la partida.\n";
        return false;
    }
    cout << "\nPartida cargada.\n"
         << "\nVagon " << vagonActual + 1 << " de " << Tren::NUM_VAGONES << "\n"
         << tren.getVagon(vagonActual)->getDescripcion() << "\n";
    return true;
}

void Juego::comenzarIntento(int indice) {
    vagonActual = indice;
    tren.crearEnemigos(indice);
    jugador.iniciarNivel();
    intentoActivo = true;
}

void Juego::entrarAVagon(int indice) {
    vagonActual = indice;
    cout << "\nVagon " << indice + 1 << " de " << Tren::NUM_VAGONES << "\n"
         << tren.getVagon(indice)->getDescripcion() << "\n";

    if (completado[indice]) {
        cout << "\nYa pasaste este vagon. Quieres jugarlo otra vez por exp?\n"
             << "1. No, solo pasar\n"
             << "2. Si, jugarlo de nuevo\n> ";
        if (leerOpcion(1, 2) == 2) {
            comenzarIntento(indice);
        } else {
            tren.vaciarEnemigos(indice);
            intentoActivo = false;
        }
    } else {
        comenzarIntento(indice);
    }
}

void Juego::completarNivel() {
    completado[vagonActual] = true;
    intentoActivo = false;
    cout << "\nPasaste el vagon " << vagonActual + 1 << ".\n";
    if (vagonActual == Tren::NUM_VAGONES - 1) {
        partidaGanada = true;
    } else {
        cout << "Ya puedes ir al vagon " << vagonActual + 2 << ".\n";
    }
}

void Juego::perderNivel() {
    cout << "\nTe quedaste sin vidas.\n"
         << "Pierdes la exp de este intento.\n";
    jugador.descartarIntento();
    intentoActivo = false;
    tren.vaciarEnemigos(vagonActual);

    if (vagonActual == 0) {
        cout << "Vuelves a intentar el vagon 1.\n";
        entrarAVagon(0);
    } else {
        cout << "Regresas al vagon anterior.\n";
        entrarAVagon(vagonActual - 1);
    }
}

int Juego::jugarPartida() {
    while (!partidaGanada && !salirAlMenu) {
        mostrarEstado();
        ejecutarMenu();
    }

    if (salirAlMenu) {
        return 1;
    }

    cout << "\nEl Conductor cae y logras frenar el tren.\n"
         << "Ganaste. Exp total: " << jugador.getExperiencia() << "\n"
         << "1. Menu principal\n"
         << "2. Salir\n> ";
    return leerOpcion(1, 2);
}

void Juego::iniciar() {
    while (true) {
        cout << "\nMenu principal\n"
             << "1. Nueva partida\n"
             << "2. Cargar partida\n"
             << "3. Salir\n> ";
        int opcion = leerOpcion(1, 3);

        if (opcion == 3) {
            break;
        }
        if (opcion == 2) {
            if (!cargarPartida()) {
                continue;
            }
        } else {
            nuevaPartida();
        }

        if (jugarPartida() == 2) {
            break;
        }
    }
    cout << "Chao.\n";
}

void Juego::mostrarEstado() const {
    const Vagon* vagon = tren.getVagon(vagonActual);

    cout << "\nVagon " << vagon->getNumero() << "/" << Tren::NUM_VAGONES
         << " | Vidas: " << jugador.getVidas() << "/" << jugador.getVidasMax()
         << " | Exp: " << jugador.getExperiencia() << "\n";

    cout << "Tienes:";
    const vector<Objeto>& inv = jugador.getInventario();
    if (inv.empty()) {
        cout << " nada";
    } else {
        for (int i = 0; i < static_cast<int>(inv.size()); i++) {
            cout << (i == 0 ? " " : ", ") << inv[i].getNombre();
        }
    }
    cout << "\n";

    if (vagon->hayEnemigosVivos()) {
        cout << "Enemigos:\n";
        for (const Enemigo* e : vagon->getEnemigos()) {
            if (e->estaVivo()) {
                cout << "- " << e->getNombre() << " (" << e->getVidas() << " vidas)\n";
            }
        }
    } else {
        cout << "Vagon sin enemigos.\n";
        if (vagon->hayObjetos()) {
            cout << "Hay cosas para recoger.\n";
        }
    }
}

void Juego::ejecutarMenu() {
    Vagon* vagon = tren.getVagon(vagonActual);
    bool hayEnemigos = vagon->hayEnemigosVivos();

    vector<Accion> acciones;
    acciones.push_back(ATACAR);
    acciones.push_back(SUERO);
    acciones.push_back(CAMBIAR);
    if (vagon->hayObjetos() && !hayEnemigos) {
        acciones.push_back(RECOGER);
    }
    if (!hayEnemigos) {
        acciones.push_back(GUARDAR);
    }
    acciones.push_back(SALIR);

    cout << "\nOpciones:\n";
    int idx = 1;
    for (Accion acc : acciones) {
        cout << idx++ << ". " << etiqueta(acc) << "\n";
    }
    cout << "> ";
    int eleccion = leerOpcion(1, static_cast<int>(acciones.size()));

    Combate combate(jugador, *vagon);
    bool gastoTurno = false;

    switch (acciones[eleccion - 1]) {
        case ATACAR:
            gastoTurno = combate.accionAtacar();
            break;
        case SUERO:
            gastoTurno = jugador.usarSuero();
            break;
        case CAMBIAR:
            cambiarDeVagon();
            return;
        case RECOGER:
            recogerObjetos();
            break;
        case GUARDAR:
            if (Guardado::guardar(*this, RUTA_GUARDADO)) {
                cout << "Partida guardada en " << RUTA_GUARDADO << ".\n";
            } else {
                cout << "No se pudo guardar la partida.\n";
            }
            return;
        case SALIR:
            cout << "Si sales al menu principal pierdes lo que no hayas guardado.\n"
                 << "1. Quedarme\n2. Salir\n> ";
            if (leerOpcion(1, 2) == 2) {
                salirAlMenu = true;
            }
            return;
    }

    if (gastoTurno && vagon->hayEnemigosVivos()) {
        combate.turnoEnemigos();
    }

    if (!jugador.estaVivo()) {
        perderNivel();
    } else if (intentoActivo && !vagon->hayEnemigosVivos()) {
        completarNivel();
    }
}

void Juego::cambiarDeVagon() {
    Vagon* actual = tren.getVagon(vagonActual);

    cout << "\nA donde vas?\n"
         << "1. Quedarme aqui\n"
         << "2. Vagon anterior";
    if (vagonActual > 0) {
        cout << " (vagon " << vagonActual << (completado[vagonActual - 1] ? ", listo" : "") << ")";
    } else {
        cout << " (no hay)";
    }
    cout << "\n3. Siguiente vagon";
    if (vagonActual < Tren::NUM_VAGONES - 1) {
        cout << " (vagon " << vagonActual + 2 << (completado[vagonActual + 1] ? ", listo" : "") << ")";
    } else {
        cout << " (no hay)";
    }
    cout << "\n> ";
    int opcion = leerOpcion(1, 3);

    if (opcion == 1) {
        return;
    }

    if (opcion == 2) {
        if (vagonActual == 0) {
            cout << "No hay vagon atras.\n";
            return;
        }
        if (intentoActivo && actual->hayEnemigosVivos()) {
            cout << "Si te devuelves ahora pierdes la exp de este intento y se reinicia el vagon.\n"
                 << "1. Quedarme\n2. Devolverme\n> ";
            if (leerOpcion(1, 2) == 1) {
                return;
            }
            jugador.descartarIntento();
            intentoActivo = false;
            tren.vaciarEnemigos(vagonActual);
        }
        entrarAVagon(vagonActual - 1);
        return;
    }

    if (vagonActual == Tren::NUM_VAGONES - 1) {
        cout << "Ya estas en el ultimo vagon.\n";
        return;
    }
    if (actual->hayEnemigosVivos()) {
        cout << "Mata a los enemigos primero para avanzar.\n";
        return;
    }
    entrarAVagon(vagonActual + 1);
}

void Juego::recogerObjetos() {
    vector<Objeto> tomados = tren.getVagon(vagonActual)->recogerObjetos();
    for (const Objeto& o : tomados) {
        jugador.agregarObjeto(o);
        cout << "Agarras: " << o.getNombre() << " (" << o.getDescripcion() << ")\n";
    }
}
