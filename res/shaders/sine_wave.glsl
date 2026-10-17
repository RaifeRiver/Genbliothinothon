
#define MAX_INT 2147483647
#define PI 3.141592653589793238462643383279502884197169399375105820974944592

float audio_sin(float x) {
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
    s=s*(s*s*15731+789221)+1376312589;
    return (float(s&0x7fffffff)/1073741824.0*2.0-1.0)*vol;
}
float sine_wave(uint samp, int Hz, float vol){
    return (vol*audio_sin(PI*Hz*samp/48000.0));
}
float triangleDepth(uint samp, int Hz, float vol, int depth){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((audio_sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float triangle(uint samp, int Hz, float vol) {
    return triangleDepth(samp, Hz, vol, 10);
}
float ramp_up(uint samp, int Hz, float vol, int depth=80){
    x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((1.0-2.0*((n+1)&1))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
return x;
}
float ramp_down(uint samp, int Hz, float vol, int depth = 80){
    x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((-1*(1.0-2.0*(n&1)))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float square(uint samp, int Hz, float vol, int depth = 200){
    x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((2.0*(n&1))/n*PI)*(vol * 0.7 * audio_sin(n*PI*Hz*samp/48000.0));
    }
return x;
}

float Tramp_down(uint samp, int Hz, float vol){
    float p=96000.0/Hz;
    return(((2*samp/p+h)%2)*-1+1);
}
