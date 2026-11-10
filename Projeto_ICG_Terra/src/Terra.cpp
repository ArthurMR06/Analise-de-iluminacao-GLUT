#include "Terra.h"

#include <vector>

namespace
{
    struct Ponto
    {
        float longitude;
        float latitude;
    };

    // Poligonos simplificados: servem para criar uma representacao visual,
    // e nao um mapa geografico com contornos cartograficos precisos.
    const Ponto americaNorte[] = {
        {-168,70}, {-150,72}, {-135,68}, {-125,60}, {-130,52},
        {-124,42}, {-117,32}, {-105,25}, {-97,18}, {-88,16},
        {-82,23}, {-80,30}, {-75,35}, {-65,45}, {-58,53},
        {-75,60}, {-100,72}, {-130,75}
    };

    const Ponto americaSul[] = {
        {-81,12}, {-70,10}, {-60,5}, {-50,-2}, {-44,-12},
        {-50,-22}, {-55,-32}, {-65,-55}, {-72,-52}, {-76,-35},
        {-78,-15}
    };

    const Ponto africa[] = {
        {-17,35}, {-5,37}, {10,37}, {25,32}, {33,20}, {43,10},
        {50,0}, {42,-12}, {35,-25}, {28,-34}, {18,-35},
        {8,-30}, {0,-20}, {-8,-5}, {-15,10}
    };

    const Ponto europaAsia[] = {
        {-10,36}, {0,43}, {10,45}, {15,55}, {30,60}, {45,68},
        {65,70}, {80,72}, {100,70}, {120,65}, {140,60},
        {160,55}, {170,48}, {160,40}, {145,38}, {135,42},
        {125,35}, {115,25}, {105,20}, {100,10}, {90,8},
        {80,15}, {70,22}, {60,25}, {50,30}, {40,38},
        {30,40}, {20,38}, {10,35}, {0,38}
    };

    const Ponto australia[] = {
        {113,-11}, {130,-10}, {145,-15}, {154,-25},
        {150,-38}, {135,-43}, {118,-35}, {112,-22}
    };

    const Ponto groenlandia[] = {
        {-60,83}, {-45,82}, {-25,75}, {-35,60}, {-50,60}, {-65,70}
    };

    bool pontoNoPoligono(float longitude,
                         float latitude,
                         const Ponto poligono[],
                         int quantidade)
    {
        bool dentro = false;

        for (int i = 0, j = quantidade - 1; i < quantidade; j = i++)
        {
            const bool cruza =
                ((poligono[i].latitude > latitude) !=
                 (poligono[j].latitude > latitude)) &&
                (longitude <
                 (poligono[j].longitude - poligono[i].longitude) *
                 (latitude - poligono[i].latitude) /
                 (poligono[j].latitude - poligono[i].latitude) +
                 poligono[i].longitude);

            if (cruza)
                dentro = !dentro;
        }

        return dentro;
    }

    bool ehTerra(float longitude, float latitude)
    {
        return
            pontoNoPoligono(longitude, latitude, americaNorte,
                sizeof(americaNorte) / sizeof(Ponto)) ||
            pontoNoPoligono(longitude, latitude, americaSul,
                sizeof(americaSul) / sizeof(Ponto)) ||
            pontoNoPoligono(longitude, latitude, africa,
                sizeof(africa) / sizeof(Ponto)) ||
            pontoNoPoligono(longitude, latitude, europaAsia,
                sizeof(europaAsia) / sizeof(Ponto)) ||
            pontoNoPoligono(longitude, latitude, australia,
                sizeof(australia) / sizeof(Ponto)) ||
            pontoNoPoligono(longitude, latitude, groenlandia,
                sizeof(groenlandia) / sizeof(Ponto));
    }
}

void criarTexturaTerra(Estado& estado)
{
    const int largura = 512;
    const int altura = 256;
    std::vector<unsigned char> pixels(largura * altura * 3);

    for (int y = 0; y < altura; ++y)
    {
        for (int x = 0; x < largura; ++x)
        {
            const float longitude = -180.0f + 360.0f * x / (largura - 1);
            // A primeira linha da imagem representa o polo norte.
            const float latitude = 90.0f - 180.0f * y / (altura - 1);
            const int indice = (y * largura + x) * 3;
            const int variacao = (x * 17 + y * 31) % 18;

            if (ehTerra(longitude, latitude))
            {
                // Continentes em tons de verde.
                pixels[indice]     = static_cast<unsigned char>(45 + variacao);
                pixels[indice + 1] = static_cast<unsigned char>(115 + variacao);
                pixels[indice + 2] = static_cast<unsigned char>(35 + variacao / 2);
            }
            else
            {
                // Oceanos em tons de azul.
                pixels[indice]     = static_cast<unsigned char>(5 + variacao / 5);
                pixels[indice + 1] = static_cast<unsigned char>(55 + variacao);
                pixels[indice + 2] = static_cast<unsigned char>(135 + variacao);
            }
        }
    }

    glGenTextures(1, &estado.texturaTerra);
    glBindTexture(GL_TEXTURE_2D, estado.texturaTerra);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 largura, altura, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pixels.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    estado.esferaQuadric = gluNewQuadric();
    if (estado.esferaQuadric != nullptr)
    {
        gluQuadricNormals(estado.esferaQuadric, GLU_SMOOTH);
        gluQuadricTexture(estado.esferaQuadric, GL_TRUE);
    }
}

void destruirTerra(Estado& estado)
{
    if (estado.esferaQuadric != nullptr)
    {
        gluDeleteQuadric(estado.esferaQuadric);
        estado.esferaQuadric = nullptr;
    }

    if (estado.texturaTerra != 0)
    {
        glDeleteTextures(1, &estado.texturaTerra);
        estado.texturaTerra = 0;
    }
}
