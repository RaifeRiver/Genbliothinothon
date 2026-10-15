// sin(t)=amp, output amp as audio data;
// sample rate 96Khz; bit depth 32 bit; bit rate 24KB/s???;
// 2 methods for audio rep. one is binary 32 bit at set time;
// other is set of sine waves with amplitude multiplier;
#define MAX_INT 2147483647

float sine_wave(int sample, int Hz, float vol){
    float t = sample / 96000.0;
    return (vol*sin(2.0 * 3.1415926535897932384626433832795 * t * Hz));
}

float ramp_up(int sample, int Hz, float vol, int depth = 80) {
    float t = sample / 96000.0;
    float x=0;
    for (int n = 1; n < depth; n++){
        x+=((1.0-2.0*((n+1)&1))/(n))*(vol * sin(2.0 * 3.1415926535897932384626433832795 * t * Hz * n));
    }
    return x;
}

float ramp_down(int sample, int Hz, float vol, int depth = 80) {
    float t = sample / 96000.0;
float x=0;
for (int n = 1; n < depth; n++){
x+=((-1*(1.0-2.0*(n&1)))/(n))*(vol * sin(2.0 * 3.1415926535897932384626433832795 * t * Hz * n));
}
return x;
}






