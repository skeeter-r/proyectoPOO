# Ultimo Vagon - Version 0 (muy basica)

Juego de rol por consola en C++. Esta primera version solo tiene un combate:
el jugador contra un Asesino, con dos armas (Navaja y Latigo), una chaqueta y un suero.

## Archivos

Todos en la misma carpeta:

- Clases: `Personaje`, `Objeto`, `Jugador`, `Enemigo`, `Asesino`, `Combate` (cada una con su `.h` y su `.cpp`)
- `main.cpp`: crea al jugador y a un Asesino, y lanza el combate

## Compilar y ejecutar (Linux)

```
g++ -std=c++17 -Wall *.cpp -o juego
./juego
```

## Como se juega

Cada turno eliges un numero del menu: 1) Atacar, 2) Usar objeto, 3) Huir.
Armas: Navaja (2 de dano) y Latigo (1 de dano a todos los enemigos).
Objetos: Chaqueta (el Asesino te quita 1 vida menos por golpe) y Suero vital (+4 vidas).
