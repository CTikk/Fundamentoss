#include "barra.h"
#include <iostream>

unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

Barra::Barra(float x, float y, float ancho, float alto, float velocidad, int teclaArriba, int teclaAbajo)
 : x_(x), y_(y), ancho_(ancho), alto_(alto), velocidad_(velocidad), teclaArriba_(teclaArriba), teclaAbajo_(teclaAbajo)
 {
    initializeVertices();
 }

 void Barra::initializeVertices()
 {
    float vertices[] = 
    {
        -ancho_ / 2, -alto_ / 2,
        ancho_ / 2, -alto_ / 2,
        ancho_ / 2, alto_ / 2,
        -ancho_ / 2, alto_ / 2
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)  0);
    glEnableVertexAttribArray(0);
 }

 void Barra::processInput(GLFWwindow* window)
 {
    if(glfwGetKey(window, teclaArriba_) == GLFW_PRESS)
    {
        y_ += velocidad_ * deltaTime;
        if(y_ + alto_ / 2 > 1.0f)
        {
            y_ = 1.0f - alto_ / 2;
        } 
    }

    if(glfwGetKey(window, teclaAbajo_) == GLFW_PRESS)
    {
        y_ -= velocidad_ * deltaTime;
        if(y_ - alto_ / 2 < -1.0f)
        {
            y_ = -1.0f + alto_ / 2;
        }
    }
 }

 void Barra::update()
 {
    // implementar
 }

 void Barra::draw(GLuint shaderProgram)
 {
    glUseProgram(shaderProgram);
    GLint offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    glUniform2f(offsetLoc, x_, y_);
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // Blanco

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 }
// a
 void Barra::destroy()
 {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
 }