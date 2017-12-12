//
//  PATgui.h
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  17/08/2017
//-----------------------------------------------------------------------

#ifndef _PATgui_h_
#define _PATgui_h_

#include "ofMain.h"
#include "PATfft.h"
#include "PATfile.h"
#include "PATeq.h"
#include "ofxDatGui.h"
#include <vector>
#include <math.h>


#define NUMBER_OF_WINDOWS       (80)
#define NUMBER_OF_OUTPUTS       (2)
#define NUMBER_OF_INPUTS        (2)
#define SAMPLE_RATE             (44100)
#define FRAMES_PER_BUFFER       (2048)
#define NUMBER_OF_BUFFERS       (4)


//-----------------------------------------------------------------------
class PATgui : public ofSimpleApp{
    string errorString;

public:
    int  inputDeviceIndex;
    int  outputDeviceIndex;
    
    void setup();
    void update();
    void draw();
    
    void audioIn( float* inputBuffer,  int bufferSize, int nChannels);
    void audioOut(float* outputBuffer, int bufferSize, int nChannels);
        
    void onInputDeviceDropdownEvent( ofxDatGuiDropdownEvent  e);
    void onOutputDeviceDropdownEvent(ofxDatGuiDropdownEvent  e);
    void onAudioAnalyserToggleEvent( ofxDatGuiToggleEvent    e);
    void onPinkNoiseToggleEvent(     ofxDatGuiToggleEvent    e);
    void onEQToggleEvent(            ofxDatGuiToggleEvent    e);
    void onCreatePresetButtonEvent(  ofxDatGuiButtonEvent    e);
    void onErrorButtonEvent(         ofxDatGuiButtonEvent    e);
    
    void setErrorMessage(string error);

private:
    
    float* leftInput;
    float* rightInput;
    
    float  scaledX;
    float  scaledY;
    
    float  magnitude[FRAMES_PER_BUFFER];
    float  phase[FRAMES_PER_BUFFER];
    float  power[FRAMES_PER_BUFFER];
    float  frequency[NUMBER_OF_WINDOWS][FRAMES_PER_BUFFER/2];
    float  freq_phas[NUMBER_OF_WINDOWS][FRAMES_PER_BUFFER/2];
    
    vector<int>           audioInputDeviceIndex;
    vector<int>           audioOutputDeviceIndex;
    vector<string>        audioInputDeviceList;
    vector<string>        audioOutputDeviceList;
    vector<ofSoundDevice> devicesList;

    ofxDatGuiDropdown*    imenu;
    ofxDatGuiDropdown*    omenu;
    ofxDatGuiToggle*      pinkToggle;
    ofxDatGuiToggle*      analyserToggle;
    ofxDatGuiToggle*      EQToggle;
    ofxDatGuiButton*      createPresetButton;
    ofxDatGuiButton*      errorButton;
    ofxDatGuiTextInput*   presetText;
    
    ofSoundStream         soundInputStream;
    ofSoundStream         soundOutputStream;
    ofSoundDevice         soundDevice;
    
    ofColor               colorTheme;
    ofColor               colorText;

};

#endif

