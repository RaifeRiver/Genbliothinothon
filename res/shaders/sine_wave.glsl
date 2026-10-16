// sin(t)=amp, output amp as audio data;
// sample rate 96Khz; bit depth 32 bit; bit rate 24KB/s???;
// 2 methods for audio rep. one is binary 32 bit at set time;
// other is set of sine waves with amplitude multiplier;
#define MAX_INT 2147483647
#define PI 3.1415926535897932384626433832795

int sine_wave(int sample, int Hz, float vol){
    float t = sample / 96000.0;
    return (vol*MAX_INT*sin(2.0 * PI * t * Hz));
}

int triangle(int sample, int Hz, float vol, int depth = 10) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=MAX_INT*((sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*sin(2.0 * PI * t * Hz * n));
}
return x;
}

int ramp_up(int sample, int Hz, float vol, int depth = 80) {
    float t = sample / 96000.0;
    int x=0;
    for (int n = 1; n < depth; n++){
        x+=MAX_INT*((1.0-2.0*((n+1)&1))/(n))*(vol * sin(2.0 * PI * t * Hz * n));
    }
    return x;
}

int ramp_down(int sample, int Hz, float vol, int depth = 80) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=MAX_INT*((-1*(1.0-2.0*(n&1)))/(n))*(vol * sin(2.0 * PI * t * Hz * n));
}
return x;
}

int square(int sample, int Hz, float vol, int depth = 200) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=MAX_INT*((2.0*(n&1))/n*PI)*(vol * 0.7 * sin(2.0 * PI * t * Hz * n));
}
return x;
}
// below here things are ints from sin so it could make problems.
int Msine_wave(int sample, int Hz, float vol){
    float t = sample / 96000.0;
return (vol*sin(2.0 * PI * t * Hz));
}

int Mtriangle(int sample, int Hz, float vol, int depth = 10) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=((sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*sin(2.0 * PI * t * Hz * n));
}
return x;
}

int Mramp_up(int sample, int Hz, float vol, int depth = 80) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=((1.0-2.0*((n+1)&1))/(n))*(vol * sin(2.0 * PI * t * Hz * n));
}
return x;
}

int Mramp_down(int sample, int Hz, float vol, int depth = 80) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=((-1*(1.0-2.0*(n&1)))/(n))*(vol * sin(2.0 * PI * t * Hz * n));
}
return x;
}

int Msquare(int sample, int Hz, float vol, int depth = 200) {
    float t = sample / 96000.0;
int x=0;
for (int n = 1; n < depth; n++){
x+=((2.0*(n&1))/n*PI)*(vol * 0.7 * sin(2.0 * PI * t * Hz * n));
}
return x;
}


