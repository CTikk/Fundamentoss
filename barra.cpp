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
    // vértices de la barra
    float vertices[] = 
    {
        -ancho_ / 2, -alto_ / 2, // Esquina inferior izquierda
         ancho_ / 2, -alto_ / 2, // Esquina inferior derecha
         ancho_ / 2,  alto_ / 2, // Esquina superior derecha
        -ancho_ / 2,  alto_ / 2  // Esquina superior izquierda
    };

    // Genera los objetos de buffer de OpenGL para almacenar los vértices e índices
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    // Vincula el VAO y configura los VBO y EBO
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura los atributos de vértices (posición 2D)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)  0);
    glEnableVertexAttribArray(0);
}

// Controles
void Barra::processInput(GLFWwindow* window, float deltaTime)
{
    // Movimiento hacia arriba
    if(glfwGetKey(window, teclaArriba_) == GLFW_PRESS)
    {
        y_ += velocidad_ * deltaTime;
        
        // Evita que la barra se salga de la pantalla superior
        if(y_ + alto_ / 2 > 1.0f)
        {
            y_ = 1.0f - alto_ / 2;
        } 
    }

    // Movimiento hacia abajo
    if(glfwGetKey(window, teclaAbajo_) == GLFW_PRESS)
    {
        y_ -= velocidad_ * deltaTime;
        
        // Evita que la barra se salga de la pantalla inferior
        if(y_ - alto_ / 2 < -1.0f)
        {
            y_ = -1.0f + alto_ / 2;
        }
    }
}


void Barra::update()
{
    // Implementar 
}


void Barra::draw(GLuint shaderProgram)
{
    // Usa el programa de shader proporcionado
    glUseProgram(shaderProgram);
    
    //  posición
    GLint offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    glUniform2f(offsetLoc, x_, y_);

    // color 
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // Blanco

    // Dibuja el rectángulo de la barra 
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//  liberar los recursos asociados con la barra
void Barra::destroy()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}