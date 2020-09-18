#version 120

// inputs
attribute vec3 pos;
//vec3 color;

// outputs
//vec3 vertexColor;

// uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
    //vertexColor = color;
}
