#ifndef CONTROLES_H
#define CONTROLES_H

#include "Estado.h"

// Funcoes chamadas pelo Controlador quando ocorre uma interacao.
void teclado(Estado& estado, unsigned char tecla);
void setas(Estado& estado, int tecla);
void mouseBotao(Estado& estado, int botao, int acao, int x, int y);
void moverMouse(Estado& estado, int x, int y);
void arrastarCamera(Estado& estado, int x, int y);
void redimensionar(Estado& estado, int largura, int altura);

#endif
