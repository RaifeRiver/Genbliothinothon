#version 330

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec4 vertexColour;
layout (location = 3) in uint vertexType;

out vec2 UV;
out vec4 colour;
flat out uint type;

uniform uvec2 uScreenSize;

void main() {
    vec2 position = vertexPosition / vec2(uScreenSize) * 2.0f - 1.0f;
    gl_Position = vec4(position.x, -position.y, 0.0f, 1.0f);
    UV = vertexUV;
    colour = vertexColour;
    type = vertexType;
}