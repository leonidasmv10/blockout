#version 430 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 color;
uniform bool isBinding;

uniform float mixed;
uniform float blending;

void main()
{
    if (isBinding)
    {
        vec4 res = mix(color, texture(texture1, TexCoord), mixed);
        FragColor = vec4(res.x, res.y, res.z, blending);
    }
    else
    {
        FragColor = color;
    }
}