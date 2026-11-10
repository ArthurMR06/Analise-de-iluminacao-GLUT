#ifndef ESTADO_H
#define ESTADO_H

// Permite acessar as funcoes de shader do OpenGL 2.1 em ambientes que
// disponibilizam os prototipos por meio de glext.h.
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/freeglut.h>
#include <GL/glu.h>

// Reune os valores compartilhados pelas partes do programa.
struct Estado
{
    // Posicao da Terra no plano XZ.
    float esferaX = 0.0f;
    float esferaZ = 0.0f;

    // Posicao da lampada.
    float luzX = 0.0f;
    float luzY = 4.0f;
    float luzZ = 2.0f;

    // Dimensoes atuais da janela.
    int largura = 800;
    int altura = 600;

    // 1 = Flat, 2 = Gouraud, 3 = Phong.
    int modoSombreamento = 2;

    // Recursos graficos criados durante a inicializacao.
    GLuint programaPhong = 0;
    GLuint texturaTerra = 0;
    GLUquadric* esferaQuadric = nullptr;
};

#endif
