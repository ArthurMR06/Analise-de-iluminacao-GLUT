// Projeto ICG - Terra com sombreamento Flat, Gouraud e Phong (arquivo unico).
// Compilar: g++ -std=c++17 main.cpp -o ICG_Terra -lglut -lGLU -lGL
// Teclas: 1 Flat | 2 Gouraud | 3 Phong | setas movem a Terra (Shift+cima/baixo = eixo Y)
//         mouse solto move a lampada | botao direito + arrasto gira a camera | ESC sai
#define GL_GLEXT_PROTOTYPES
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

// ---------- Estado ----------
float terra[3] = {0, 1, 0};          // posicao da Terra
float luz[4] = {0, 4, 2, 1};         // posicao da lampada (w=1: luz posicional)
float yaw = 37.9f, pitch = 25.8f;    // camera orbital, em graus
int modo = 2;                        // 1 = Flat, 2 = Gouraud, 3 = Phong
bool arrastando = false;             // botao direito pressionado
int mx, my;                          // ultima posicao do mouse no arrasto
GLuint phong = 0;                    // programa GLSL (0 = indisponivel)
GLint uTex;                          // uniform: 1 = aplica a textura
GLUquadric* esfera;
const char* titulos[] = {"ICG - Flat Shading", "ICG - Gouraud Shading", "ICG - Phong Shading"};

// ---------- Shaders Phong (iluminacao calculada por fragmento) ----------
const char* VS = R"(#version 120
varying vec3 n, l, v;
void main() {
    vec4 p = gl_ModelViewMatrix * gl_Vertex;
    n = gl_NormalMatrix * gl_Normal;
    l = gl_LightSource[0].position.xyz - p.xyz;   // ja esta no espaco da camera
    v = -p.xyz;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();
})";

const char* FS = R"(#version 120
uniform sampler2D tex;
uniform int uTex;
varying vec3 n, l, v;
void main() {
    vec4 c = gl_Color;
    if (uTex == 1) c *= texture2D(tex, gl_TexCoord[0].xy);
    vec3 N = normalize(n), L = normalize(l);
    float d = max(dot(N, L), 0.0);
    float s = d > 0.0 ? pow(max(dot(reflect(-L, N), normalize(v)), 0.0), 48.0) : 0.0;
    gl_FragColor = vec4(c.rgb * (0.22 + d) + 0.8 * s, c.a);   // ambiente + difusa + especular
})";

GLuint compilar(GLenum tipo, const char* fonte)
{
    GLuint s = glCreateShader(tipo);
    glShaderSource(s, 1, &fonte, nullptr);
    glCompileShader(s);
    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { char log[1024]; glGetShaderInfoLog(s, 1024, nullptr, log); fprintf(stderr, "Erro no shader:\n%s\n", log); }
    return s;
}

// ---------- Textura procedural da Terra (azul e verde) ----------
void criarTextura()
{
    // Poligonos simplificados (longitude, latitude) dos continentes.
    const std::vector<std::vector<float>> continentes = {
        {-168,70, -150,72, -135,68, -125,60, -130,52, -124,42, -117,32, -105,25, -97,18, -88,16,
         -82,23, -80,30, -75,35, -65,45, -58,53, -75,60, -100,72, -130,75},                    // America do Norte
        {-81,12, -70,10, -60,5, -50,-2, -44,-12, -50,-22, -55,-32, -65,-55, -72,-52, -76,-35,
         -78,-15},                                                                              // America do Sul
        {-17,35, -5,37, 10,37, 25,32, 33,20, 43,10, 50,0, 42,-12, 35,-25, 28,-34, 18,-35,
         8,-30, 0,-20, -8,-5, -15,10},                                                          // Africa
        {-10,36, 0,43, 10,45, 15,55, 30,60, 45,68, 65,70, 80,72, 100,70, 120,65, 140,60,
         160,55, 170,48, 160,40, 145,38, 135,42, 125,35, 115,25, 105,20, 100,10, 90,8,
         80,15, 70,22, 60,25, 50,30, 40,38, 30,40, 20,38, 10,35, 0,38},                        // Europa e Asia
        {113,-11, 130,-10, 145,-15, 154,-25, 150,-38, 135,-43, 118,-35, 112,-22},              // Australia
        {-60,83, -45,82, -25,75, -35,60, -50,60, -65,70}};                                      // Groenlandia

    // Teste ponto-em-poligono (cruzamento de raios).
    auto dentro = [](const std::vector<float>& p, float lo, float la) {
        bool r = false;
        size_t n = p.size() / 2;
        for (size_t i = 0, j = n - 1; i < n; j = i++) {
            float xi = p[2*i], yi = p[2*i+1], xj = p[2*j], yj = p[2*j+1];
            if ((yi > la) != (yj > la) && lo < (xj - xi) * (la - yi) / (yj - yi) + xi) r = !r;
        }
        return r;
    };

    static unsigned char px[256][512][3];
    for (int y = 0; y < 256; ++y)
        for (int x = 0; x < 512; ++x) {
            float lo = -180.0f + 360.0f * x / 511, la = 90.0f - 180.0f * y / 255;
            int v = (x * 17 + y * 31) % 18;   // pequena variacao de tom
            unsigned char* c = px[y][x];
            if (std::any_of(continentes.begin(), continentes.end(), [&](auto& p) { return dentro(p, lo, la); }))
                { c[0] = 45 + v; c[1] = 115 + v; c[2] = 35 + v / 2; }   // continente: verde
            else
                { c[0] = 5 + v / 5; c[1] = 55 + v; c[2] = 135 + v; }    // oceano: azul
        }

    GLuint t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, px);

    esfera = gluNewQuadric();
    gluQuadricNormals(esfera, GLU_SMOOTH);
    gluQuadricTexture(esfera, GL_TRUE);
}

// ---------- Desenho ----------
void desenhar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera orbital ao redor de (0, 0.5, 0), a 12.66 de distancia.
    glTranslatef(0, 0, -12.66f);
    glRotatef(pitch, 1, 0, 0);
    glRotatef(-yaw, 0, 1, 0);
    glTranslatef(0, -0.5f, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, luz);   // depois da camera: a posicao fica no espaco do olho
    glUseProgram(modo == 3 ? phong : 0);
    glShadeModel(modo == 1 ? GL_FLAT : GL_SMOOTH);

    // Base fixa
    if (modo == 3) glUniform1i(uTex, 0);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0, -0.1f, 0);
    glScalef(10, 0.2f, 10);
    glutSolidCube(1);
    glPopMatrix();

    // Terra: Flat usa malha simples (faces visiveis); Gouraud e Phong, malha detalhada.
    if (modo == 3) glUniform1i(uTex, 1);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(terra[0], terra[1], terra[2]);
    gluSphere(esfera, 1, modo == 1 ? 20 : 64, modo == 1 ? 12 : 48);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // Lampada (so a representacao visual, sem iluminacao)
    glUseProgram(0);
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 0);
    glPushMatrix();
    glTranslatef(luz[0], luz[1], luz[2]);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// ---------- Entrada ----------
void redimensionar(int w, int h)
{
    if (!h) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char k, int, int)
{
    if (k == 27) exit(0);
    if (k == '3' && !phong) puts("Phong nao esta disponivel: verifique os shaders.");
    else if (k >= '1' && k <= '3') { modo = k - '0'; glutSetWindowTitle(titulos[modo - 1]); }
    glutPostRedisplay();
}

void setas(int k, int, int)
{
    float s = (k == GLUT_KEY_RIGHT || k == GLUT_KEY_UP) ? 0.2f : -0.2f;
    if (k == GLUT_KEY_LEFT || k == GLUT_KEY_RIGHT) terra[0] += s;
    else if (k == GLUT_KEY_UP || k == GLUT_KEY_DOWN) {
        if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) terra[1] += s;   // Y sem limite
        else terra[2] -= s;
    } else return;
    terra[0] = std::clamp(terra[0], -4.0f, 4.0f);   // limites so em X e Z
    terra[2] = std::clamp(terra[2], -4.0f, 4.0f);
    glutPostRedisplay();
}

void mouse(int botao, int acao, int x, int y)
{
    if (botao != GLUT_RIGHT_BUTTON) return;
    arrastando = (acao == GLUT_DOWN);
    mx = x; my = y;
}

void moverLuz(int x, int y)   // mouse solto: lampada no plano XZ
{
    luz[0] = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 10 - 5;
    luz[2] = (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 10 - 5;
    glutPostRedisplay();
}

void girarCamera(int x, int y)   // botao direito + arrasto
{
    if (!arrastando) return;
    yaw += (x - mx) * 0.4f;
    pitch = std::clamp(pitch - (y - my) * 0.4f, -5.0f, 85.0f);
    mx = x; my = y;
    glutPostRedisplay();
}

// ---------- Programa ----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(titulos[1]);

    // OpenGL: profundidade, luz 0 (difusa e especular ja sao 1 por padrao) e material.
    glClearColor(0.1f, 0.1f, 0.15f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);   // glColor define ambiente e difusa
    const GLfloat ambiente[] = {0.15f, 0.15f, 0.15f, 1}, especular[] = {0.8f, 0.8f, 0.8f, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 48);

    criarTextura();

    phong = glCreateProgram();
    glAttachShader(phong, compilar(GL_VERTEX_SHADER, VS));
    glAttachShader(phong, compilar(GL_FRAGMENT_SHADER, FS));
    glLinkProgram(phong);
    GLint ok;
    glGetProgramiv(phong, GL_LINK_STATUS, &ok);
    if (ok) uTex = glGetUniformLocation(phong, "uTex");
    else { fputs("Aviso: nao foi possivel carregar o Phong Shading.\n", stderr); phong = 0; }

    glutDisplayFunc(desenhar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(setas);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(moverLuz);
    glutMotionFunc(girarCamera);
    glutMainLoop();
}
