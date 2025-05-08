#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "barra.h" // clase de barras
#include "pelota.h" // clase de la pelota 


// === Shaders ===
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 uOffset;
void main() {
    gl_Position = vec4(aPos + uOffset, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 uColor;
    void main() {
        FragColor = uColor;
    }
)";

// === Callback para tamaño de ventana ===
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// === Compilar Shaders ===
GLuint crearShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main()
{
    // === Inicializar GLFW ===
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Pong - Variante 1", nullptr, nullptr);
    if(!window)
    {
        std::cerr << "Error al crear la ventana \n";
        glfwTerminate;
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error al inicializar GLAD\n";
        return -1;
    }

    GLuint shaderProgram = crearShaderProgram();

    // Crear las dos barras
    Barra barra1(-0.9f, 0.0f, 0.05f, 0.3f, 0.02f, GLFW_KEY_W, GLFW_KEY_S);         // Jugador 1
    Barra barra2(0.9f, 0.0f, 0.05f, 0.3f, 0.02f, GLFW_KEY_UP, GLFW_KEY_DOWN);     // Jugador 2

    // Crear la pelota
    Pelota pelota(0.01f); // velocidad inicial
    int puntajeJugador1 = 0;
    int puntajeJugador2 = 0;
    // Loop principal
    while (!glfwWindowShouldClose(window)) 
    {
        // Entrada
        glfwPollEvents();
        barra1.processInput(window);
        barra2.processInput(window);
        if(!pelota.espera)
        {
            pelota.move();

            
        
        

        // Colisión con barras
        if (pelota.vx_ < 0 && pelota.collidesWith(barra1)) {
            pelota.vx_ = -pelota.vx_;
        }
        if (pelota.vx_ > 0 && pelota.collidesWith(barra2)) {
            pelota.vx_ = -pelota.vx_;
        }

        // Verificar puntos
        int punto = pelota.checkPoint();
        if (punto != 0) 
        {
            std::cout << "Jugador " << punto << " anota un punto!\n";
            if(punto == 1)
            {
                puntajeJugador1 += 1;
                std::cout << "Jugador 1 lleva: " << puntajeJugador1 << " puntos \n";
            }
            else if(punto == 2)
            {
              puntajeJugador2 += 1;
              std::cout << "Jugador 2 lleva: " << puntajeJugador1 << " puntos \n";
            }

            
            pelota.reset();
        }
        }
        else
        {
          pelota.timer();
        }
        // Renderizado
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        barra1.draw(shaderProgram);
        barra2.draw(shaderProgram);
        pelota.draw(shaderProgram);

        glfwSwapBuffers(window);
    }

    // Liberar recursos
    barra1.destroy();
    barra2.destroy();
    pelota.destroy();
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}