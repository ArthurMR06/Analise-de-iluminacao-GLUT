cmake_minimum_required(VERSION 3.16)
project(ICG_Terra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(ICG_Terra
    src/main.cpp
    src/Cena.cpp
    src/Controles.cpp
    src/Phong.cpp
    src/Terra.cpp
)

target_include_directories(ICG_Terra PRIVATE include)
target_link_libraries(ICG_Terra PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)

# Copia os arquivos GLSL para a pasta de compilacao.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
