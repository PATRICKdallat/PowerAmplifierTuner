//
//  PATeq.h
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  03/09/2017
//-----------------------------------------------------------------------


#ifndef PATeq_h
#define PATeq_h

#define NUMBER_OF_EQ_BANDS      (6)
#define NUMBER_OF_EQ_PARAMETERS (2)


//-----------------------------------------------------------------------
class PATeq{
    
public:
    PATeq();
    PATeq(int f1,int g1,
            int f2,int g2,
            int f3,int g3,
            int f4,int g4,
            int f5,int g5,
            int f6,int g6);
    ~PATeq();
    void setEQ1(int f, int g);
    int getEQ1freq();
    int getEQ1gain();
    
    void setEQ2(int f, int g);
    int getEQ2freq();
    int getEQ2gain();
    
    void setEQ3(int f, int g);
    int getEQ3freq();
    int getEQ3gain();
    
    void setEQ4(int f, int g);
    int getEQ4freq();
    int getEQ4gain();
    
    void setEQ5(int f, int g);
    int getEQ5freq();
    int getEQ5gain();
    
    void setEQ6(int f, int g);
    int getEQ6freq();
    int getEQ6gain();
    
    void eqSculptor(int i, float magnitude);
    
private:
    int freq1;  int gain1;
    int freq2;  int gain2;
    int freq3;  int gain3;
    int freq4;  int gain4;
    int freq5;  int gain5;
    int freq6;  int gain6;
    
};
#endif
