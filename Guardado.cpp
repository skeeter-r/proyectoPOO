#include "Guardado.h"
#include "Juego.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace {
const string ENCABEZADO = "TREN_A_LA_DERIVA";

vector<string> dividir(const string& texto, char separador) {
    vector<string> partes;
    string actual;
    for (char c : texto) {
        if (c == separador) {
            if (!actual.empty()) {
                partes.push_back(actual);
            }
            actual.clear();
        } else {
            actual += c;
        }
    }
    if (!actual.empty()) {
        partes.push_back(actual);
    }
    return partes;
}

string unir(const vector<Objeto>& objetos) {
    string texto;
    for (int i = 0; i < static_cast<int>(objetos.size()); i++) {
        if (i > 0) {
            texto += ";";
        }
        texto += objetos[i].serializar();
    }
    return texto;
}

bool leerObjetos(const string& texto, vector<Objeto>& salida) {
    salida.clear();
    for (const string& nombre : dividir(texto, ';')) {
        Objeto objeto = Objeto::crearNavaja();
        if (!Objeto::desdeNombre(nombre, objeto)) {
            return false;
        }
        salida.push_back(objeto);
    }
    return true;
}
}

bool Guardado::guardar(const Juego& juego, const string& ruta) {
    ofstream archivo(ruta);
    if (!archivo) {
        return false;
    }

    const Jugador& jugador = juego.jugador;
    int total = juego.tren.getNumVagones();

    archivo << ENCABEZADO << "\n";
    archivo << "vagonActual=" << juego.vagonActual + 1 << "\n";
    archivo << "vidas=" << jugador.getVidas() << "\n";
    archivo << "experiencia=" << jugador.getExperiencia() << "\n";
    archivo << "sueroDesbloqueado=" << (jugador.getSueroDesbloqueado() ? 1 : 0) << "\n";

    archivo << "completado=";
    for (int i = 0; i < total; i++) {
        archivo << (juego.completado[i] ? 1 : 0) << (i < total - 1 ? "," : "");
    }
    archivo << "\n";

    archivo << "inventario=" << unir(jugador.getInventario()) << "\n";
    for (int i = 0; i < total; i++) {
        archivo << "objetos" << i + 1 << "=" << unir(juego.tren.getVagon(i)->getObjetos()) << "\n";
    }

    return archivo.good();
}

bool Guardado::cargar(Juego& juego, const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo) {
        return false;
    }

    string linea;
    if (!getline(archivo, linea) || linea != ENCABEZADO) {
        return false;
    }

    map<string, string> datos;
    while (getline(archivo, linea)) {
        size_t posicion = linea.find('=');
        if (posicion == string::npos) {
            continue;
        }
        datos[linea.substr(0, posicion)] = linea.substr(posicion + 1);
    }

    const char* obligatorias[] = {"vagonActual", "vidas", "experiencia", "sueroDesbloqueado",
                                  "completado", "inventario"};
    for (const char* clave : obligatorias) {
        if (datos.count(clave) == 0) {
            return false;
        }
    }

    const int total = Tren::NUM_VAGONES;
    int vagon = 0, vidas = 0, experiencia = 0, suero = 0;
    try {
        vagon = stoi(datos.at("vagonActual"));
        vidas = stoi(datos.at("vidas"));
        experiencia = stoi(datos.at("experiencia"));
        suero = stoi(datos.at("sueroDesbloqueado"));
    } catch (...) {
        return false;
    }

    if (vagon < 1 || vagon > total) {
        return false;
    }
    if (vidas < 1 || vidas > juego.jugador.getVidasMax()) {
        return false;
    }
    if (experiencia < 0) {
        return false;
    }
    if (suero != 0 && suero != 1) {
        return false;
    }

    vector<string> marcas = dividir(datos.at("completado"), ',');
    if (static_cast<int>(marcas.size()) != total) {
        return false;
    }
    bool completado[Tren::NUM_VAGONES];
    for (int i = 0; i < total; i++) {
        if (marcas[i] != "0" && marcas[i] != "1") {
            return false;
        }
        completado[i] = (marcas[i] == "1");
    }
    if (!completado[vagon - 1]) {
        return false;
    }

    vector<Objeto> inventario;
    if (!leerObjetos(datos.at("inventario"), inventario)) {
        return false;
    }

    vector<Objeto> objetosVagon[Tren::NUM_VAGONES];
    for (int i = 0; i < total; i++) {
        string clave = "objetos" + to_string(i + 1);
        if (datos.count(clave) && !leerObjetos(datos[clave], objetosVagon[i])) {
            return false;
        }
    }

    juego.tren.crear();
    for (int i = 0; i < total; i++) {
        Vagon* v = juego.tren.getVagon(i);
        v->vaciarObjetos();
        for (const Objeto& o : objetosVagon[i]) {
            v->agregarObjeto(o);
        }
        juego.completado[i] = completado[i];
    }
    juego.jugador.restaurar(vidas, experiencia, suero == 1, inventario);
    juego.vagonActual = vagon - 1;
    juego.intentoActivo = false;
    juego.partidaGanada = false;
    juego.salirAlMenu = false;
    return true;
}
