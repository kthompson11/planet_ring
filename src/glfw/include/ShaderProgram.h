#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <glad/glad.h>

#define VERTEX_DATA_SIZE                (6 * sizeof(float))
#define VERTEX_ELEMENT_FLOAT_COUNT      (VERTEX_DATA_SIZE / sizeof(float))

class ShaderProgram
{
public:
    ShaderProgram(const std::string &vShaderPath, const std::string &fShaderPath);

    GLuint ID() { return programID; }

private:
    GLuint programID;
};

#endif /* SHADER_PROGRAM_H */