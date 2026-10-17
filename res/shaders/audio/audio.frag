#version 330

out vec4 audioOut;

uniform uint uSampleBase;

#define MAX_INT 2147483647
#define PI 3.141592653589793238462643383279502884197169399375105820974944592
#define FS 96000.0

float audio_sin(float x) {return sin(x);
    float TWO_PI = 2.0 * PI;
    float HALF_PI = 0.5 * PI;

    x = x - TWO_PI * floor(x / TWO_PI + 0.5);

    float sign = 1.0;
    if (x > HALF_PI) {
        x = PI - x;
    } else if (x < -HALF_PI) {
        x = -PI - x;
        sign = -1.0;
    }

    float x2 = x * x;
    float p = -1.6059043836821614e-10;
    p = p * x2 + 2.505210838544172e-8;
    p = p * x2 + -2.755731922398589e-6;
    p = p * x2 + 0.0001984126984126984;
    p = p * x2 + -0.0083333333333333333;
    p = p * x2 + 0.16666666666666666;

    return sign * x * (1.0 - x2 * p);
}
float white(uint samp,float vol){
    uint s=((samp)<<13)^(samp);
    s=s*(s*s*15731u+789221u)+1376312589u;
    return (float(s&0x7fffffffu)/1073741824.0*2.0-1.0)*vol;
}
float sine_wave(uint samp, float Hz, float vol){
    return (vol*audio_sin(PI*Hz*samp/48000.0));
}
float triangle(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((audio_sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float ramp_up(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((1.0-2.0*((n+1)&1))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float ramp_down(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((-1*(1.0-2.0*(n&1)))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float square(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((2.0*(n&1))/n*PI)*(vol * 0.7 * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Tramp_down(uint samp, float Hz, float vol){
    float p=FS/Hz;
    return(vol*(mod((2u*samp/p+Hz),2)*-1+1));
}
float Tramp_up(uint samp, float Hz, float vol){
    float p=FS/Hz;
    return(vol*(mod((2u*samp/p),2)+1));
}
float Tsquare(uint samp, float Hz, float vol){
    return(vol*(2.0*(uint(samp/(FS/Hz))&1u)-1.0));
}
float Ttriangle(uint samp, float Hz, float vol){
    return(vol*(1.0-2.0*abs(2.0*fract(samp/(FS/Hz))-1.0)));
}
float note(int n, int octave) {
    return 27.5 * pow(2.0, float(n - 1 + octave * 12) / 12.0);
}

//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================

// ==========================================
// AUDIO FILTER & MULTI-VOICE SYSTEM
// ==========================================

// Filter Type Definitions
#define FILTER_LOWPASS    0
#define FILTER_HIGHPASS   1
#define FILTER_LOWSHELF   2
#define FILTER_HIGHSHELF  3
#define FILTER_NOTCH      4

// Robust Biquad filter core implementing Robert Bristow-Johnson formulas
void applyBiquad(inout float buffer[1024], int type, float hz, float q, float gain) {
    float w0 = 2.0 * PI * clamp(hz, 20.0, FS * 0.49) / FS;
    float cosW0 = cos(w0);
    float sinW0 = sin(w0);
    float alpha = sinW0 / (2.0 * clamp(q, 0.1, 10.0));

    float b0 = 0.0, b1 = 0.0, b2 = 0.0;
    float a0 = 1.0, a1 = 0.0, a2 = 0.0;

    if (type == FILTER_LOWPASS) {
        b0 = (1.0 - cosW0) * 0.5; b1 = 1.0 - cosW0; b2 = (1.0 - cosW0) * 0.5;
        a0 = 1.0 + alpha; a1 = -2.0 * cosW0; a2 = 1.0 - alpha;
    } else if (type == FILTER_HIGHPASS) {
        b0 = (1.0 + cosW0) * 0.5; b1 = -(1.0 + cosW0); b2 = (1.0 + cosW0) * 0.5;
        a0 = 1.0 + alpha; a1 = -2.0 * cosW0; a2 = 1.0 - alpha;
    } else if (type == FILTER_NOTCH) {
        b0 = 1.0; b1 = -2.0 * cosW0; b2 = 1.0;
        a0 = 1.0 + alpha; a1 = -2.0 * cosW0; a2 = 1.0 - alpha;
    } else {
        float dbGain = clamp(gain, -1.0, 1.0) * 24.0;
        float A = pow(10.0, dbGain / 40.0);
        float sqrtA = sqrt(A);
        float alphaShelf = alpha * sqrt(2.0);
        if (type == FILTER_LOWSHELF) {
            b0 = A * ((A + 1.0) - (A - 1.0) * cosW0 + 2.0 * sqrtA * alphaShelf);
            b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cosW0);
            b2 = A * ((A + 1.0) - (A - 1.0) * cosW0 - 2.0 * sqrtA * alphaShelf);
            a0 = (A + 1.0) + (A - 1.0) * cosW0 + 2.0 * sqrtA * alphaShelf;
            a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cosW0);
            a2 = (A + 1.0) + (A - 1.0) * cosW0 - 2.0 * sqrtA * alphaShelf;
        } else {
            b0 = A * ((A + 1.0) + (A - 1.0) * cosW0 + 2.0 * sqrtA * alphaShelf);
            b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosW0);
            b2 = A * ((A + 1.0) + (A - 1.0) * cosW0 - 2.0 * sqrtA * alphaShelf);
            a0 = (A + 1.0) - (A - 1.0) * cosW0 + 2.0 * sqrtA * alphaShelf;
            a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cosW0);
            a2 = (A + 1.0) - (A - 1.0) * cosW0 - 2.0 * sqrtA * alphaShelf;
        }
    }

    float invA0 = 1.0 / a0;
    b0 *= invA0; b1 *= invA0; b2 *= invA0; a1 *= invA0; a2 *= invA0;

    float prevIn[2] = float[2](0.0, 0.0);
    float prevOut[2] = float[2](0.0, 0.0);

    for (int i = 0; i < 1024; i++) {
        float inSample = buffer[i];
        float outSample = (b0 * inSample) + (b1 * prevIn[0]) + (b2 * prevIn[1])
                        - (a1 * prevOut[0]) - (a2 * prevOut[1]);
        prevIn[1] = prevIn[0]; prevIn[0] = inSample;
        prevOut[1] = prevOut[0]; prevOut[0] = outSample;
        buffer[i] = outSample;
    }
}

// ==========================================
// VOICE MACROS & CHAINING UTILITIES
// ==========================================

#define VOICE_BEGIN(wavExpr) \
{ \
    float _buf[1024]; \
    for (int _i = 0; _i < 1024; _i++) { \
        uint sampleI = baseSampleI + uint(_i); \
        _buf[_i] = (wavExpr); \
    }

#define VOICE_FILTER(type, hz, q, gain) \
    applyBiquad(_buf, type, hz, q, gain);

#define VOICE_END(outVar) \
    outVar = _buf[1023]; \
}

#define MAKE_VOICE(outVar, wavExpr, type, hz, q, gain) \
    VOICE_BEGIN(wavExpr) \
    VOICE_FILTER(type, hz, q, gain) \
    VOICE_END(outVar)

// ==========================================
// MAIN SHADER ENTRY
// ==========================================

void main() {
    uvec2 sampleVec = uvec2(gl_FragCoord.xy);
    uint baseSampleI = sampleVec.x + (sampleVec.y * 128u) + uSampleBase;

    float voice1, voice2;

    // Voice 1: Single-filter setup (Lowpass)
    MAKE_VOICE(voice1, Tramp_up(sampleI, note(4, 1), 0.7f), FILTER_LOWPASS, 300.0, 1.0, 0.0);

    // Voice 2: Chained multi-filter setup (800Hz HPF + 2kHz HPF)
    VOICE_BEGIN(Tramp_up(sampleI, note(3, 1), 0.5f))
        VOICE_FILTER(FILTER_HIGHPASS, 800.0, 0.7, 0.0)
        VOICE_FILTER(FILTER_LOWPASS, 2000.0, 0.7, 0.0)
    VOICE_END(voice2)

    // Mix voices together safely and output directly to audioOut
    float finalMix = (voice1 + voice2) * 0.5f;
    audioOut = vec4(finalMix, finalMix, 0.0f, 0.0f);
}