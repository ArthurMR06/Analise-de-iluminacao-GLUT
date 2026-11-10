# Organizacao e responsabilidade dos arquivos

## `include/` - declaracoes

- **Estado.h**: guarda os dados compartilhados: posicao tridimensional da Terra, posicao da lampada, dimensoes da janela, modo de sombreamento, orientacao da camera e identificadores dos recursos graficos.
- **Controlador.h**: declara a classe que coordena a inicializacao e o funcionamento da aplicacao.
- **Cena.h**: declara a funcao que desenha a cena.
- **Controles.h**: declara as funcoes que interpretam teclado, setas, mouse e redimensionamento.
- **Terra.h**: declara a criacao e a liberacao da textura e da esfera.
- **Phong.h**: declara a criacao e a configuracao do programa de shaders.

## `src/` - implementacao C++

- **main.cpp**: ponto de entrada reduzido. Cria um objeto `Controlador` e chama `executar()`.
- **Controlador.cpp**: centraliza a inicializacao do FreeGLUT e do OpenGL, carrega os recursos, registra os callbacks, inicia o loop principal e libera os recursos ao sair.
- **Cena.cpp**: calcula a posicao da camera orbital e desenha a base fixa, a Terra e a representacao visual da lampada. O modo Flat usa uma malha mais simples para evidenciar as faces; Gouraud e Phong usam uma malha mais detalhada.
- **Controles.cpp**: interpreta as teclas 1, 2 e 3; movimenta a Terra nos eixos X, Y e Z; movimenta a lampada com o mouse solto; e gira a camera quando o botao direito e mantido pressionado durante o arrasto.
- **Terra.cpp**: define poligonos simplificados dos continentes, gera uma textura azul e verde em memoria e configura a esfera para receber essa textura.
- **Phong.cpp**: le os arquivos GLSL, compila os shaders, vincula o programa e envia os parametros de iluminacao.

## `shaders/` - iluminacao Phong

- **phong.vert**: transforma a posicao e a normal dos vertices para o espaco da camera e envia os dados ao proximo estagio.
- **phong.frag**: calcula as componentes ambiente, difusa e especular para cada fragmento. O calculo por fragmento caracteriza o Phong Shading.

## Fluxo de inicializacao

1. `main.cpp` cria `Controlador`.
2. `Controlador::executar()` inicializa o FreeGLUT e cria a janela.
3. O controlador configura o OpenGL e carrega textura e shaders.
4. O controlador registra as funcoes de desenho e de entrada do usuario.
5. `glutMainLoop()` mantem a aplicacao ativa e chama os callbacks quando necessario.
6. Ao encerrar, o controlador libera os recursos graficos.

## Controles

- **Setas esquerda/direita**: movem a Terra em X.
- **Setas cima/baixo**: movem a Terra em Z.
- **Shift + seta cima/baixo**: movem a Terra livremente em Y.
- **Mouse sem botoes pressionados**: move a lampada em XZ.
- **Botao direito pressionado + arrasto**: orbita a camera.
- **Teclas 1, 2 e 3**: selecionam Flat, Gouraud e Phong.
