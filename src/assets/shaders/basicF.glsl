#version 330 core

uniform vec3 res;
uniform vec4 color;
uniform float uTime;

in vec2 texOut;

void main()
{
   gl_FragColor = color;
   // gl_FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

// void main()
// {
//    float ar = res.x / res.y;

//    vec2 tex = texOut;
//    tex.x *= ar;
//    tex.y = 1.0f - tex.y;
//    tex = tex*4;

//    tex.x = tex.x + mod(int(tex.x), 2) / 2.f;
//    tex.y = tex.y + mod(int(tex.y), 2) / 2.0f;
//    tex.xy = fract(tex.xy);
//    tex.xy -= 0.5f;
//    // float d = length(tex.xy)- uTime*0.5f;
//    float d = length(tex.xy) - 0.4f;

//    d = fract(d);
//    d = step(0.05f, d);
//    // d = smoothstep(0.05f, 0.1f, d);
//    gl_FragColor = vec4(d, d, 0.0f, 1.0f + uTime);
//    // gl_FragColor = vec4(tex.xy, 0.0f, 1.0f);

// }

/*
// Interesting clepsidra
void main()
{
   vec2 tex = texOut * 1.0f;
   float aspect = tex.x / tex.y;
   tex.xy -= 0.5f;
   tex.x /= tex.y;
   // float d = length(tex.xy)-0.25f;
   float d = step(0.2f, length(tex.xy)-0.25f);
   // gl_FragColor = vec4(fract(tex.xy), 0.0f, 1.0f);
   gl_FragColor = vec4(d, d, 0.0f, 1.0f);
}

*/