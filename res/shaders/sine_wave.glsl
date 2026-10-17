
#define MAX_INT 2147483647
#define PI 3.141592653589793238462643383279502884197169399375105820974944592


float white(uint samp,float vol){
    uint s=((samp)<<13)^(samp);
    s=s*(s*s*15731+789221)+1376312589;
    return (float(s&0x7fffffff)/1073741824.0*2.0-1.0)*vol;
}
float sine_wave(uint samp, float Hz, float vol){
    return (vol*audio_sin(PI*Hz*samp/48000.0));
}
float triangle(uint samp, float Hz, float vol, int depth=10){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((audio_sin(0.5f*(PI-((n<<1)-1)*PI*x))/(((n<<1)-1)*((n<<1)-1)))*vol*audio_sin(n*PI*Hz*samp/48000.0));
    }
}
float ramp_up(uint samp, float Hz, float vol, int depth=80){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((1.0-2.0*((n+1)&1))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
return x;
}
float ramp_down(uint samp, float Hz, float vol, int depth = 80){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((-1*(1.0-2.0*(n&1)))/(n))*(vol * audio_sin(n*PI*Hz*samp/48000.0));
    }
    return x;
}
float square(uint samp, float Hz, float vol, int depth = 200){
    float x=0.0;
    for (int n = 1; n < depth; n++){
        x+=((2.0*(n&1))/n*PI)*(vol * 0.7 * audio_sin(n*PI*Hz*samp/48000.0));
    }
return x;
}
float Tramp_down(uint samp, float Hz, float vol){
    float p=96000.0/Hz;
    return(vol*(mod((2*samp/p+h),2)*-1+1));
}
float Tramp_up(uint samp, float Hz, float vol){
    float p=96000.0/Hz;
    return(vol*(mod((2*samp/p),2)+1));
}
float Tsquare(uint samp, float Hz, float vol){
    return(vol*(2.0*(uint(samp/(96000.0/Hz))&1u)-1.0));
}
float Ttriangle(uint samp, float Hz, float vol){
    return(vol*(1.0-2.0*abs(2.0*fract(samp/(96000.0/Hz))-1.0)));
}


