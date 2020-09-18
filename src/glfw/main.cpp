
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Simulation.h"

using namespace std;






const char* vertshaderSource = R"(
    #version 120
    attribute vec3 pos;

    void main() {
        gl_Position = vec4(pos.xyz, 1);
    }
)";
const char* fragshaderSource = R"(
    #version 120

    void main() {
        gl_FragColor = vec4(1, 0, 0, 1);
    }
)";

struct vert {
    float x, y, z;
};






string loadShaderSource(const char *path)
{
    fstream fileStream;
    fileStream.open(path);
    stringstream strStream;
    strStream << fileStream.rdbuf();
    string s;
    s = strStream.str();

    return s;
}

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

    GLint paramValue;

    // vertex shader compilation
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    string shaderSource = loadShaderSource("shader.vs");
    const char *shaderSource_cstr = shaderSource.c_str();
    glShaderSource(vShader, 1, &shaderSource_cstr, NULL);
    glCompileShader(vShader);
    // check if compilation succeeded and print out info log
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &paramValue);
    if (paramValue != GL_TRUE) {
        GLchar infoLog[500];
        glGetShaderInfoLog(vShader, 500, NULL, infoLog);
        std::cout << "vShader - compile error\n";
        cout << infoLog;
    } else {
        // print out the infolog if it is not empty
        glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &paramValue);
        if (paramValue != 0) {
            GLchar infoLog[500];
            glGetShaderInfoLog(vShader, 500, NULL, infoLog);
            cout << infoLog;
        }
    }

    // fragment shader compilation
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderSource = loadShaderSource("shader.fs");
    shaderSource_cstr = shaderSource.c_str();
    glShaderSource(fShader, 1, &shaderSource_cstr, NULL);
    glCompileShader(fShader);
    // check if compilation succeeded and print out info log
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &paramValue);
    if (paramValue != GL_TRUE) {
        GLchar infoLog[500];
        glGetShaderInfoLog(fShader, 500, NULL, infoLog);
        std::cout << "fShader - compile error\n";
        cout << infoLog;
    } else {
        // print out the infolog if it is not empty
        glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &paramValue);
        if (paramValue != 0) {
            GLchar infoLog[500];
            glGetShaderInfoLog(fShader, 500, NULL, infoLog);
            cout << infoLog;
        }
    }

    // create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    // bind attributes
    glBindAttribLocation(shaderProgram, 0, "pos");
    //glBindAttribLocation(shaderProgram, 1, "color");
    glLinkProgram(shaderProgram);
    // check if link succeeded
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &paramValue);
    if (paramValue != GL_TRUE) {
        GLchar infoLog[500];
        glGetProgramInfoLog(shaderProgram, 500, NULL, infoLog);
        cout << "asdf\n";
        std::cout << "shaderProgram - link error\n";
        cout << infoLog;
    } else {
        // print out the infolog if it is not empty
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &paramValue);
        if (paramValue != 0) {
            GLchar infoLog[500];
            glGetProgramInfoLog(shaderProgram, 500, NULL, infoLog);
            cout << infoLog;
        }
    }

    // free shader resources
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    // set current program
    glUseProgram(shaderProgram);
    //  enable the attribute array indices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)3);
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    // shader program setup done; simulation objects should manage vertex buffers

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    GLuint vbuf;
    glGenBuffers(1, &vbuf);

    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}