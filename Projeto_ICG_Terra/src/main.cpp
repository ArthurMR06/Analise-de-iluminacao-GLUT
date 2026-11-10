#include "Controlador.h"

// O main apenas entrega a execucao ao controlador geral.
int main(int argc, char** argv)
{
    Controlador aplicacao;
    return aplicacao.executar(argc, argv);
}
