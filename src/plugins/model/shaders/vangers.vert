#version 150 core

in vec3 vertexPosition;
in vec3 vertexColor;
varying vec3 color;

out vec3 worldPosition;
uniform mat4 modelMatrix;
uniform mat4 mvp;

void main()
{
    // Transform position, normal, and tangent to world coords
    worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    color = vertexColor;
    // Calculate vertex position in clip coordinates
    gl_Position = mvp * vec4(worldPosition, 1.0);
}
