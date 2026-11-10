#ifndef CONTROLES_H
#define CONTROLES_H

#include "Estado.h"

// Funcoes chamadas pelo FreeGLUT quando o usuario interage.
void teclado(unsigned char tecla, int x, int y);
void setas(int tecla, int x, int y);
void moverMouse(int x, int y);
void redimensionar(int largura, int altura);

// Informa qual instancia de Estado sera usada pelos callbacks.
void definirEstadoControles(Estado* estado);

#endif
