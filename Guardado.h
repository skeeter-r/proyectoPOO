#ifndef GUARDADO_H
#define GUARDADO_H

#include <string>

class Juego;

// Guarda y carga la partida en un archivo de texto.
// Solo se puede guardar cuando el vagon actual no tiene enemigos vivos,
// asi que no hace falta guardar el estado de los enemigos.
class Guardado {
public:
    static bool guardar(const Juego& juego, const std::string& ruta);   // false si no pudo escribir
    static bool cargar(Juego& juego, const std::string& ruta);          // false si no existe o es invalido
};

#endif
