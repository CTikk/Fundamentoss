#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

const int TRIANGLES = 20;
const float PI = 3.14159265359f;
const float radioExt = 0.8f;//Radio exterior
const float radioInt = 0.4f;//Radio interior

const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    void main() 
    {
        gl_Position = vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() 
    {
        FragColor = vec4(1.0, 0.8, 0.0, 1.0); // Amarillo fuerte
    }
)";
//Vertice de la estrella
vector<float> generateStarVertices()
{
    vector<float> vertices;
    float angleStep = PI / 10.0f; //deberían ser 18 grados
    float angulo = 0;

    for(int i = 0; i < TRIANGLES; i++)
    {
        float angle0 = i * angleStep;
        float angle1 = (i + 1) * angleStep;
        
        float r0 = (i % 2 == 0) ? radioExt : radioInt;
        float r1 = (i % 2 == 0) ? radioInt : radioExt;

        // Centro
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        // angulo actual
        vertices.push_back(cos(angle0) * r0);
        vertices.push_back(sin(angle0) * r0);
        vertices.push_back(0.0f);

        // siguiente angulo
        vertices.push_back(cos(angle1) * r1);
        vertices.push_back(sin(angle1) * r1);
        vertices.push_back(0.0f);
    }
    return vertices;
}

GLuint createShaderProgram()
{
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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Estrella de 10 Puntas", nullptr, nullptr);

    if(!window)
    {
      cerr << "Fallo al crear ventana GLFW \n";
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
     cerr << " Fallo al inicializar GLAD\n";
     return -1;
    }

    GLuint shaderProgram = createShaderProgram();

    // Generar vertices de la estrella
    std::vector<float> vertices = generateStarVertices();

    // Crear VBO y VAO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, TRIANGLES * 3); // 20 triángulos

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

