#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "Estado.h"

// Centraliza a inicializacao, os eventos e o ciclo de vida da aplicacao.
class Controlador
{
public:
    int executar(int argc, char** argv);

private:
    Estado estado;
    int janela = 0;
    bool recursosLiberados = false;

    void inicializarOpenGL();
    void carregarRecursos();
    void registrarEventos();
    void liberarRecursos();

    static Controlador* instancia;

    // Adaptadores estaticos exigidos pelo sistema de callbacks do FreeGLUT.
    static void desenhar();
    static void tecladoCallback(unsigned char tecla, int x, int y);
    static void setasCallback(int tecla, int x, int y);
    static void mouseCallback(int botao, int acao, int x, int y);
    static void movimentoPassivoCallback(int x, int y);
    static void movimentoAtivoCallback(int x, int y);
    static void redimensionarCallback(int largura, int altura);
    static void fecharJanelaCallback();
};

#endif
