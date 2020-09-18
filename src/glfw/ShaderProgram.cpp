#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "ShaderProgram.h"

using namespace std;

/******************** Helper Functions ********************/

static string loadShaderSource(const std::string &path)
{
    fstream fileStream;
    fileStream.open(path);
    ostringstream strStream;
    strStream << fileStream.rdbuf();
    string s;
    s = strStream.str();

    return s;
}

static GLuint compileShader(GLenum shaderType, const std::string &path)
{
    string source = loadShaderSource(path);
    const char *source_cstr = source.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source_cstr, nullptr);
    glCompileShader(shader);
    
    // check for compilation errors
    GLint param;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
    if (param == GL_FALSE) {
        string message;
        switch (shaderType)
        {
        case GL_VERTEX_SHADER:
            message = "vertex shader - compile error\n";
            break;
        case GL_FRAGMENT_SHADER:
            message = "fragment shader - compile eror\n";
            break;
        }

        cout << message;
    }

    // print out the log
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param);
    vector<char> log(param);
    glGetShaderInfoLog(shader, param, nullptr, log.data());
    cout << log.data();

    return shader;
}

/******************** Constructors/Destructors ********************/

ShaderProgram::ShaderProgram(const std::string &vShaderPath, const std::string &fShaderPath)
{
    GLuint vShader = compileShader(GL_VERTEX_SHADER, vShaderPath);
    GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fShaderPath);

    programID = glCreateProgram();
    glAttachShader(programID, vShader);
    glAttachShader(programID, fShader);
    glBindAttribLocation(programID, 0, "pos");
    glBindAttribLocation(programID, 1, "color");
    glLinkProgram(programID);

    // cleanup shaders
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    // check for link errors
    GLint param;
    glGetProgramiv(programID, GL_LINK_STATUS, &param);
    if (param == GL_FALSE) {
        cout << "shader program - link error\n";
    }

    // print out the log
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &param);
    vector<char> log(param);
    glGetProgramInfoLog(programID, param, nullptr, log.data());
    cout << log.data();
}
