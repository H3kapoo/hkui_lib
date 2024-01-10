#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

uniform mat4 modelMatrix;
uniform mat4 projMatrix;

out vec2 texOut;

void main()
{
    texOut = vTex;
    gl_Position = projMatrix * modelMatrix * vec4(vPos.xyz, 1.0);
}
