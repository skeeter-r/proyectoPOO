#ifndef TREN_H
#define TREN_H

#include <vector>
#include "Vagon.h"

class Tren {
public:
    static const int NUM_VAGONES = 5;

private:
    std::vector<Vagon*> vagones;

    void liberar();

public:
    Tren();
    ~Tren();
    Tren(const Tren&) = delete;
    Tren& operator=(const Tren&) = delete;

    void crear();
    void crearEnemigos(int indice);
    void vaciarEnemigos(int indice);

    Vagon* getVagon(int indice) const;
    int getNumVagones() const;
};

#endif
