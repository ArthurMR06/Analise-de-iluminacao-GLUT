#ifndef PHONG_H
#define PHONG_H

#include "Estado.h"

// Le, compila e vincula os arquivos GLSL da pasta shaders/.
GLuint criarProgramaPhong(const char* caminhoVertex,
                          const char* caminhoFragment);

// Ativa o shader e envia os parametros da iluminacao.
void configurarPhong(const Estado& estado,
                     bool usarTextura,
                     float luzOlhoX,
                     float luzOlhoY,
                     float luzOlhoZ);

#endif
