#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <string>
#include "Personaje.h"
#include "Objeto.h"

class Jugador : public Personaje {
private:
    int experiencia;                   // puntaje acumulado
    int experienciaAlEmpezar;          // XP al empezar el intento: se recupera si pierde o abandona
    int armaSeleccionada;              // indice en el inventario
    bool sueroDesbloqueado;            // true desde que recoge el suero por primera vez
    std::vector<Objeto> inventario;    // COMPOSICION: el inventario muere con el jugador

public:
    Jugador(const std::string& nombre);   // 8 vidas

    // Ataca con el arma seleccionada.
    // El latigo (golpea a todos) lo gestiona Combate llamando a atacar() por cada enemigo.
    void atacar(Personaje& objetivo) override;
    // Si la chaqueta esta en el inventario, cada golpe se reduce en 1 (minimo 1)
    void recibirDanio(int cantidad) override;

    // Inventario
    void agregarObjeto(const Objeto& obj);
    bool usarSuero();                   // solo funciona si cura TODAS las vidas que otorga
    bool seleccionarArma(int indice);
    bool tieneChaqueta() const;
    bool tieneSuero() const;
    const std::vector<Objeto>& getInventario() const;

    // Progreso
    void ganarExperiencia(int xp);
    void iniciarNivel();                // empieza un intento: vidas al maximo, suero repuesto (si ya lo desbloqueo)
    void descartarIntento();            // pierde o abandona: recupera la XP del inicio y las vidas
    void reiniciar();                   // nueva partida: todo en cero, inventario vacio

    int getExperiencia() const;
};

#endif
