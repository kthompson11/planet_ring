#ifndef ORIGINCAMERA_H
#define ORIGINCAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"

class OriginCamera
{
public:
    OriginCamera(float theta, float phi, float verticalSensitivity, float horizontalSensitivity, float frustrumSize, ShaderProgram &program);

    void update(GLFWwindow *window, ShaderProgram &program);

private:
    float theta, phi;
    float horizontalSensitivity, verticalSensitivity;
    double xLast, yLast;
    bool lmbPressed = false;

    void setView(ShaderProgram &program);
};

#endif /* ORIGIN_CAMERA_H */