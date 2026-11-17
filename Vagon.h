#ifndef VAGON_H
#define VAGON_H

#include <map>
#include <string>
#include <vector>
#include "Enemigo.h"
#include "Objeto.h"

// Un vagon del tren (una "habitacion" del mapa). Cada nivel tiene 4 vagones.
// El vagon 1 es la cabina del Conductor (el primero del tren) y el vagon 4 es el ultimo,
// donde empieza el jugador. "adelante" lleva hacia el vagon 1 y "atras" hacia el vagon 4.
class Vagon {
private:
    int numero;                                // 1 (cabina del Conductor) a 4 (inicio del jugador)
    std::string descripcion;
    std::vector<Enemigo*> enemigos;            // AGREGACION: los crea y libera quien arma el nivel (Juego por ahora, luego Tren), no el Vagon
    std::vector<Objeto> objetos;               // COMPOSICION: objetos que se pueden tomar aqui
    std::map<std::string, Vagon*> salidas;     // ASOCIACION: "adelante" y "atras"

public:
    Vagon(int numero, const std::string& descripcion);

    // Salidas
    void conectar(const std::string& direccion, Vagon* destino);
    Vagon* getSalida(const std::string& direccion) const;   // nullptr si no existe

    // Enemigos
    void agregarEnemigo(Enemigo* enemigo);
    const std::vector<Enemigo*>& getEnemigos() const;
    bool hayEnemigosVivos() const;

    // Objetos
    void agregarObjeto(const Objeto& objeto);
    bool hayObjetos() const;
    // El jugador "toma" los objetos del vagon. Devuelve un vector vacio si aun hay enemigos vivos.
    std::vector<Objeto> recogerObjetos();

    int getNumero() const;
    const std::string& getDescripcion() const;
    void describir() const;                 // imprime la descripcion y las salidas
};

#endif
