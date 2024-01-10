#version 330 core

uniform sampler2D uTextureId;

in vec2 texOut;

void main()
{
    gl_FragColor = texture(uTextureId, texOut);
}