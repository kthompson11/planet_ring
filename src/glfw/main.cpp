
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "ShaderProgram.h"
#include "Simulation.h"

using namespace std;

static const int N_PARTICLES = 2;
static const double dt = 1;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "GLFW - failed to initialize\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW - failed to create window\n";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // load GLAD opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD - failed to load\n";
        return -1;
    }

    /******************** shader program ********************/

    ShaderProgram shaderProgram("shader.vs", "shader.fs");

    // set current program
    glUseProgram(shaderProgram.ID());
    //  enable the attribute array indices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // shader program setup done; simulation objects should manage vertex buffers

    float vertices[] = {
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f,
    };

    GLuint vbuf;
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    {
        /******************** n-body simulation ********************/
        //Simulation sim(N_PARTICLES, 1, shaderProgram);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //sim.draw();
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glDeleteProgram(shaderProgram.ID());
    glfwTerminate();

    return 0;
}