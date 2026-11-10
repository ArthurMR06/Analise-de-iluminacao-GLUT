#ifndef ESTADO_H
#define ESTADO_H

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/freeglut.h>
#include <GL/glu.h>

// Reune os dados compartilhados pelos modulos do projeto.
struct Estado
{
    // Posicao da Terra. O Y permite subir e descer fora da base.
    float esferaX = 0.0f;
    float esferaY = 1.0f;
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

    // Camera orbital. Os angulos sao armazenados em graus.
    float cameraYaw = 37.9f;
    float cameraPitch = 25.8f;
    float cameraDistancia = 12.66f;
    float cameraAlvoY = 0.5f;

    // Controle do arrasto do botao direito para girar a camera.
    bool arrastandoCamera = false;
    int mouseAnteriorX = 0;
    int mouseAnteriorY = 0;

    // Recursos graficos criados durante a inicializacao.
    GLuint programaPhong = 0;
    GLuint texturaTerra = 0;
    GLUquadric* esferaQuadric = nullptr;
};

#endif
