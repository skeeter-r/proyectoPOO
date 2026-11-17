#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <string>

// Clase base abstracta: define la interfaz comun de Jugador y Enemigo.
// Las vidas son siempre numeros enteros.
class Personaje {
protected:
    std::string nombre;
    int vidas;
    int vidasMax;

public:
    Personaje(const std::string& nombre, int vidasMax);
    virtual ~Personaje() = default;   // destructor virtual: necesario para borrar por puntero base

    // Metodo virtual puro: cada hijo define su propio ataque (POLIMORFISMO)
    virtual void atacar(Personaje& objetivo) = 0;

    virtual void recibirDanio(int cantidad);
    void curar(int cantidad);          // sin superar vidasMax
    bool estaVivo() const;

    const std::string& getNombre() const;
    int getVidas() const;
    int getVidasMax() const;
};

#endif
