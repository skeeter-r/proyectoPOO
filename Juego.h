#ifndef JUEGO_H
#define JUEGO_H

#include "Jugador.h"
#include "Tren.h"

class Juego {
    friend class Guardado;

private:
    Jugador jugador;
    Tren tren;
    bool completado[Tren::NUM_VAGONES];
    int vagonActual;
    bool intentoActivo;
    bool partidaGanada;
    bool salirAlMenu;

    void nuevaPartida();
    bool cargarPartida();
    void mostrarTutorial();
    void entrarAVagon(int indice);
    void comenzarIntento(int indice);
    void completarNivel();
    void perderNivel();
    int jugarPartida();

    void mostrarEstado() const;
    void ejecutarMenu();
    void cambiarDeVagon();
    void recogerObjetos();

public:
    Juego();
    void iniciar();
};

#endif
