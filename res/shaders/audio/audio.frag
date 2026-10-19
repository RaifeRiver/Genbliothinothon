#version 330

out vec4 audioOut;

uniform uint uSampleBase;

#define MAX_INT 2147483647
#define PI 3.141592653589793238462643383279502884197169399375105820974944592f
#define TWO_PI 6.283185307179586476925286766559005768394338798750211641949889184f
#define HALF_PI 6.2831853071795864769252867665590057683943387987502116419498891846f
#define FS 96000.0

float audio_sin(float x) {

    x = mod(x + PI, TWO_PI) - PI;

    float sign = 1.0f;
    if (x > HALF_PI) {
        x = PI - x;
    } else if (x < -HALF_PI) {
        x = -PI - x;
        sign = -1.0f;
    }

    // 3. Degree-9 Minimax polynomial (uniform error distribution < 1e-7)
    float x2 = x * x;
    float poly = x * (1.0f + x2 * (-0.166666571f + x2 * (0.00833301729f + x2 * (-0.000198066152f + x2 * 0.00000260005477f))));

    return poly * sign;
}



// AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR
//   AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR
//  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR AHDSR








// AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR
//   AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR
//  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR AHDSR


float white(uint samp,float vol){
    uint s=((samp)<<13)^(samp);
    s=s*(s*s*15731u+789221u)+1376312589u;
    return (float(s&0x7fffffffu)/1073741824.0*2.0-1.0)*vol;
}
float sine(uint samp, float Hz, float vol){
    return (vol*audio_sin(PI*Hz*samp/48000.0));
}
float Htri(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((audio_sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Hsaw(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((1.0-2.0*((n+1)&1))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float HIsaw(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((-1*(1.0-2.0*(n&1)))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Hsquare(uint samp, float Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((2.0*(n&1))/n*PI)*(vol * 0.7 * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Isaw(uint samp, float Hz, float vol){
    float p=FS/Hz;
    return(vol*(mod((2u*samp/p+Hz),2)*-1+1));
}
float saw(uint samp, float Hz, float vol){
    float p=FS/Hz;
    return(vol*-1*(mod((2u*samp/p),2)*-1+1));
}
float sq(uint samp, float Hz, float vol){
    return(vol*(2.0*(uint(samp/(FS/Hz))&1u)-1.0));
}
float tri(uint samp, float Hz, float vol){
    return(vol*(1.0-2.0*abs(2.0*fract(samp/(FS/Hz))-1.0)));
}
float note(int n, int octave) {
    return 27.5 * pow(2.0, float(n - 1 + octave * 12) / 12.0);
}
// ==================================================
// MULTIFILER SEPERATE EFFECTED SUPER VOICE SYSTEM!!!
// ==================================================
// LIGHTNING-FAST ULTRA QUAD-SAMPLE SUPER QUAD SYSTEM
// ==================================================

#define FILTER_LOWPASS    0
#define FILTER_HIGHPASS   1
#define FILTER_LOWSHELF   2
#define FILTER_HIGHSHELF  3
#define FILTER_NOTCH      4
#define AUDIO_BUF_SIZE 4

void applyBiquad(inout float Buffer[AUDIO_BUF_SIZE], int type, float hz, float q, float gain) {
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

    for (int i = 0; i < AUDIO_BUF_SIZE; i++) {
        float inSample = Buffer[i];
        float outSample = (b0 * inSample) + (b1 * prevIn[0]) + (b2 * prevIn[1])
        - (a1 * prevOut[0]) - (a2 * prevOut[1]);
        prevIn[1] = prevIn[0]; prevIn[0] = inSample;
        prevOut[1] = prevOut[0]; prevOut[0] = outSample;
        Buffer[i] = outSample;
    }
}

// ==========================================
// VOICE MACROS & CHAINING UTILITIES
// ==========================================

#define VOICE_BEGIN(wavExpr) \
{ \
    float _buf[AUDIO_BUF_SIZE]; \
    for (int _i = 0; _i < AUDIO_BUF_SIZE; _i++) { \
        uint sampleI = sample0 + uint(_i); \
        _buf[_i] = (wavExpr); \
    }

#define VOICE_FILTER(type, hz, q, gain) \
    applyBiquad(_buf, type, hz, q, gain);

#define VOICE_END(outVar) \
    outVar = _buf[AUDIO_BUF_SIZE - 1]; \
}

#define MAKE_VOICE(outVar, wavExpr, type, hz, q, gain) \
    VOICE_BEGIN(wavExpr) \
    VOICE_FILTER(type, hz, q, gain) \
    VOICE_END(outVar)

// insert functions here

void main() {
    uvec2 sampleVec = uvec2(gl_FragCoord.xy);
    uint sample0 = sampleVec.x + (sampleVec.y * 128u) + uSampleBase;

    // insert audio code here

    audioOut = vec4(leftAudio, rightAudio, 0.0f, 0.0f);
}