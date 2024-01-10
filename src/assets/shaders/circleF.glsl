#version 330 core

uniform vec3 res;
uniform vec4 color;
uniform float uTime;

in vec2 texOut;

void main()
{
    vec2 xy = texOut.xy;
    xy.xy -= 0.5;
    
    float radius = 0.5;
    // float d = length(xy) - radius;
    // d = step(0.00001, d);
    // vec3 color = vec3(d);

    if (length(xy) < radius)
    {
        gl_FragColor = vec4(1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0);
    }

    // gl_FragColor = color;
}
