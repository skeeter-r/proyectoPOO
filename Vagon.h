#ifndef VAGON_H
#define VAGON_H

#include <map>
#include <string>
#include <vector>
#include "Enemigo.h"
#include "Objeto.h"

// Un vagon del tren = un nivel del juego (5 en total). El vagon 1 es el primero que
// recorre el jugador y el vagon 5 es la cabina del Conductor.
class Vagon {
private:
    int numero;                                // 1 a 5
    std::string descripcion;
    std::vector<Enemigo*> enemigos;            // AGREGACION: los crea y libera quien arma el nivel (Juego por ahora, luego Tren)
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
    std::vector<Enemigo*> vaciarEnemigos();   // los devuelve y deja el vagon sin enemigos (quien los recibe los libera)

    // Objetos
    void agregarObjeto(const Objeto& objeto);
    bool hayObjetos() const;
    // El jugador "toma" los objetos del vagon. Devuelve un vector vacio si aun hay enemigos vivos.
    std::vector<Objeto> recogerObjetos();

    int getNumero() const;
    const std::string& getDescripcion() const;
};

#endif
