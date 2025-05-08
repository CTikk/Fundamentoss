#ifndef PELOTA_H
#define PELOTA_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "barra.h"

class Pelota {
public:
    float x_, y_;
    float vx_, vy_;
    float velocidad_;
    float radio_;
    float tiempoEspera = 5.0f;
    float tiempoReinicio;
    bool espera;

    GLuint VAO_, VBO_, EBO_;

    Pelota(float velocidadInicial = 1.0f, float radio = 0.025f);
    void move();
    void draw(GLuint shaderProgram);
    void reset();

    bool collidesWith(const Barra& barra);
    bool timer(float deltaTime);
    int checkPoint(); // Devuelve 1 si punto jugador 1, 2 si punto jugador 2, 0 si ninguno

    void destroy();
};

#endif