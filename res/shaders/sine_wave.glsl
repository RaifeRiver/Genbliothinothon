#define MAX_INT 2147483647
#define PI 3.141592653589793238462643383279502884197169399375105820974944592

double audio_sin(double x) {
    double TWO_PI = 2.0 * PI;
    double HALF_PI = 0.5 * PI;

    x = x - TWO_PI * floor(x / TWO_PI + 0.5);

    double sign = 1.0;
    if (x > HALF_PI) {
        x = PI - x;
    } else if (x < -HALF_PI) {
        x = -PI - x;
        sign = -1.0;
    }

    double x2 = x * x;
    double p = -1.6059043836821614e-10;
    p = p * x2 + 2.505210838544172e-8;
    p = p * x2 + -2.755731922398589e-6;
    p = p * x2 + 0.0001984126984126984;
    p = p * x2 + -0.0083333333333333333;
    p = p * x2 + 0.16666666666666666;

    return sign * x * (1.0 - x2 * p);
}

int sine_wave(uint sample, int Hz, float vol){
    return (vol*MAX_INT*audio_sin(PI*Hz*sample/48000.0));
}

int white(uint sample, float vol) {
    int s=(sample<<13)^sample;
    s=s*(s*s*15731+789221)+1376312589;
    return int((float(s&0x7fffffff)/1073741824.0-1.0)*vol*1073741823.5);
}

int triangle(uint sample, int Hz, float vol, int depth = 10) {
int x=0;
for (int n = 1; n < depth; n++){
x+=MAX_INT*((audio_sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*audio_sin(n*PI*Hz*sample/48000.0));
}
return x;
}

int ramp_up(uint sample, int Hz, float vol, int depth = 80) {
    int x=0;
    for (int n = 1; n < depth; n++){
        x+=MAX_INT*((1.0-2.0*((n+1)&1))/(n))*(vol * audio_sin(n*PI*Hz*sample/48000.0));
    }
    return x;
}

int ramp_down(uint sample, int Hz, float vol, int depth = 80) {
int x=0;
for (int n = 1; n < depth; n++){
x+=MAX_INT*((-1*(1.0-2.0*(n&1)))/(n))*(vol * audio_sin(n*PI*Hz*sample/48000.0));
}
return x;
}

int square(uint sample, int Hz, float vol, int depth = 200) {
int x=0;
for (int n = 1; n < depth; n++){
x+=MAX_INT*((2.0*(n&1))/n*PI)*(vol * 0.7 * audio_sin(n*PI*Hz*sample/48000.0));
}
return x;
}
