#ifndef OBJETO_H
#define OBJETO_H

#include <string>

// Cada tipo de objeto del juego. El comportamiento especial se deduce del tipo:
//   LATIGO     -> golpea a todos los enemigos del vagon
//   NUDILLERAS -> arma de un solo uso (el Ladron puede robarla)
//   CHAQUETA   -> reduce 1 el dano de cada golpe recibido (minimo 1)
//   SUERO_VITAL-> +4 vidas, un solo uso
enum class TipoObjeto { NAVAJA, LATIGO, NUDILLERAS, CHAQUETA, SUERO_VITAL };

class Objeto {
private:
    std::string nombre;
    std::string descripcion;
    TipoObjeto tipo;
    int valor;   // dano (armas), reduccion (chaqueta) o vidas que cura (suero)

public:
    Objeto(const std::string& nombre, const std::string& descripcion,
           TipoObjeto tipo, int valor);

    // Getters (encapsulamiento: los atributos son privados)
    const std::string& getNombre() const;
    const std::string& getDescripcion() const;
    TipoObjeto getTipo() const;
    int getValor() const;

    bool esArma() const;         // navaja, latigo o nudilleras
    bool golpeaATodos() const;   // solo el latigo
    bool esUsoUnico() const;     // solo las nudilleras

    // Una linea para el archivo de guardado: el nombre del objeto
    std::string serializar() const;

    // "Fabricas" con los objetos del juego
    static Objeto crearNavaja();      // dano 2
    static Objeto crearLatigo();      // dano 1 a todos
    static Objeto crearNudilleras();  // dano 4, un solo uso
    static Objeto crearChaqueta();    // reduce 1 el dano recibido
    static Objeto crearSueroVital();  // +4 vidas
};

#endif
