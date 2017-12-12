//
//  PATeq.cpp
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  03/09/2017
//-----------------------------------------------------------------------

#include "PATeq.h"
#include "PATgui.h"
#include "PATfile.h"


//-----------------------------------------------------------------------
PATeq::PATeq(){}
//-----------------------------------------------------------------------
PATeq::PATeq(int f1,int g1,
                 int f2,int g2,
                 int f3,int g3,
                 int f4,int g4,
                 int f5,int g5,
                 int f6,int g6){
    setEQ1(f1,g1);
    setEQ2(f2,g2);
    setEQ3(f3,g3);
    setEQ4(f4,g4);
    setEQ5(f5,g5);
    setEQ6(f6,g6);
}

//-----------------------------------------------------------------------
PATeq::~PATeq(){}

//-----------------------------------------------------------------------
void PATeq::setEQ1(int f, int g){
    freq1 = f;
    gain1 = g;
}
int PATeq::getEQ1freq(){
    return freq1;
}
int PATeq::getEQ1gain(){
    return gain1;
}

//-----------------------------------------------------------------------
void PATeq::setEQ2(int f, int g){
    freq2 = f;
    gain2 = g;
}
int PATeq::getEQ2freq(){
    return freq2;
}
int PATeq::getEQ2gain(){
    return gain2;
}

//-----------------------------------------------------------------------
void PATeq::setEQ3(int f, int g){
    freq3 = f;
    gain3 = g;
}
int PATeq::getEQ3freq(){
    return freq3;
}
int PATeq::getEQ3gain(){
    return gain3;
}

//-----------------------------------------------------------------------
void PATeq::setEQ4(int f, int g){
    freq4 = f;
    gain4 = g;
}
int PATeq::getEQ4freq(){
    return freq4;
}
int PATeq::getEQ4gain(){
    return gain4;
}

//-----------------------------------------------------------------------
void PATeq::setEQ5(int f, int g){
    freq5 = f;
    gain5 = g;
}
int PATeq::getEQ5freq(){
    return freq5;
}
int PATeq::getEQ5gain(){
    return gain5;
}

//-----------------------------------------------------------------------
void PATeq::setEQ6(int f, int g){
    freq6 = f;
    gain6 = g;
}
int PATeq::getEQ6freq(){
    return freq6;
}
int PATeq::getEQ6gain(){
    return gain6;
}

/*======================================================================*/
//                             EQ SCULPTOR                              //
/*======================================================================*/
//-----------------------------------------------------------------------
void PATeq::eqSculptor(int i, float magnitude){
    
    float amplitude = 300 - magnitude;
    int frequency, gain;
    
    
    /* EQ1 freq 0 - 200 */
    int f1 = 0;
    int g1 = 0;
    if(i>=0 && i<9){
        if(amplitude>1){
            
            f1 = i;
            g1 = 3;
            
            if(amplitude>2*2){
                f1 = i;
                g1 = 6;
                
                if(amplitude>3*3){
                    f1 = i;
                    g1 = 9;
                }
            }
        }
    }


    /* EQ2 freq 200 - 700 */
    int f2 = 0;
    int g2 = 0;
    if(i>=9 && i<32){
        if(amplitude>1){
            f2 = i;
            g2 = 3;
            if(amplitude>2*2){
                f2 = i;
                g2 = 6;
                if(amplitude>3*3){
                    f2 = i;
                    g2 = 9;
                }
            }
        }
    }

    
    /* EQ3 freq 700 - 1k25 */
    int f3 = 0;
    int g3 = 0;
    if(i>=32 && i<58){
        if(amplitude>1){
            f3 = i;
            g3 = 3;

            if(amplitude>2*2){
                f3 = i;
                g3 = 6;

                if(amplitude>3*3){
                    f3 = i;
                    g3 = 9;

                }
            }
        }
    }
    
    /* EQ4 freq 1k25 - 1k5 */
    int f4 = 0;
    int g4 = 0;
    if(i>=58 && i<70){
        if(amplitude>1){
            f4 = i;
            g4 = 3;

            if(amplitude>2*2){
                f4 = i;
                g4 = 6;

                if(amplitude>3*3){
                    f4 = i;
                    g4 = 9;

                }
            }
        }
    }

    
    /* EQ5 freq 1k5 - 7k */
    int f5 = 0;
    int g5 = 0;
    if(i>=70 && i<325){
        if(amplitude>1){
            f5 = i;
            g5 = 3;

            if(amplitude>2*2){
                f5 = i;
                g5 = 6;

                if(amplitude>3*3){
                    f5 = i;
                    g5 = 9;

                }
            }
        }
    }

    
    /* EQ6 freq 7k - 20k */
    int f6 = 0;
    int g6 = 0;
    if(i>=325 && i<980){
        if(amplitude>1){
            f6 = i;
            g6 = 3;

            if(amplitude>2*2){
                f6 = i;
                g6 = 6;

                if(amplitude>3*3){
                    f6 = i;
                    g6 = 9;
                }
            }
        }
    }
    PATeq(f1,g1,f2,g2,f3,g3,f4,g4,f5,g5,f6,g6);
}
