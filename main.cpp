// Version 0 (muy basica): un jugador, un Asesino, dos armas y el menu de combate.
#include <iostream>
#include <vector>
#include "Jugador.h"
#include "Asesino.h"
#include "Combate.h"

using namespace std;

int main() {
    // El jugador con dos armas y dos objetos
    Jugador jugador("Heroe");
    jugador.agregarObjeto(Objeto::crearNavaja());
    jugador.agregarObjeto(Objeto::crearLatigo());
    jugador.agregarObjeto(Objeto::crearChaqueta());
    jugador.agregarObjeto(Objeto::crearSueroVital());

    // Un solo enemigo, creado con memoria dinamica
    vector<Enemigo*> enemigos;
    enemigos.push_back(new Asesino());

    cout << "Despiertas en un tren que avanza sin control.\n";
    cout << "Un Asesino te bloquea el paso. Tienes que pelear.\n\n";

    Combate combate(jugador, enemigos);
    ResultadoCombate resultado = combate.iniciar();

    cout << "\n========================================\n";
    switch (resultado) {
        case ResultadoCombate::VICTORIA: cout << "Ganaste! Experiencia total: " << jugador.getExperiencia() << "\n"; break;
        case ResultadoCombate::DERROTA:  cout << "Perdiste...\n"; break;
        case ResultadoCombate::HUIDA:    cout << "Escapaste del Asesino.\n"; break;
    }

    // Liberar la memoria de los enemigos
    for (Enemigo* e : enemigos) delete e;
    enemigos.clear();
    return 0;
}
