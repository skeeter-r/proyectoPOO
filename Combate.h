#ifndef COMBATE_H
#define COMBATE_H

#include <vector>
#include "Jugador.h"
#include "Enemigo.h"

enum class ResultadoCombate { VICTORIA, DERROTA, HUIDA };

// Maneja una pelea por turnos entre el jugador y una lista de enemigos.
// El jugador siempre actua primero; despues atacan los enemigos vivos.
class Combate {
private:
    Jugador& jugador;
    std::vector<Enemigo*>& enemigos;   // Combate NO los crea ni los borra

    bool hayEnemigosVivos() const;
    std::vector<bool> estadoVivos() const;                     // quien esta vivo ahora
    void revisarCaidos(const std::vector<bool>& vivosAntes);   // avisa y da experiencia

    void mostrarEstado() const;
    bool turnoJugador();            // true si el jugador huyo
    bool accionAtacar();            // true si gasto el turno
    bool accionUsarObjeto();        // true si gasto el turno
    void turnoEnemigos();

public:
    Combate(Jugador& jugador, std::vector<Enemigo*>& enemigos);
    ResultadoCombate iniciar();
};

#endif
