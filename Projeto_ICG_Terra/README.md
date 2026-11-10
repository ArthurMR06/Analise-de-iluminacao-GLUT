# Projeto ICG - Terra, iluminacao e sombreamento

Este projeto demonstra tres tecnicas de sombreamento em uma cena 3D feita com C++, OpenGL, GLU e FreeGLUT. A esfera representa a Terra, com oceanos azuis e continentes verdes. A base permanece fixa, as setas movimentam somente a esfera e o mouse movimenta a representacao da lampada.

## Estrutura de pastas

```text
Projeto_ICG_Terra/
|-- CMakeLists.txt
|-- README.md
|-- include/
|   |-- Estado.h
|   |-- Cena.h
|   |-- Controles.h
|   |-- Phong.h
|   `-- Terra.h
|-- src/
|   |-- main.cpp
|   |-- Cena.cpp
|   |-- Controles.cpp
|   |-- Phong.cpp
|   `-- Terra.cpp
|-- shaders/
|   |-- phong.vert
|   `-- phong.frag
`-- docs/
    `-- ESTRUTURA.md
```

### O que cada pasta faz?

- **include/**: guarda os arquivos `.h` (headers). Eles declaram as estruturas e funcoes que podem ser utilizadas por diferentes partes do programa.
- **src/**: guarda a implementacao em C++. Cada arquivo tem uma responsabilidade, explicada em `docs/ESTRUTURA.md`.
- **shaders/**: guarda os programas GLSL usados pelo Phong Shading. O arquivo `.vert` trabalha os vertices; o `.frag` calcula a iluminacao de cada fragmento da superficie.
- **docs/**: guarda a documentacao de apoio sobre a organizacao do projeto.

O arquivo **CMakeLists.txt** informa ao CMake quais arquivos devem ser compilados e quais bibliotecas graficas devem ser vinculadas.

## Controles

| Tecla ou acao | Funcao |
|---|---|
| `1` | Flat Shading |
| `2` | Gouraud Shading |
| `3` | Phong Shading |
| Seta esquerda/direita | Move a Terra no eixo X |
| Seta cima/baixo | Move a Terra no eixo Z |
| Mover o mouse | Move a lampada no plano XZ |
| `ESC` | Encerra o programa |

## Requisitos

- Compilador C++ com suporte a C++17.
- CMake 3.16 ou superior.
- OpenGL, GLU e FreeGLUT.
- Contexto OpenGL 2.1 ou superior com suporte a GLSL 1.20.

## Compilar no Linux

Instale as dependencias de desenvolvimento da sua distribuicao. Em sistemas baseados em Debian/Ubuntu, normalmente os pacotes sao `build-essential`, `cmake`, `freeglut3-dev` e `libglu1-mesa-dev`.

Na pasta `Projeto_ICG_Terra`, execute:

```bash
cmake -S . -B build
cmake --build build
```

Para executar, entre na pasta `build`:

```bash
cd build
./ICG_Terra
```

A aplicacao procura os shaders em `build/shaders`. O CMake copia essa pasta automaticamente durante a configuracao.

## Observacao sobre Phong

O modo Phong e implementado com shaders GLSL, pois `glShadeModel(GL_SMOOTH)` sozinho implementa Gouraud Shading, nao Phong Shading. Se a compilacao ou vinculacao dos shaders falhar, a mensagem de erro sera exibida no terminal e a tecla `3` nao ativara esse modo.
