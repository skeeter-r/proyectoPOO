#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include "Jugador.h"
#include "Vagon.h"

// Director del juego: menu principal, creacion de los 5 vagones (niveles) y flujo de juego.
// (Por ahora arma los vagones a mano; mas adelante esa tarea pasara a la clase Tren.)
class Juego {
private:
    static const int NUM_VAGONES = 5;

    Jugador jugador;
    std::vector<Vagon*> vagones;     // vagones[0] = vagon 1 ... vagones[4] = vagon 5 (Conductor)
    bool completado[NUM_VAGONES];    // true si el vagon ya se completo alguna vez
    int vagonActual;                 // indice en el vector
    bool intentoActivo;              // true mientras se juega un vagon con enemigos
    bool partidaGanada;

    // Creacion y limpieza
    void crearVagones();
    void liberar();
    void crearEnemigos(int indice);  // enemigos de un intento nuevo en ese vagon
    void vaciarVagon(int indice);    // quita y libera los enemigos del vagon

    // Flujo de la partida
    void nuevaPartida();
    void mostrarTutorial();
    void entrarAVagon(int indice);
    void comenzarIntento(int indice);
    void completarNivel();
    void perderNivel();

    // Menu dentro del tren
    void mostrarEstado() const;
    void ejecutarMenu();
    void cambiarDeVagon();
    void recogerObjetos();

    int jugarPartida();              // devuelve la opcion elegida al ganar (1 = menu, 2 = salir)

public:
    Juego();
    ~Juego();
    Juego(const Juego&) = delete;              // el Juego es dueno de punteros: no se copia
    Juego& operator=(const Juego&) = delete;

    void iniciar();
};

#endif
