#version 330 core

uniform sampler2DArray uTextureArrayId;
uniform int uCharIndex;
uniform vec4 uColor;

in vec2 texOut;

void main()
{
    float t =  texture(uTextureArrayId, vec3(texOut, uCharIndex)).r;

    // if (t < 0.5f)
    //     discard;
    // float feather = 0.03;
    // t = smoothstep(0.5f - feather, 0.5f + feather, t);
    gl_FragColor = vec4(uColor.xyz, t);

    // gl_FragColor = vec4(1.0f, 0.0f, 0.0f, t);
    // gl_FragColor = vec4(t, t, t, 1.0f);
    // gl_FragColor = uColor;// * vec4(t, t, 1.0, 1.0f);
}