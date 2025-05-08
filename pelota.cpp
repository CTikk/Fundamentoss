#include "pelota.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

float verticesPelota[] = {
    -0.025f, -0.025f,
     0.025f, -0.025f,
     0.025f,  0.025f,
    -0.025f,  0.025f
};

unsigned int indicesPelota[] = {
    0, 1, 2,
    2, 3, 0
};

Pelota::Pelota(float velocidadInicial, float radio)
    : x_(0.0f), y_(0.0f), velocidad_(velocidadInicial), radio_(radio)
{
    std::srand((unsigned)std::time(0));
    float angle = ((std::rand() % 120) - 60) * 3.14159f / 180.0f;
    vx_ = std::cos(angle) * velocidad_;
    vy_ = std::sin(angle) * velocidad_;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPelota), verticesPelota, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPelota), indicesPelota, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
bool Pelota::timer(float deltaTime)
{
    if (espera) 
    {
        tiempoReinicio += deltaTime;
        if (tiempoReinicio >= tiempoEspera) 
        {
            espera = false;
            tiempoReinicio = 0.0f;
            return true;
        }
    }
    return false;
}

void Pelota::move(float deltaTime) {
    x_ += vx_*deltaTime;
    y_ += vy_*deltaTime;

    if (y_ + radio_ >= 1.0f || y_ - radio_ <= -1.0f) {
        vy_ = -vy_; // Rebote vertical
    }
}

bool Pelota::collidesWith(const Barra& barra) {
    float barraIzq = barra.x_ - barra.ancho_ / 2;
    float barraDer = barra.x_ + barra.ancho_ / 2;
    float barraArr = barra.y_ + barra.alto_ / 2;
    float barraAba = barra.y_ - barra.alto_ / 2;

    return (x_ + radio_ > barraIzq && x_ - radio_ < barraDer &&
            y_ + radio_ > barraAba && y_ - radio_ < barraArr);
}

int Pelota::checkPoint() {
    if (x_ - radio_ > 1.0f) return 1; // Jugador 1 anota
    if (x_ + radio_ < -1.0f) return 2; // Jugador 2 anota
    return 0;
}

void Pelota::draw(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    GLint offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    glUniform2f(offsetLoc, x_, y_);
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform4f(colorLoc, 0.9f, 0.2f, 0.2f, 1.0f); // Rojo

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Pelota::reset() {

        espera = true;
        tiempoReinicio = 0.0f;
        x_ = y_ = 0.0f;
        float angle = ((std::rand() % 120) - 60) * 3.14159f / 180.0f;
        float dir = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
        vx_ = dir * std::cos(angle) * velocidad_;
        vy_ = std::sin(angle) * velocidad_ ;
    
       
}

void Pelota::destroy() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}