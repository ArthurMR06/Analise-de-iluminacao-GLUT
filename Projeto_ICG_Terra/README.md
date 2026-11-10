# Projeto ICG - Terra, camera e sombreamento

Projeto em C++, OpenGL, GLU e FreeGLUT que apresenta uma esfera texturizada como a Terra, uma base fixa, uma lampada movimentada pelo mouse e tres tecnicas de sombreamento: Flat, Gouraud e Phong.

## Estrutura do projeto

```text
Projeto_ICG_Terra/
|-- CMakeLists.txt
|-- README.md
|-- include/
|   |-- Estado.h
|   |-- Controlador.h
|   |-- Cena.h
|   |-- Controles.h
|   |-- Phong.h
|   `-- Terra.h
|-- src/
|   |-- main.cpp
|   |-- Controlador.cpp
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

## Funcao de cada pasta e arquivo

- **include/**: contem os arquivos `.h`, que declaram as estruturas e funcoes utilizadas pelos modulos.
- **src/**: contem a implementacao em C++.
- **shaders/**: contem os programas GLSL do Phong Shading. O `.vert` prepara dados dos vertices e o `.frag` calcula a iluminacao por fragmento.
- **docs/**: contem a explicacao da arquitetura e dos controles.

Arquivos centrais:

- **main.cpp**: possui somente a criacao do controlador e a chamada para iniciar a aplicacao.
- **Controlador.cpp**: e o controlador geral. Cria a janela, configura o OpenGL, carrega os recursos, registra os eventos e inicia o loop principal do FreeGLUT.
- **Estado.h**: guarda os dados compartilhados, como posicao da Terra, posicao da luz e angulos da camera.
- **Cena.cpp**: posiciona a camera e desenha a base, a Terra e a lampada.
- **Controles.cpp**: trata as teclas, o mouse e o redimensionamento da janela.
- **Terra.cpp**: cria a textura azul e verde e configura a esfera.
- **Phong.cpp**: carrega, compila e configura os shaders Phong.

## Controles

| Acao | Resultado |
|---|---|
| `1` | Ativa Flat Shading. A esfera usa uma malha mais simples e apresenta faces mais visiveis. |
| `2` | Ativa Gouraud Shading. A esfera usa uma malha mais detalhada e iluminacao interpolada entre os vertices. |
| `3` | Ativa Phong Shading, calculado no fragment shader. |
| Seta esquerda/direita | Move a Terra no eixo X. |
| Seta cima/baixo | Move a Terra para frente/tras no eixo Z. |
| `Shift` + seta cima | Sobe a Terra no eixo Y. |
| `Shift` + seta baixo | Desce a Terra no eixo Y. |
| Mouse movimentado sem botoes | Move a lampada no plano XZ. |
| Segurar o botao direito e arrastar | Gira/inclina a camera ao redor da cena. |
| `ESC` | Encerra o programa. |

A movimentacao vertical da Terra nao possui limite ligado a base. Os limites laterais continuam aplicados aos eixos X e Z.

## Diferenca visual entre Flat e Gouraud

No modo **Flat**, o OpenGL calcula a iluminacao por primitiva e a esfera e desenhada com menos subdivisoes (20 fatias por 12 pilhas), deixando as faces mais aparentes.

No modo **Gouraud**, a iluminacao e calculada nos vertices e as cores sao interpoladas pela superficie. A esfera usa uma malha mais detalhada (64 fatias por 48 pilhas), produzindo uma aparencia mais suave.

A diferenca de quantidade de subdivisoes foi intencional para deixar a comparacao visual mais clara durante a demonstracao. Portanto, alem do metodo de interpolacao da iluminacao, a densidade da malha tambem muda entre os dois modos.

## Compilacao no Linux

Dependencias: compilador C++17, CMake 3.16 ou superior, OpenGL, GLU e FreeGLUT, com suporte a contexto OpenGL 2.1 e GLSL 1.20.

Em sistemas baseados em Debian/Ubuntu, normalmente podem ser instalados com:

```bash
sudo apt install build-essential cmake freeglut3-dev libglu1-mesa-dev
```

Na pasta `Projeto_ICG_Terra`, execute:

```bash
cmake -S . -B build
cmake --build build
cd build
./ICG_Terra
```

O CMake copia a pasta `shaders` para a pasta de compilacao. Por isso, execute o programa a partir de `build`.

## Sobre o Phong Shading

O Phong e implementado com shaders GLSL. O vertex shader transforma posicoes e normais; o fragment shader calcula a iluminacao ambiente, difusa e especular para cada fragmento. O uso de `glShadeModel(GL_SMOOTH)` sozinho representa Gouraud, nao Phong.
