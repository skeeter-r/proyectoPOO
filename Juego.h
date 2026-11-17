#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include "Jugador.h"
#include "Vagon.h"

// Director del juego: crea el nivel, muestra el menu y mueve al jugador entre vagones.
// (Por ahora crea el nivel 1 a mano; mas adelante esa tarea pasara a la clase Tren.)
class Juego {
private:
    Jugador jugador;
    std::vector<Vagon*> vagones;   // vagones[0] = vagon 1 (cabina del Conductor) ... vagones[3] = vagon 4 (inicio)
    int vagonActual;               // indice en el vector

    void crearNivel();
    void liberar();
    void mostrarEstado() const;
    void ejecutarMenu();
    void cambiarDeVagon();
    void recogerObjetos();

public:
    Juego();
    ~Juego();
    Juego(const Juego&) = delete;              // el Juego es dueno de punteros: no se copia
    Juego& operator=(const Juego&) = delete;

    void iniciar();
};

#endif
