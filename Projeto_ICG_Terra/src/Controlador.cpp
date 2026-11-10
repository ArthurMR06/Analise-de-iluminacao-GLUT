#include "Controlador.h"

#include "Cena.h"
#include "Controles.h"
#include "Phong.h"
#include "Terra.h"

#include <iostream>

Controlador* Controlador::instancia = nullptr;

int Controlador::executar(int argc, char** argv)
{
    instancia = this;

    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    janela = glutCreateWindow("ICG - Gouraud Shading");

    // Permite que glutMainLoop() retorne quando a janela for fechada.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    inicializarOpenGL();
    carregarRecursos();
    registrarEventos();

    glutMainLoop();

    liberarRecursos();
    instancia = nullptr;
    return 0;
}

void Controlador::inicializarOpenGL()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    const GLfloat ambiente[] = {0.15f, 0.15f, 0.15f, 1.0f};
    const GLfloat difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat materialEspecular[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialEspecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 48.0f);

    // O programa inicia no modo Gouraud.
    glShadeModel(GL_SMOOTH);
}

void Controlador::carregarRecursos()
{
    criarTexturaTerra(estado);

    estado.programaPhong = criarProgramaPhong(
        "shaders/phong.vert", "shaders/phong.frag");

    if (estado.programaPhong == 0)
    {
        std::cerr << "Aviso: nao foi possivel carregar o Phong Shading."
                  << std::endl;
    }
}

void Controlador::registrarEventos()
{
    glutDisplayFunc(desenhar);
    glutReshapeFunc(redimensionarCallback);
    glutKeyboardFunc(tecladoCallback);
    glutSpecialFunc(setasCallback);
    glutMouseFunc(mouseCallback);
    glutPassiveMotionFunc(movimentoPassivoCallback);
    glutMotionFunc(movimentoAtivoCallback);
    glutCloseFunc(fecharJanelaCallback);
}

void Controlador::liberarRecursos()
{
    if (recursosLiberados)
        return;

    destruirTerra(estado);

    if (estado.programaPhong != 0)
    {
        glDeleteProgram(estado.programaPhong);
        estado.programaPhong = 0;
    }

    recursosLiberados = true;
}

void Controlador::desenhar()
{
    if (instancia != nullptr)
        desenharCena(instancia->estado);
}

void Controlador::tecladoCallback(unsigned char tecla, int, int)
{
    if (instancia != nullptr)
        ::teclado(instancia->estado, tecla);
}

void Controlador::setasCallback(int tecla, int, int)
{
    if (instancia != nullptr)
        ::setas(instancia->estado, tecla);
}

void Controlador::mouseCallback(int botao, int acao, int x, int y)
{
    if (instancia != nullptr)
        mouseBotao(instancia->estado, botao, acao, x, y);
}

void Controlador::movimentoPassivoCallback(int x, int y)
{
    if (instancia != nullptr)
        moverMouse(instancia->estado, x, y);
}

void Controlador::movimentoAtivoCallback(int x, int y)
{
    if (instancia != nullptr)
        arrastarCamera(instancia->estado, x, y);
}

void Controlador::redimensionarCallback(int largura, int altura)
{
    if (instancia != nullptr)
        ::redimensionar(instancia->estado, largura, altura);
}

void Controlador::fecharJanelaCallback()
{
    if (instancia != nullptr)
        instancia->liberarRecursos();
}
