#version 330

#define MAX_INT 2147483647
#define PI 3.1415926535897932384626433832795

out ivec4 audioOut;

uniform uint uSampleBase;

int sine_wave(uint sample, int Hz, float vol){
    return int(vol*MAX_INT*sin(2.0*PI*Hz*sample/96000.0));
}

void main() {
    int leftAudio = 0;
    int rightAudio = 0;

    uvec2 sampleVec = uvec2(gl_FragCoord.xy);
    uint sampleI = sampleVec.x + sampleVec.y * 128u + uSampleBase;

    leftAudio = sine_wave(sampleI, 440, 0.3);

    audioOut = ivec4(leftAudio, rightAudio, 0, 0);
}