#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <string>

class Personaje {
protected:
    std::string nombre;
    int vidas;
    int vidasMax;

public:
    Personaje(const std::string& nombre, int vidasMax);
    virtual ~Personaje() = default;

    virtual void atacar(Personaje& objetivo) = 0;

    virtual void recibirDanio(int cantidad);
    void curar(int cantidad);
    bool estaVivo() const;

    const std::string& getNombre() const;
    int getVidas() const;
    int getVidasMax() const;
};

#endif
