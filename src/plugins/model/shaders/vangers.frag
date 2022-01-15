#version 150 core

uniform vec3 maincolor;
varying vec3 color;

out vec4 fragColor;

void main()
{
    //output color from material
    fragColor = vec4(color, 1.0);
}

