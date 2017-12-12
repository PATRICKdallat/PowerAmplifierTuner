
#ifndef _PATfft_h_
#define _PATfft_h_

#ifndef M_PI
#define	M_PI		3.14159265358979323846  /* pi */
#endif


class PATfft {
    
public:
    
    PATfft();
    ~PATfft();
    
    void powerSpectrumTransform(float* data,
                                float* magnitude,
                                float* phase,
                                float* power,
                                float* avg_power);
    
    void inversePowerSpectrumTransform(int windowFunctionSize,
                                       float* finalOut,
                                       float* magnitude,
                                       float* phase);
};

#endif	
