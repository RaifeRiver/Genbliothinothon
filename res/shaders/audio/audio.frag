#version 330

out vec4 audioOut;

uniform uint uSampleBase;

#define MAX_INT 2147483647
#define PI 3.141592653589793238462643383279502884197169399375105820974944592f
#define TWO_PI 6.283185307179586476925286766559005768394338798750211641949889184f
#define HALF_PI 6.2831853071795864769252867665590057683943387987502116419498891846f
#define e 2.718281828459045235360287471352662497757247093699959574966967627f
#define mS_conversion_num 0.0104166666666666666666666666666666666666666666666666666666666666f
#define S_conversion_num 0.0000104166666666666666666666666666666666666666666666666666666666f
#define FS 96000.0f

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

vec2 AHDSR(float Amplitude_in, float sam, float Attack_Time, float Attack_Curve, float Hold_Time, float Decay_Time, float Decay_Curve, float Sustain_Percent, float Release_Time, float Release_Curve, bool Held, float frequency){
    float Decay_start = Attack_Time + Hold_Time;
    float Sustain_start = Decay_start + Decay_Time;
    float Release_end = Sustain_start + Release_Time;
    float Amplitude = 0.0f;
    float Time = mS_conversion_num * sam;
    //attack
    if (Time <= Attack_Time) {
        float t = Time / Attack_Time;
        float base = (1.0f - pow(e, -Attack_Curve * t)) / (1.0f - pow(e, -Attack_Curve));
        float u = (t > 0.95f) ? (t - 0.95f) * 20.0f : 0.0f;
        Amplitude = (t > 0.95f) ? base + (1.0f - base) * (u * u * (3.0f - 2.0f * u)) : base;
    }
    //hold
    else if (Time < Decay_start) {
        Amplitude = 1.0f;
    }
    //decay&sustain, sustain_start = point where decay ends
    else if (Time < Sustain_start){
        Amplitude = Sustain_Percent + (1-Sustain_Percent) * ((1-(pow(e, ((-1* Decay_Curve * (Sustain_start-Time)) /Decay_Time))))/(1 - (pow(e, (-1 * Decay_Curve)))));
    }
    //sustain when held
    else if (Held == true) {
        Amplitude = Sustain_Percent;
    }
    //release
    else if (Time < Release_end){
        Amplitude = Sustain_Percent * ((1-(pow(e, ((-1 * Release_Curve * (Release_end - Time)) / Release_Time))))/(1-(pow(e, (-1 * Release_Curve)))));
    }
    else {
        Amplitude = 0.0f;
    }
    return vec2(Amplitude*Amplitude_in, frequency);
}

// AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR
//   AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR
//  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR  AHDSR AHDSR


float white(uint samp,float Gain){
    uint s=((samp)<<13)^(samp);
    s=s*(s*s*15731u+789221u)+1376312589u;
    return (float(s&0x7fffffffu)/1073741824.0*2.0-1.0)*Gain;
}
float sine(uint samp, float Hz, float Gain){
    return (Gain*audio_sin(PI*Hz*samp/48000.0));
}
float Htri(uint samp, float Hz, float Gain, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((audio_sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*Gain*audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Hsaw(uint samp, float Hz, float Gain, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((1.0-2.0*((n+1)&1))/(n))*(Gain * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float HIsaw(uint samp, float Hz, float Gain, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((-1*(1.0-2.0*(n&1)))/(n))*(Gain * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Hsquare(uint samp, float Hz, float Gain, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((2.0*(n&1))/n*PI)*(Gain * 0.7 * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float Isaw(uint samp, float Hz, float Gain){
    float p=FS/Hz;
    return(Gain*(mod((2u*samp/p+Hz),2)*-1+1));
}
float saw(uint samp, float Hz, float Gain){
    float p=FS/Hz;
    return(Gain*-1*(mod((2u*samp/p),2)*-1+1));
}
float sq(uint samp, float Hz, float Gain){
    return(Gain*(2.0*(uint(samp/(FS/Hz))&1u)-1.0));
    }
float tri(uint samp, float Hz, float Gain){
    return(float(Gain*(1.0-2.0*abs(2.0*fract(samp/(FS/Hz))-1.0))));
}
//float AT, float AC, float HT, float DT, float DC, float SP, float RT, float RC
float note(int n, int octave) {
    return 27.5 * pow(2.0, float(n - 1 + octave * 12) / 12.0);
}
vec2 Tremlo(uint Samp, float Amp, float Period, float Gain, float Smooth) {
    float x = tanh(sin((PI*Samp)/(FS*Period)));
    float left = Gain*((tanh(sin(x*Smooth))/Smooth)*0.5f)+1.0f;
    float right = Gain*((-1.0f*(tanh(sin(x*Smooth)))/Smooth)*0.5f)+1.0f;
    return vec2(left, right);
}
float XOR(float Amp, float idunno) { // RaifeRiver: "idunno"
                                     return(mod(Amp+idunno,2.0f));
}
float BitCrusher(float Amp, int Bits, float Gain) {
    return ((Bits <= 1 ? (Amp >= 0.0 ? 1.0 : -1.0) : ((floor((Amp * 0.5 + 0.5) * exp2(float(Bits))) / exp2(float(Bits))) * 2.0 - 1.0)) * Gain);
}// folded
float OverInvert(float Amp, float Drive, float Gain) {
    return float(-1.0f*(((mod((Amp*Drive),2.0f))-1.0f))*Gain);
}
float SoftDistort(float Amp, float Drive, float Gain) {
    return float((tanh(Amp*Drive))*Gain);
}
float HardClip(float Amp, float Drive, float Gain) {
    return float(clamp(Amp*Drive, -1.0f, 1.0f)*Gain);
}
float FullRectifier(float Amp, float Drive, float Gain) {
    return float(abs(Amp*Drive)*Gain);
}// waves positive
float AntiDepressant(float Amp, float Drive, float Gain) {
    return float(max(Amp*Drive,0.0f)*Gain);
}// waves negative
float Depresser(float Amp, float Drive, float Gain) {
    return float(min(Amp*Drive, 0)*Gain);
}
float TapeSaturation(float Amp, float Drive, float Gain) {
    return float((tanh(Amp*Drive))/(tanh(Drive))*Gain);
}
float TubeSaturation(float Amp, float Drive, float Gain) {
    return float((Amp*Drive)/(1+Drive*abs(Amp)));
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
        uint sampleI = s + uint(_i); \
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

float noise(uint s, float volume, float lowPass, float highPass) {
    float amplitude;
    VOICE_BEGIN(white(sampleI,volume))
    VOICE_FILTER(FILTER_LOWPASS,lowPass,0.0f,1.0f)
    VOICE_FILTER(FILTER_HIGHPASS,highPass,0.0f,1.0f)
    VOICE_END(amplitude)
    return amplitude;
}

// insert functions here

void main() {
    uvec2 sampleVec = uvec2(gl_FragCoord.xy);
    uint sample0 = sampleVec.x + (sampleVec.y * 128u) + uSampleBase;

    // insert audio code here

    audioOut = vec4(leftAudio, rightAudio, 0.0f, 0.0f);
}
