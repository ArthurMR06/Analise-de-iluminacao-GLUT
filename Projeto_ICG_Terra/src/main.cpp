#include "Cena.h"
#include "Controles.h"
#include "Phong.h"
#include "Terra.h"

#include <iostream>

namespace
{
    Estado estado;

    void inicializar()
    {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glEnable(GL_DEPTH_TEST);
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

        criarTexturaTerra(estado);
        estado.programaPhong = criarProgramaPhong(
            "shaders/phong.vert", "shaders/phong.frag");

        if (estado.programaPhong == 0)
            std::cerr << "Aviso: nao foi possivel carregar o Phong Shading."
                      << std::endl;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // O shader usa GLSL 1.20, compativel com contexto OpenGL 2.1.
    glutInitContextVersion(2, 1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ICG - Gouraud Shading");

    inicializar();
    definirEstadoControles(&estado);

    glutDisplayFunc([]() { desenharCena(estado); });
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(setas);
    glutPassiveMotionFunc(moverMouse);

    glutMainLoop();

    destruirTerra(estado);
    if (estado.programaPhong != 0)
        glDeleteProgram(estado.programaPhong);

    return 0;
}
