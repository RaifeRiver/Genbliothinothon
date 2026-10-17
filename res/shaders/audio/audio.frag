#version 330

#define PI 3.1415926535897932384626433832795

out vec4 audioOut;

uniform uint uSampleBase;

float sine_wave(uint s, float frequency, float volume) {
    return volume * sin(2.0f * PI * frequency * s / 96000.0f);
}

void main() {
    float leftAudio = 0;
    float rightAudio = 0;

    uvec2 sampleVec = uvec2(gl_FragCoord.xy);
    uint sampleI = sampleVec.x + sampleVec.y * 128u + uSampleBase;

    leftAudio = sine_wave(sampleI, 440.0f, 0.3f);
    rightAudio = leftAudio;

    audioOut = vec4(leftAudio, rightAudio, 0.0f, 0.0f);
}