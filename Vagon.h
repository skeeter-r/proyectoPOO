#ifndef VAGON_H
#define VAGON_H

#include <map>
#include <string>
#include <vector>
#include "Enemigo.h"
#include "Objeto.h"

class Vagon {
private:
    int numero;
    std::string descripcion;
    std::vector<Enemigo*> enemigos;
    std::vector<Objeto> objetos;
    std::map<std::string, Vagon*> salidas;

public:
    Vagon(int numero, const std::string& descripcion);

    void conectar(const std::string& direccion, Vagon* destino);
    Vagon* getSalida(const std::string& direccion) const;

    void agregarEnemigo(Enemigo* enemigo);
    const std::vector<Enemigo*>& getEnemigos() const;
    bool hayEnemigosVivos() const;
    std::vector<Enemigo*> vaciarEnemigos();

    void agregarObjeto(const Objeto& objeto);
    bool hayObjetos() const;
    const std::vector<Objeto>& getObjetos() const;
    void vaciarObjetos();
    std::vector<Objeto> recogerObjetos();

    int getNumero() const;
    const std::string& getDescripcion() const;
};

#endif