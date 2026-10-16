#version 330

in vec2 UV;
in vec4 colour;

out vec4 fragColour;

uniform sampler2D uFontAtlas;

void main() {
    vec3 msd = texture(uFontAtlas, UV).rgb;
    float sd = max(min(msd.r, msd.g), min(max(msd.r, msd.g), msd.b)) - 0.5f;
    float opacity = clamp(sd * 2.0f + 0.5f, 0.0f, 1.0f);
    fragColour = vec4(colour.rgb, colour.a * opacity);
}