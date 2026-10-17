#version 330

out vec4 fragColour;

uniform float uFrequencies[512];
uniform uvec2 uScreenSize;

void main() {
    float frequency = uFrequencies[int(512.0f * (floor(gl_FragCoord.x) / float(uScreenSize.x)))];
    if (abs(gl_FragCoord.y / float(uScreenSize.y) - (frequency + 1.0f) * 0.5f) <= 0.001f) {
        fragColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else if(uint(gl_FragCoord.y) == uScreenSize.y / 2u) {
        fragColour = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }
    else if(abs(int(gl_FragCoord.y) - int(uScreenSize.y) / 2) % 40 == 0) {
        fragColour = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    }
    else if(abs(int(gl_FragCoord.x) - int(uScreenSize.x) / 2) % 40 == 0) {
        fragColour = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    }
    else {
        fragColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}