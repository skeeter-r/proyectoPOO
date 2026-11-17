#ifndef COMBATE_H
#define COMBATE_H

#include <vector>
#include "Jugador.h"
#include "Vagon.h"

// Reglas del combate dentro de un vagon: el ataque del jugador y el turno de los enemigos.
// El menu y el movimiento entre vagones los maneja Juego.
class Combate {
private:
    Jugador& jugador;
    Vagon& vagon;

    std::vector<bool> estadoVivos() const;                     // quien esta vivo ahora
    void revisarCaidos(const std::vector<bool>& vivosAntes);   // avisa y da experiencia

public:
    Combate(Jugador& jugador, Vagon& vagon);

    bool accionAtacar();      // pide arma y objetivo; true si el jugador gasto su turno
    void turnoEnemigos();     // cada enemigo vivo ataca a su manera (polimorfismo)
};

#endif
