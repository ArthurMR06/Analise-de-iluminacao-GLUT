# Responsabilidade de cada arquivo

## Pasta `include/` - declaracoes

- **Estado.h**: concentra os dados compartilhados pela aplicacao: posicao da Terra, posicao da lampada, dimensoes da janela, modo de sombreamento e identificadores dos recursos graficos. Isso evita manter varias copias das mesmas variaveis.
- **Cena.h**: declara a funcao responsavel por desenhar a cena.
- **Controles.h**: declara os callbacks do teclado, das setas, do mouse e do redimensionamento da janela.
- **Terra.h**: declara as funcoes que criam e liberam a textura e a esfera.
- **Phong.h**: declara as funcoes que criam e configuram o programa de shaders.

## Pasta `src/` - implementacao C++

- **main.cpp**: e o ponto de entrada. Inicializa o FreeGLUT, configura a iluminacao, cria os recursos e registra as funcoes de callback.
- **Cena.cpp**: monta a camera e desenha os tres elementos visuais: base, Terra e lampada. A transformacao da base e fixa; a posicao da esfera vem das variaveis movimentadas pelas setas.
- **Controles.cpp**: interpreta as teclas. As teclas 1, 2 e 3 selecionam Flat, Gouraud e Phong; as setas alteram as coordenadas da Terra; o mouse altera as coordenadas da lampada.
- **Terra.cpp**: define poligonos simplificados dos continentes e gera uma imagem de textura em memoria, colorindo oceanos de azul e continentes de verde. Tambem configura a esfera da GLU para utilizar essa textura.
- **Phong.cpp**: le os arquivos GLSL, compila os shaders, vincula o programa e envia os parametros de iluminacao para a placa grafica.

## Pasta `shaders/` - iluminacao por fragmento

- **phong.vert**: executa a etapa de vertices. Transforma a posicao e a normal para o espaco da camera e encaminha essas informacoes ao fragment shader.
- **phong.frag**: calcula, para cada fragmento, as componentes ambiente, difusa e especular. Essa interpolacao das normais e o calculo da iluminacao por fragmento caracterizam o Phong Shading.

## Arquivo `CMakeLists.txt`

Define os arquivos C++ que formam o executavel, localiza OpenGL/GLU/FreeGLUT e copia a pasta `shaders` para a pasta de compilacao.
