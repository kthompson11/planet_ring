#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <glad/glad.h>

class ShaderProgram
{
public:
    ShaderProgram(const std::string &vShaderPath, const std::string &fShaderPath);

    GLuint ID() { return programID; }

private:
    GLuint programID;
};

#endif /* SHADER_PROGRAM_H */