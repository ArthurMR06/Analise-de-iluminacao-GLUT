#include "Cena.h"
#include "Phong.h"

namespace
{
    void obterPosicaoLuzNaCamera(const Estado& estado,
                                float& x, float& y, float& z)
    {
        // A matriz modelview transforma a posicao da luz do espaco do mundo
        // para o espaco da camera, que e o espaco usado pelo shader.
        GLfloat matriz[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, matriz);

        x = matriz[0] * estado.luzX + matriz[4] * estado.luzY +
            matriz[8] * estado.luzZ + matriz[12];
        y = matriz[1] * estado.luzX + matriz[5] * estado.luzY +
            matriz[9] * estado.luzZ + matriz[13];
        z = matriz[2] * estado.luzX + matriz[6] * estado.luzY +
            matriz[10] * estado.luzZ + matriz[14];
    }
}

void desenharCena(Estado& estado)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera permanece observando o centro da cena.
    gluLookAt(7.0, 6.0, 9.0,
              0.0, 0.5, 0.0,
              0.0, 1.0, 0.0);

    // Posicao da luz para a iluminacao fixa do OpenGL.
    const GLfloat posicaoLuz[] = {
        estado.luzX, estado.luzY, estado.luzZ, 1.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    float luzOlhoX = 0.0f;
    float luzOlhoY = 0.0f;
    float luzOlhoZ = 0.0f;
    obterPosicaoLuzNaCamera(estado, luzOlhoX, luzOlhoY, luzOlhoZ);

    const bool usarPhong =
        estado.modoSombreamento == 3 && estado.programaPhong != 0;

    // Escolhe o metodo de sombreamento.
    if (usarPhong)
        configurarPhong(estado, false, luzOlhoX, luzOlhoY, luzOlhoZ);
    else
    {
        glUseProgram(0);
        glShadeModel(estado.modoSombreamento == 1 ? GL_FLAT : GL_SMOOTH);
    }

    // BASE FIXA: sua posicao nao depende da Terra nem da lampada.
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f);
    glScalef(10.0f, 0.2f, 10.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Terra: somente a esfera se desloca com as setas.
    glPushMatrix();
    glTranslatef(estado.esferaX, 1.0f, estado.esferaZ);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, estado.texturaTerra);

    if (usarPhong)
        configurarPhong(estado, true, luzOlhoX, luzOlhoY, luzOlhoZ);
    else
        glUseProgram(0);

    if (estado.esferaQuadric != nullptr)
        gluSphere(estado.esferaQuadric, 1.0, 64, 64);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // A lampada e apenas a representacao visual da fonte de luz.
    glUseProgram(0);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(estado.luzX, estado.luzY, estado.luzZ);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}
