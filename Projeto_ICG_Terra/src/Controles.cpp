#include "Controles.h"

#include <cstdlib>
#include <iostream>

namespace
{
    Estado* estadoAtual = nullptr;
}

void definirEstadoControles(Estado* estado)
{
    estadoAtual = estado;
}

void teclado(unsigned char tecla, int, int)
{
    if (estadoAtual == nullptr)
        return;

    switch (tecla)
    {
        case '1':
            estadoAtual->modoSombreamento = 1;
            glutSetWindowTitle("ICG - Flat Shading");
            break;

        case '2':
            estadoAtual->modoSombreamento = 2;
            glutSetWindowTitle("ICG - Gouraud Shading");
            break;

        case '3':
            if (estadoAtual->programaPhong != 0)
            {
                estadoAtual->modoSombreamento = 3;
                glutSetWindowTitle("ICG - Phong Shading");
            }
            else
            {
                std::cout << "Phong nao esta disponivel: verifique os shaders."
                          << std::endl;
            }
            break;

        case 27:
            std::exit(0);
            break;
    }

    glutPostRedisplay();
}

void setas(int tecla, int, int)
{
    if (estadoAtual == nullptr)
        return;

    const float velocidade = 0.2f;

    switch (tecla)
    {
        case GLUT_KEY_LEFT:
            estadoAtual->esferaX -= velocidade;
            break;
        case GLUT_KEY_RIGHT:
            estadoAtual->esferaX += velocidade;
            break;
        case GLUT_KEY_UP:
            estadoAtual->esferaZ -= velocidade;
            break;
        case GLUT_KEY_DOWN:
            estadoAtual->esferaZ += velocidade;
            break;
        default:
            return;
    }

    // Mantem a esfera dentro dos limites da base fixa.
    if (estadoAtual->esferaX < -4.0f) estadoAtual->esferaX = -4.0f;
    if (estadoAtual->esferaX >  4.0f) estadoAtual->esferaX =  4.0f;
    if (estadoAtual->esferaZ < -4.0f) estadoAtual->esferaZ = -4.0f;
    if (estadoAtual->esferaZ >  4.0f) estadoAtual->esferaZ =  4.0f;

    glutPostRedisplay();
}

void moverMouse(int x, int y)
{
    if (estadoAtual == nullptr)
        return;

    estadoAtual->luzX = (static_cast<float>(x) / estadoAtual->largura) * 10.0f - 5.0f;
    estadoAtual->luzZ = (static_cast<float>(y) / estadoAtual->altura) * 10.0f - 5.0f;

    glutPostRedisplay();
}

void redimensionar(int largura, int altura)
{
    if (estadoAtual == nullptr)
        return;

    if (altura == 0)
        altura = 1;

    estadoAtual->largura = largura;
    estadoAtual->altura = altura;

    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,
                   static_cast<float>(largura) / static_cast<float>(altura),
                   0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
