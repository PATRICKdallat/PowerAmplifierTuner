#include "PATfft.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/*======================================================================*/
//                     CONSTRUCTOR and DESTRUCTOR                       //
/*======================================================================*/

//-----------------------------------------------------------------------
PATfft::PATfft() {}
//-----------------------------------------------------------------------
PATfft::~PATfft() {}



/*======================================================================*/
//                      FFT VARIABLE CALCULATIONS                       //
/*======================================================================*/

int** gFFTBitTable = NULL;
const int maxFastBits = 16;

//-----------------------------------------------------------------------
int isPowerOfTwo(int x){
    
    if(x<2){ return false; }

    if(x & (x-1)){ return false; }
    
    return true;
}

//-----------------------------------------------------------------------
int numberOfBitsNeeded(int PowerOfTwo){
    
    if (PowerOfTwo < 2) {
        fprintf(stderr, "Error: FFT called with size %d\n", PowerOfTwo);
        exit(1);
    }
    
    for (int i = 0;; i++){
        if (PowerOfTwo & (1 << i)){
            return i;
        }
    }
}

//-----------------------------------------------------------------------
int reverseBits(int index,
                int numberOfBits){
    
    int reverse;
    
    for (int i = reverse = 0; i < numberOfBits; i++) {
        reverse = (reverse << 1) | (index & 1);
        index >>= 1;
    }
    
    return reverse;
}



/*======================================================================*/
//                          FFT INITIALIZER                             //
/*======================================================================*/

//-----------------------------------------------------------------------
void initializeFFT(){
    
    gFFTBitTable = new int *[maxFastBits];
    
    int len = 2;
    for(int b=1; b<=maxFastBits; b++) {
        
        gFFTBitTable[b - 1] = new int[len];
        
        for (int i=0; i<len; i++){
            gFFTBitTable[b - 1][i] = reverseBits(i, b);
        }
        
        len <<= 1;
    }
}



/*======================================================================*/
//                          FFT CALCULATIONS                            //
/*======================================================================*/
//-----------------------------------------------------------------------
inline int fastReverseBits(int i,
                           int numberOfBits){
    
    if(numberOfBits <= maxFastBits){
        return gFFTBitTable[numberOfBits - 1][i];
    }else{
        return reverseBits(i, numberOfBits);
    }
}

//-----------------------------------------------------------------------
void fFT(int numberOfSamples,
         bool inverseTransform,
         float* realInputPart,
         float* imagInputPart,
         float* realOutputPart,
         float* imagOutputPart){
    
    int numberOfBits;
    int i, j, k, n;
    int BlockSize, BlockEnd;
    
    double angle_numerator = 2.0 * M_PI;
    float tr, ti;                /* temp real, temp imaginary */
    
    if(!isPowerOfTwo(numberOfSamples)){
        fprintf(stderr, "%d is not a power of two\n", numberOfSamples);
        exit(1);
    }
    
    if(!gFFTBitTable){
        initializeFFT();
    }
    
    if(inverseTransform){
        angle_numerator = -angle_numerator;
    }
    
    numberOfBits = numberOfBitsNeeded(numberOfSamples);
    
    /*
     **   Do simultaneous data copy and bit-reversal ordering into outputs...
     */
    
    for(int i=0; i<numberOfSamples; i++){
        j = fastReverseBits(i, numberOfBits);
        realOutputPart[j] = realInputPart[i];
        imagOutputPart[j] = (imagInputPart == NULL) ? 0.0 : imagInputPart[i];
    }
    
    /*
     **   Do the FFT itself...
     */
    
    BlockEnd = 1;
    for(BlockSize=2; BlockSize<=numberOfSamples; BlockSize<<=1) {
        
        double delta_angle = angle_numerator / (double) BlockSize;
        
        float sm2 = sin(-2 * delta_angle);
        float sm1 = sin(-delta_angle);
        float cm2 = cos(-2 * delta_angle);
        float cm1 = cos(-delta_angle);
        float w = 2 * cm1;
        float ar0, ar1, ar2, ai0, ai1, ai2;
        
        for(int i=0; i<numberOfSamples; i+=BlockSize) {
            ar2 = cm2;
            ar1 = cm1;
            
            ai2 = sm2;
            ai1 = sm1;
            
            for(int j=i, n=0; n<BlockEnd; j++, n++) {
                ar0 = w * ar1 - ar2;
                ar2 = ar1;
                ar1 = ar0;
                
                ai0 = w * ai1 - ai2;
                ai2 = ai1;
                ai1 = ai0;
                
                k = j + BlockEnd;
                tr = ar0 * realOutputPart[k] - ai0 * imagOutputPart[k];
                ti = ar0 * imagOutputPart[k] + ai0 * realOutputPart[k];
                
                realOutputPart[k] = realOutputPart[j] - tr;
                imagOutputPart[k] = imagOutputPart[j] - ti;
                
                realOutputPart[j] += tr;
                imagOutputPart[j] += ti;
            }
        }
        
        BlockEnd = BlockSize;
    }
    
    /*
     **   Need to normalize if inverse transform...
     */
    
    if(inverseTransform){
        float denom = (float) numberOfSamples;
        
        for(int i=0; i<numberOfSamples; i++){
            realOutputPart[i] /= denom;
            imagOutputPart[i] /= denom;
        }
    }
}

//-----------------------------------------------------------------------
void realFFT(int numberOfSamples,
             float* realInputPart,
             float* realOutputPart,
             float* imagOutputPart){
    
    int half = numberOfSamples / 2;
    int i;
    
    float theta = M_PI / half;
    
    float* tmpReal = new float[half];
    float* tmpImag = new float[half];
    
    for (i = 0; i < half; i++) {
        tmpReal[i] = realInputPart[2 * i];
        tmpImag[i] = realInputPart[2 * i + 1];
    }
    
    fFT(half, 0, tmpReal, tmpImag, realOutputPart, imagOutputPart);
    
    float wtemp = float (sin(0.5 * theta));
    
    float wpr = -2.0 * wtemp * wtemp;
    float wpi = float (sin(theta));
    float wr = 1.0 + wpr;
    float wi = wpi;
    
    int i3;
    
    float h1r, h1i, h2r, h2i;
    
    for (i = 1; i < half / 2; i++) {
        
        i3 = half - i;
        
        h1r = 0.5  * (realOutputPart[i] + realOutputPart[i3]);
        h1i = 0.5  * (imagOutputPart[i] - imagOutputPart[i3]);
        h2r = 0.5  * (imagOutputPart[i] + imagOutputPart[i3]);
        h2i = -0.5 * (realOutputPart[i] - realOutputPart[i3]);
        
        realOutputPart[i]  =  h1r + wr * h2r - wi * h2i;
        imagOutputPart[i]  =  h1i + wr * h2i + wi * h2r;
        realOutputPart[i3] =  h1r - wr * h2r + wi * h2i;
        imagOutputPart[i3] = -h1i + wr * h2i + wi * h2r;
        
        wr = (wtemp = wr) * wpr - wi * wpi + wr;
        wi = wi * wpr + wtemp * wpi + wi;
    }
    
    realOutputPart[0] = (h1r = realOutputPart[0]) + imagOutputPart[0];
    imagOutputPart[0] =  h1r - imagOutputPart[0];
    
    delete[]tmpReal;
    delete[]tmpImag;
}

//-----------------------------------------------------------------------
void powerSpectrum(int numberOfSamples,
                   float* input,
                   float* output){
    
    int half = numberOfSamples / 2;
    
    float theta = M_PI / half;
    
    float *tmpReal = new float[half];
    float *tmpImag = new float[half];
    float *RealOut = new float[half];
    float *ImagOut = new float[half];
    
    for (int i = 0; i < half; i++) {
        tmpReal[i] = input[2 * i];
        tmpImag[i] = input[2 * i + 1];
    }
    
    fFT(half, 0, tmpReal, tmpImag, RealOut, ImagOut);
    
    float wtemp = float (sin(0.5 * theta));
    
    float wpr = -2.0 * wtemp * wtemp;
    float wpi = float (sin(theta));
    float wr = 1.0 + wpr;
    float wi = wpi;
    
    int i3;
    
    float h1r, h1i, h2r, h2i, rt, it;
    //float total=0;
    
    for (int i = 1; i < half / 2; i++) {
        
        i3 = half - i;
        
        h1r = 0.5 * (RealOut[i] + RealOut[i3]);
        h1i = 0.5 * (ImagOut[i] - ImagOut[i3]);
        h2r = 0.5 * (ImagOut[i] + ImagOut[i3]);
        h2i = -0.5 * (RealOut[i] - RealOut[i3]);
        
        rt = h1r + wr * h2r - wi * h2i; //printf("Realout%i = %f",i,rt);total+=fabs(rt);
        it = h1i + wr * h2i + wi * h2r; // printf("  Imageout%i = %f\n",i,it);
        
        output[i] = rt * rt + it * it;
        
        rt = h1r - wr * h2r + wi * h2i;
        it = -h1i + wr * h2i + wi * h2r;
        
        output[i3] = rt * rt + it * it;
        
        wr = (wtemp = wr) * wpr - wi * wpi + wr;
        wi = wi * wpr + wtemp * wpi + wi;
    }
    
    //printf("total = %f\n",total);
    rt = (h1r = RealOut[0]) + ImagOut[0];
    it = h1r - ImagOut[0];
    output[0] = rt * rt + it * it;
    
    rt = RealOut[half / 2];
    it = ImagOut[half / 2];
    output[half / 2] = rt * rt + it * it;
    
    delete[]tmpReal;
    delete[]tmpImag;
    delete[]RealOut;
    delete[]ImagOut;
}


/*======================================================================*/
//                     'WINDOW FUNCTION' FUNCTIONS                      //
/*======================================================================*/

//-----------------------------------------------------------------------
int numberOfWindowFunctions(){
    return 4;
}

//-----------------------------------------------------------------------
char* windowFunctionName(int function){
    
    switch (function) {
        default:
        case 0:
            return "Rectangular";
        case 1:
            return "Hanning";
        case 2:
            return "Hamming";
        case 3:
            return "Bartlett";


    }
}

//-----------------------------------------------------------------------
void windowFunction(int function,
                    int numberOfSamples,
                    float* in){
    
    // Hanning
    if (function == 1) {
        for (int i = 0; i < numberOfSamples; i++){
            in[i] *= 0.50 - 0.50 * cos(2 * M_PI * i / (numberOfSamples - 1));
        }
    }
    
    // Hamming
    if (function == 2) {
        for (int i = 0; i < numberOfSamples; i++){
            in[i] *= 0.54 - 0.46 * cos(2 * M_PI * i / (numberOfSamples - 1));
        }
    }
    
    // Bartlett (triangular)
    if (function == 3) {
        for (int i = 0; i < numberOfSamples / 2; i++) {
            in[i] *= (i / (float) (numberOfSamples / 2));
            in[i + (numberOfSamples / 2)] *= (1.0 - (i / (float) (numberOfSamples / 2)));
        }
    }
    
}




/*======================================================================*/
//                     POWER SPECTRUM TRANSFORMS                        //
/*======================================================================*/
//-----------------------------------------------------------------------
void PATfft::powerSpectrumTransform(int windowFunctionSize,
                                    float* data,
                                    float* magnitude,
                                    float* phase,
                                    float* power,
                                    float* avg_power){
    int i;
    int selectedWindowFunction = 3;
    float total_power = 0.0f;
    int half = windowFunctionSize/2;
    
    /* processing variables*/
    float* inputReal      = new float[windowFunctionSize];
    float* inputImaginary = new float[windowFunctionSize];
    float* outputReal     = new float[windowFunctionSize];
    float* outputImaginary= new float[windowFunctionSize];
    
    for (i = 0; i < windowFunctionSize; i++) {
        inputReal[i] = data[i];
    }
    
    windowFunction(selectedWindowFunction, windowFunctionSize, inputReal);
    realFFT(windowFunctionSize, inputReal, outputReal, outputImaginary);
    
    for (i = 0; i < half; i++) {
        // Power Calculations
        power[i] = (outputReal[i]*outputReal[i]) + (outputImaginary[i]*outputImaginary[i]);
        total_power += power[i];
        // Magnitude and Phase Calculations
        magnitude[i] = 2.0*sqrt(power[i]);
        phase[i] = atan2(outputImaginary[i],outputReal[i]);
    }
    // Average Power Calculations
    *(avg_power) = total_power / (float) half;
    
    // Memory Management
    delete[]inputReal;
    delete[]inputImaginary;
    delete[]outputReal;
    delete[]outputImaginary;
}

//-----------------------------------------------------------------------
void PATfft::inversePowerSpectrumTransform(int windowFunctionSize,
                                           float* finalOut,
                                           float* magnitude,
                                           float* phase){
    int i;
    int selectedWindowFunction = 3;
    int half = windowFunctionSize/2;
    
    float* inputReal = new float[windowFunctionSize];
    float* inputImaginary = new float[windowFunctionSize];
    float* outputReal = new float[windowFunctionSize];
    float* outputImaginary = new float[windowFunctionSize];
    
    /* get real and imag part */
    for (i = 0; i < half; i++) {
        inputReal[i] = magnitude[i]*cos(phase[i]);
        inputImaginary[i]  = magnitude[i]*sin(phase[i]);
    }
    
    /* zero negative frequencies */
    for (i = half; i < windowFunctionSize; i++) {
        inputReal[i] = 0.0;
        inputImaginary[i] = 0.0;
    }
    
    fFT(windowFunctionSize, 1, inputReal, inputImaginary, outputReal, outputImaginary); // second parameter indicates inverse transform
    windowFunction(selectedWindowFunction, windowFunctionSize, outputReal);
				
    for (i=0; i<windowFunctionSize; i++) {
        finalOut[i] += outputReal[i];
    }
    
    delete[]inputReal;
    delete[]inputImaginary;
    delete[]outputReal;
    delete[]outputImaginary;
}
