
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "ShaderProgram.h"
#include "PlanetRingSim.h"

using namespace std;

static const int N_PARTICLES = 8;
static const double dt = 1;

void handleMouse(GLFWwindow *window, ShaderProgram &program);
void resizeViewport(GLFWwindow* window, GLsizei width, GLsizei height);

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
    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW - failed to create window\n";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // set callbacks
    glfwSetFramebufferSizeCallback(window, resizeViewport);

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
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);  

    // shader program setup done; simulation objects should manage vertex buffers

    {
        /******************** n-body simulation ********************/
        PlanetRingSim sim(200, window, shaderProgram);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            sim.draw();
            sim.step(1);

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

void resizeViewport(GLFWwindow* window, GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
}