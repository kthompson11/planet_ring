#version 120

// inputs
attribute vec3 pos;
attribute vec3 color;

// outputs
varying vec3 vertexColor;

// uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vertexColor = color;
}
