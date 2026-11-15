#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <string>
#include "Personaje.h"
#include "Objeto.h"

class Jugador : public Personaje {
private:
    int nivel;                         // empieza en 1; sube +1 al derrotar al Conductor
    int experiencia;                   // puntaje acumulado
    bool chaquetaPuesta;               // true hasta que termine el nivel
    int armaSeleccionada;              // indice en el inventario
    std::vector<Objeto> inventario;    // COMPOSICION: el inventario muere con el jugador

public:
    Jugador(const std::string& nombre);   // nivel 1: 8 vidas maximas

    // Ataca con el arma seleccionada. Si es de un solo uso (nudilleras), se elimina del inventario.
    // El latigo (golpea a todos) lo gestiona Combate llamando a atacar() por cada enemigo.
    void atacar(Personaje& objetivo) override;
    // Con la chaqueta puesta, cada golpe se reduce en 1 (minimo 1)
    void recibirDanio(int cantidad) override;

    // Inventario
    void agregarObjeto(const Objeto& obj);
    bool usarObjeto(int indice);        // chaqueta (se pone) o suero (cura); se elimina del inventario
    bool seleccionarArma(int indice);
    bool tieneNudilleras() const;       // para el Ladron
    bool quitarNudilleras();            // para el Ladron: true si las tenia
    const std::vector<Objeto>& getInventario() const;

    // Progreso
    void ganarExperiencia(int xp);
    void subirNivel();                  // nivel + 1 y vidasMax + 1 (8, 9 y 10 vidas en los niveles 1, 2 y 3)
    void iniciarNivel();                // vidas = vidasMax, inventario = kit inicial, sin chaqueta

    int getNivel() const;
    int getExperiencia() const;
    bool getChaquetaPuesta() const;
};

// Kit con el que el jugador empieza cada nivel: Navaja, Latigo y Nudilleras.
// La chaqueta y el suero NO estan en el kit: se encuentran en los vagones.

#endif
