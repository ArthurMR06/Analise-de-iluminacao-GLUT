#include "Controles.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

void teclado(Estado& estado, unsigned char tecla)
{
    switch (tecla)
    {
        case '1':
            estado.modoSombreamento = 1;
            glutSetWindowTitle("ICG - Flat Shading");
            break;

        case '2':
            estado.modoSombreamento = 2;
            glutSetWindowTitle("ICG - Gouraud Shading");
            break;

        case '3':
            if (estado.programaPhong != 0)
            {
                estado.modoSombreamento = 3;
                glutSetWindowTitle("ICG - Phong Shading");
            }
            else
            {
                std::cout << "Phong nao esta disponivel: verifique os shaders."
                          << std::endl;
            }
            break;

        case 27:
            // Encerra o loop do FreeGLUT para que o controlador libere os recursos.
            glutLeaveMainLoop();
            break;
    }

    glutPostRedisplay();
}

void setas(Estado& estado, int tecla)
{
    const float velocidade = 0.2f;
    const int modificadores = glutGetModifiers();
    const bool shiftPressionado = (modificadores & GLUT_ACTIVE_SHIFT) != 0;

    switch (tecla)
    {
        case GLUT_KEY_LEFT:
            estado.esferaX -= velocidade;
            break;

        case GLUT_KEY_RIGHT:
            estado.esferaX += velocidade;
            break;

        case GLUT_KEY_UP:
            if (shiftPressionado)
                estado.esferaY += velocidade;
            else
                estado.esferaZ -= velocidade;
            break;

        case GLUT_KEY_DOWN:
            if (shiftPressionado)
                estado.esferaY -= velocidade;
            else
                estado.esferaZ += velocidade;
            break;

        default:
            return;
    }

    // Os limites laterais continuam valendo apenas para X e Z.
    // A coordenada Y nao e limitada: a Terra pode subir ou descer livremente.
    estado.esferaX = std::max(-4.0f, std::min(4.0f, estado.esferaX));
    estado.esferaZ = std::max(-4.0f, std::min(4.0f, estado.esferaZ));

    glutPostRedisplay();
}

void mouseBotao(Estado& estado, int botao, int acao, int x, int y)
{
    // Segurar o botao direito e arrastar gira a camera.
    if (botao == GLUT_RIGHT_BUTTON)
    {
        if (acao == GLUT_DOWN)
        {
            estado.arrastandoCamera = true;
            estado.mouseAnteriorX = x;
            estado.mouseAnteriorY = y;
        }
        else if (acao == GLUT_UP)
        {
            estado.arrastandoCamera = false;
        }
    }
}

void moverMouse(Estado& estado, int x, int y)
{
    // Com o mouse solto, o movimento continua controlando a lampada.
    if (estado.largura <= 0 || estado.altura <= 0)
        return;

    estado.luzX = (static_cast<float>(x) / estado.largura) * 10.0f - 5.0f;
    estado.luzZ = (static_cast<float>(y) / estado.altura) * 10.0f - 5.0f;

    glutPostRedisplay();
}

void arrastarCamera(Estado& estado, int x, int y)
{
    if (!estado.arrastandoCamera)
        return;

    const int deslocamentoX = x - estado.mouseAnteriorX;
    const int deslocamentoY = y - estado.mouseAnteriorY;

    // Movimento horizontal gira a camera ao redor da cena.
    // Movimento vertical inclina a camera para cima ou para baixo.
    estado.cameraYaw += deslocamentoX * 0.4f;
    estado.cameraPitch -= deslocamentoY * 0.4f;

    // Evita que a camera vire de ponta-cabeca ou atravesse o chao.
    estado.cameraPitch = std::max(-5.0f, std::min(85.0f, estado.cameraPitch));

    estado.mouseAnteriorX = x;
    estado.mouseAnteriorY = y;

    glutPostRedisplay();
}

void redimensionar(Estado& estado, int largura, int altura)
{
    if (altura == 0)
        altura = 1;

    estado.largura = largura;
    estado.altura = altura;

    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,
                   static_cast<float>(largura) / static_cast<float>(altura),
                   0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
