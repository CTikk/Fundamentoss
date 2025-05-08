#ifndef BARRA_H
#define BARRA_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Barra{
    public:
    float x_, y_; // Posicion
    float ancho_, alto_; // Tamaño
    float velocidad_;
    float deltaTime = 1.0f / 60.0f;
    int teclaArriba_, teclaAbajo_; // Indicador de si se presiona o no la tecla

    GLuint VAO_, VBO_, EBO_;

    Barra(float x, float y, float ancho, float alto, float velocidad, int teclaArriba, int teclaABajo);
    void processInput(GLFWwindow* window);
    void update();
    void draw(GLuint shaderProgram);
    void destroy();

    private:
    void initializeVertices();
};

#endif