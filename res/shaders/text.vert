#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec4 vertexColour;

out vec2 UV;
out vec4 colour;

void main() {
    gl_Position = vec4(vertexPosition, 1.0f);
    UV = vertexUV;
    colour = vertexColour;
}