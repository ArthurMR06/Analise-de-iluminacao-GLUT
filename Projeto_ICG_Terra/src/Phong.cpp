#include "Phong.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    std::string lerArquivo(const char* caminho)
    {
        std::ifstream arquivo(caminho);
        if (!arquivo)
        {
            std::cerr << "Nao foi possivel abrir o arquivo: "
                      << caminho << std::endl;
            return {};
        }

        std::ostringstream conteudo;
        conteudo << arquivo.rdbuf();
        return conteudo.str();
    }

    GLuint compilarShader(GLenum tipo, const std::string& codigo)
    {
        const char* fonte = codigo.c_str();
        GLuint shader = glCreateShader(tipo);
        glShaderSource(shader, 1, &fonte, nullptr);
        glCompileShader(shader);

        GLint sucesso = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &sucesso);

        if (sucesso != GL_TRUE)
        {
            GLint tamanho = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tamanho);
            std::vector<char> log(tamanho > 1 ? tamanho : 1);
            glGetShaderInfoLog(shader, tamanho, nullptr, log.data());

            std::cerr << "Erro ao compilar shader:\n"
                      << log.data() << std::endl;
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }
}

GLuint criarProgramaPhong(const char* caminhoVertex,
                          const char* caminhoFragment)
{
    const std::string codigoVertex = lerArquivo(caminhoVertex);
    const std::string codigoFragment = lerArquivo(caminhoFragment);

    if (codigoVertex.empty() || codigoFragment.empty())
        return 0;

    const GLuint vertex = compilarShader(GL_VERTEX_SHADER, codigoVertex);
    if (vertex == 0)
        return 0;

    const GLuint fragment = compilarShader(GL_FRAGMENT_SHADER, codigoFragment);
    if (fragment == 0)
    {
        glDeleteShader(vertex);
        return 0;
    }

    const GLuint programa = glCreateProgram();
    glAttachShader(programa, vertex);
    glAttachShader(programa, fragment);
    glLinkProgram(programa);

    GLint sucesso = GL_FALSE;
    glGetProgramiv(programa, GL_LINK_STATUS, &sucesso);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (sucesso != GL_TRUE)
    {
        GLint tamanho = 0;
        glGetProgramiv(programa, GL_INFO_LOG_LENGTH, &tamanho);
        std::vector<char> log(tamanho > 1 ? tamanho : 1);
        glGetProgramInfoLog(programa, tamanho, nullptr, log.data());

        std::cerr << "Erro ao vincular programa Phong:\n"
                  << log.data() << std::endl;
        glDeleteProgram(programa);
        return 0;
    }

    return programa;
}

void configurarPhong(const Estado& estado,
                     bool usarTextura,
                     float luzOlhoX,
                     float luzOlhoY,
                     float luzOlhoZ)
{
    glUseProgram(estado.programaPhong);

    glUniform3f(glGetUniformLocation(estado.programaPhong,
                "uLightPositionEye"), luzOlhoX, luzOlhoY, luzOlhoZ);
    glUniform1i(glGetUniformLocation(estado.programaPhong,
                "uUseTexture"), usarTextura ? 1 : 0);
    glUniform1i(glGetUniformLocation(estado.programaPhong,
                "uTexture"), 0);

    glUniform3f(glGetUniformLocation(estado.programaPhong,
                "uGlobalAmbient"), 0.22f, 0.22f, 0.22f);
    glUniform3f(glGetUniformLocation(estado.programaPhong,
                "uLightDiffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(estado.programaPhong,
                "uLightSpecular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(estado.programaPhong,
                "uMaterialSpecular"), 0.8f, 0.8f, 0.8f);
    glUniform1f(glGetUniformLocation(estado.programaPhong,
                "uShininess"), 48.0f);
}
