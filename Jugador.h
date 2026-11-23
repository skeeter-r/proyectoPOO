#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <string>
#include "Personaje.h"
#include "Objeto.h"

class Jugador : public Personaje {
private:
    int experiencia;
    int experienciaAlEmpezar;
    int armaSeleccionada;
    bool sueroDesbloqueado;
    std::vector<Objeto> inventario;

public:
    Jugador(const std::string& nombre);

    void atacar(Personaje& objetivo) override;
    void recibirDanio(int cantidad) override;

    void agregarObjeto(const Objeto& obj);
    bool usarSuero();
    bool seleccionarArma(int indice);
    bool tieneChaqueta() const;
    bool tieneSuero() const;
    const std::vector<Objeto>& getInventario() const;

    void ganarExperiencia(int xp);
    void iniciarNivel();
    void descartarIntento();
    void reiniciar();
    void restaurar(int vidas, int experiencia, bool sueroDesbloqueado,
                   const std::vector<Objeto>& inventario);

    int getExperiencia() const;
    bool getSueroDesbloqueado() const;
};

#endif
