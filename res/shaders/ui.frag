#version 330

in vec2 UV;
in vec4 colour;
flat in uint type;

out vec4 fragColour;

uniform sampler2D uFontAtlas;
uniform sampler2D uUIAtlas;

void main() {
    if (type == 0u) {
        vec3 msd = texture(uFontAtlas, UV).rgb;
        float sd = max(min(msd.r, msd.g), min(max(msd.r, msd.g), msd.b)) - 0.5f;
        ivec2 atlasSize = textureSize(uFontAtlas, 0);
        vec2 screenTextureSize = vec2(dFdx(UV.x), dFdy(UV.y)) * atlasSize;
        float pixelDistance = sd * max(2.0f / length(screenTextureSize), 1.0f);
        float opacity = clamp(pixelDistance + 0.5f, 0.0f, 1.0f);
        fragColour = vec4(colour.rgb, colour.a * opacity);
    }
    else if (type == 1u) {
        fragColour = colour * texture(uUIAtlas, UV);
    }
    else {
        fragColour = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    }
}