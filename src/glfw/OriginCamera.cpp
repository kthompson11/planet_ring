
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OriginCamera.h"

using namespace std;

OriginCamera::OriginCamera(float theta, float phi, float verticalSensitivity, 
    float horizontalSensitivity, float frustrumSize, ShaderProgram &program) :
    theta(theta), phi(phi), verticalSensitivity(verticalSensitivity), 
    horizontalSensitivity(horizontalSensitivity)
{
    // set projection matrix
    double halfSize = frustrumSize / 2;
    glm::mat4 projection = glm::ortho(-halfSize, halfSize, 
                                      -halfSize, halfSize, 
                                      -halfSize, halfSize);
    GLint projectionLoc = glGetUniformLocation(program.ID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // set view matrix
    setView(program);
}

void OriginCamera::update(GLFWwindow *window, ShaderProgram &program)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (lmbPressed) {
        // rotate view matrix
        float xDiff = xPos - xLast;
        float yDiff = yPos - yLast;
        theta += xDiff * horizontalSensitivity;
        phi += yDiff * verticalSensitivity;

        // clamp phi
        if (phi > 180.0f) {
            phi = 180.0f;
        } else if (phi < 0.0f) {
            phi = 0.0f;
        }
        
        setView(program);
    }
    xLast = xPos;
    yLast = yPos;

    int status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (status == GLFW_PRESS) {
        lmbPressed = true;
    } else {
        lmbPressed = false;
    }
}

void OriginCamera::setView(ShaderProgram &program)
{
    glm::mat4 view(1.0f);
    view = glm::rotate(view, glm::radians(-phi), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(theta), glm::vec3(0.0f, 0.0f, 1.0f));
    GLint viewLoc = glGetUniformLocation(program.ID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}