#include "Entrada.h"
#include <cstdlib>
#include <iostream>

int leerOpcion(int minimo, int maximo) {
    int opcion;
    while (true) {
        if (std::cin >> opcion && opcion >= minimo && opcion <= maximo) return opcion;
        if (std::cin.eof()) {
            std::cout << "\nFin de la entrada. Saliendo.\n";
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Opcion invalida. Intenta de nuevo: ";
    }
}
