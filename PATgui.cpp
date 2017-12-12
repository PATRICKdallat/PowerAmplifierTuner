//
//  PATgui.cpp
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  17/08/2017
//-----------------------------------------------------------------------

#include "PATgui.h"

/*======================================================================*/
//                APPLICATION: setup | update | draw                    //
/*======================================================================*/
//-----------------------------------------------------------------------
void PATgui::setup(){
    ofSetWindowTitle("Power Amplifier Tuner");
    /* Initialisation */
    outputDeviceIndex = 1;
    inputDeviceIndex  = 0;
    
    scaledX   = (ofGetWindowWidth() - ofGetWindowHeight()/5)/4;
    scaledY   = (ofGetWindowHeight() - ofGetWindowHeight()/5)/4;
    
    colorTheme        = ofColor::green;
    colorText         = ofColor::white;
    
    leftInput         = new float[FRAMES_PER_BUFFER];
    rightInput        = new float[FRAMES_PER_BUFFER];
    
    devicesList       = soundInputStream.getDeviceList();
    
    for(int i=0; i<NUMBER_OF_WINDOWS; i++){
        for(int j=0; j<FRAMES_PER_BUFFER/2; j++){
            frequency[i][j] = 0;
        }
    }

    /* Collecting I/O devices for menus */
    for(int i = 0; i < devicesList.size(); i++ ){
        const ofSoundDevice* devInfo;
        devInfo = &devicesList.at(i);
        
        // Store devices with at least 1 input channel
        if(devInfo->inputChannels > 0){
            audioInputDeviceList.push_back(devInfo->name);
            audioInputDeviceIndex.push_back(i);
        }
        // Store devices with at least 1 output channel
        if(devInfo->outputChannels > 0){
            audioOutputDeviceList.push_back(devInfo->name);
            audioOutputDeviceIndex.push_back(i);
        }
    }
    
    /* Setup the UI elements */
    imenu = new ofxDatGuiDropdown("SELECT INPUT AUDIO DEVICE:", audioInputDeviceList);
    imenu->setStripeColor(colorTheme);
    imenu->setWidth(ofGetWindowWidth()/4);
    imenu->setPosition(0, 0);
    imenu->onDropdownEvent(this, &PATgui::onInputDeviceDropdownEvent);
    
    omenu = new ofxDatGuiDropdown("SELECT OUTPUT AUDIO DEVICE:", audioOutputDeviceList);
    omenu->setStripeColor(colorTheme);
    omenu->setWidth(ofGetWindowWidth()/4);
    omenu->setPosition(imenu->getWidth(), 0);
    omenu->onDropdownEvent(this, &PATgui::onOutputDeviceDropdownEvent);
    
    pinkToggle = new ofxDatGuiToggle("PINK NOISE");
    pinkToggle->setStripeColor(colorTheme);
    pinkToggle->setWidth(ofGetWindowWidth()/4);
    pinkToggle->setPosition((imenu->getWidth() + omenu->getWidth()), 0);
    pinkToggle->onToggleEvent(this, &PATgui::onPinkNoiseToggleEvent);
    
    analyserToggle = new ofxDatGuiToggle("AUDIO ANALYSER");
    analyserToggle->setStripeColor(colorTheme);
    analyserToggle->setWidth(ofGetWindowWidth()/4);
    analyserToggle->setPosition(ofGetWindowWidth()-analyserToggle->getWidth(), 0);
    analyserToggle->onToggleEvent(this, &PATgui::onAudioAnalyserToggleEvent);
    analyserToggle->setChecked(true);
    
    EQToggle = new ofxDatGuiToggle("EQUALISATION SCULPTOR");
    EQToggle->setStripeVisible(false);
    EQToggle->setBorder(colorTheme, 2);
    EQToggle->setBackgroundColor(ofColor::darkGreen);
    EQToggle->setWidth(ofGetWindowWidth()/4);
    EQToggle->setPosition(ofGetWindowWidth()/2, ofGetWindowHeight()-EQToggle->getHeight()*2);
    EQToggle->onToggleEvent(this, &PATgui::onEQToggleEvent);
    EQToggle->setChecked(false);
    
    createPresetButton = new ofxDatGuiButton("CREATE PRESET");
    createPresetButton->setStripeColor(colorTheme);
    createPresetButton->setBorder(colorTheme, 1);
    createPresetButton->setWidth(scaledX/2);
    createPresetButton->setPosition(ofGetWindowWidth()/2-scaledX/4, scaledY);
    createPresetButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    createPresetButton->onButtonEvent(this, &PATgui::onCreatePresetButtonEvent);
    
    errorButton = new ofxDatGuiButton("ERROR : Already a preset file with that name");
    errorButton->setStripeVisible(false);
    errorButton->setLabelUpperCase(false);
    errorButton->setBackgroundColor(ofColor::orangeRed);
    errorButton->setPosition(0, ofGetWindowHeight()-errorButton->getHeight());
    errorButton->setWidth(ofGetWindowWidth());
    errorButton->setVisible(false);
    errorButton->onButtonEvent(this, &PATgui::onErrorButtonEvent);

    presetText = new ofxDatGuiTextInput("PRESET NAME :", "PresetName");
    presetText->setStripeColor(colorTheme);
    presetText->setTextUpperCase(false);
    presetText->setInputType(ofxDatGuiInputType::ALPHA_NUMERIC);
    presetText->setWidth(scaledX,scaledX/3);
    presetText->setPosition(ofGetWindowWidth()/2-presetText->getWidth(), scaledY);

    /* Setup Input Stream */
    soundInputStream.setDeviceID(inputDeviceIndex);
    soundInputStream.setInput(this);
    soundInputStream.setup(this,
                           0,
                           NUMBER_OF_INPUTS,
                           SAMPLE_RATE,
                           FRAMES_PER_BUFFER,
                           NUMBER_OF_BUFFERS);
}

//-----------------------------------------------------------------------
void PATgui::update(){
    ofBackground(ofColor::black);

    scaledX   = (ofGetWindowWidth() - ofGetWindowHeight()/5)/4;
    scaledY   = ((ofGetWindowHeight() - ofGetWindowHeight()/5)/40)*23;
    
    /* Mouse over animation */
    imenu->setBackgroundColors(ofColor::black, (colorTheme, 100), (colorTheme,200));
    omenu->setBackgroundColors(ofColor::black, (colorTheme, 100), (colorTheme,200));
    pinkToggle->setBackgroundColors(ofColor::black, (colorTheme, 100), (colorTheme,200));
    analyserToggle->setBackgroundColors(ofColor::black, (colorTheme, 100), (colorTheme,200));
    createPresetButton->setBackgroundColors(ofColor::black, (colorTheme, 100), (colorTheme,200));
    
    /* Set Width of UI elements */
    imenu->setWidth(ofGetWindowWidth()/4);
    omenu->setWidth(ofGetWindowWidth()/4);
    pinkToggle->setWidth(ofGetWindowWidth()/4);
    analyserToggle->setWidth(ofGetWindowWidth()/4);
    EQToggle->setWidth(ofGetWindowWidth()/4);
    createPresetButton->setWidth(scaledX/2);
    errorButton->setWidth(ofGetWindowWidth());
    presetText->setWidth(scaledX,scaledX/3);
    
    /* Set Position of UI elements */
    imenu->setPosition(0, 0);
    omenu->setPosition(imenu->getWidth(), 0);
    pinkToggle->setPosition((imenu->getWidth() + omenu->getWidth()), 0);
    analyserToggle->setPosition(ofGetWindowWidth()-analyserToggle->getWidth(), 0);
    EQToggle->setPosition(ofGetWindowWidth()/2-EQToggle->getWidth()/2,
                          ofGetWindowHeight()-EQToggle->getHeight()*2.1);
    createPresetButton->setPosition(ofGetWindowWidth()/2 + scaledX/4, scaledY);
    errorButton->setPosition(0, ofGetWindowHeight()-errorButton->getHeight());
    presetText->setPosition(ofGetWindowWidth()/2-presetText->getWidth(), scaledY);
    
    /* Update the GUI components */
    imenu->update();
    omenu->update();
    pinkToggle->update();
    analyserToggle->update();
    EQToggle->update();
    createPresetButton->update();
    errorButton->update();
    presetText->update();
}

//-----------------------------------------------------------------------
void PATgui::draw(){
    /* Initialisation of Variables */
    PATfft fft;
    PATeq eq;
    
    static int index=0;
    float averagePower = 0.0f;
    float x,        y;
    float inXrange, inYrange;
    float outXrange,outYrange;
    float FSX_start  = ofGetWindowHeight()/10;
    float FSY_start  = ofGetWindowHeight()/10;
    float ECX_start  = ofGetWindowHeight()/10;
    float ECY_start  = (ofGetWindowHeight()/10)*5.5;
    float X_width  = ofGetWindowWidth()    - (FSX_start*2);
    float Y_height = ofGetWindowHeight()/2 - (FSY_start*2);
    
    /* Draw Frequency Spectrum */ {
    ofSetColor(colorText);
    ofDrawBitmapString("Frequency Spectrum", FSX_start, FSY_start-5);
        /* Relative dB scale */ {
        ofDrawBitmapString("0dB -", (FSX_start-40), FSY_start+5);
        ofDrawBitmapString("-10 -", (FSX_start-40), FSY_start+((Y_height/2)/3)+5);
        ofDrawBitmapString("-20 -", (FSX_start-40), FSY_start+(((Y_height/2)/3)*2)+5);
        ofDrawBitmapString("-30 -", (FSX_start-40), FSY_start+(Y_height/2)+5);
        ofDrawBitmapString("-40 -", (FSX_start-40), FSY_start+(((Y_height/2)/3)*4)+5);
        ofDrawBitmapString("-50 -", (FSX_start-40), FSY_start+(((Y_height/2)/3)*5)+5);
        ofDrawBitmapString("-60 -", (FSX_start-40), FSY_start+Y_height+5);
        }
        /* Frequency scale */ {
        ofDrawBitmapString("0Hz",    (FSX_start),             FSY_start + Y_height+15);
        ofDrawBitmapString("180Hz",  (FSX_start+X_width)/4+30,FSY_start + Y_height+15);
        ofDrawBitmapString("880Hz",(FSX_start+X_width)/2+25, FSY_start + Y_height+15);
        ofDrawBitmapString("4.35kHz", (FSX_start+X_width)/4*3, FSY_start + Y_height+15);
        ofDrawBitmapString("20kHz",  (FSX_start+X_width-15),  FSY_start + Y_height+15);
        }
        /* Spectrum Grid */ {
        ofSetColor(colorTheme);
        ofNoFill();
        ofDrawRectangle(FSX_start,FSY_start,X_width,Y_height);
        ofSetColor(colorTheme, 50);
        ofDrawRectangle(FSX_start,FSY_start,X_width/4,Y_height);
        ofDrawRectangle(FSX_start,FSY_start,X_width/2,Y_height);
        ofDrawRectangle(FSX_start,FSY_start,(X_width/4)*3,Y_height);
        ofDrawRectangle(FSX_start,FSY_start,X_width,Y_height/3);
        ofDrawRectangle(FSX_start,FSY_start,X_width,(Y_height/3)*2);
       }
    }
    
    /* Draw Equalisation Curve */ {
    ofSetColor(colorText);
    ofDrawBitmapString("Equalisation Curve", ECX_start, ECY_start-5);
        /* Equalisation dB Scale */ {
        ofDrawBitmapString("+12 -", (ECX_start-40), ECY_start+5);
        ofDrawBitmapString(" +9 -", (ECX_start-40), ECY_start+(Y_height/8)+5);
        ofDrawBitmapString(" +6 -", (ECX_start-40), ECY_start+(Y_height/4)+5);
        ofDrawBitmapString(" +3 -", (ECX_start-40), ECY_start+((Y_height/8)*3)+5);
        ofDrawBitmapString("0dB -", (ECX_start-40), ECY_start+(Y_height/2)+5);
        ofDrawBitmapString(" -3 -", (ECX_start-40), ECY_start+((Y_height/8)*5)+5);
        ofDrawBitmapString(" -6 -", (ECX_start-40), ECY_start+((Y_height/4)*3)+5);
        ofDrawBitmapString(" -3 -", (ECX_start-40), ECY_start+((Y_height/8)*7)+5);
        ofDrawBitmapString("-12 -", (ECX_start-40), ECY_start+Y_height+5);
        }
        /* Frequency scale */ {
        ofDrawBitmapString("0Hz",    (ECX_start),              ECY_start + Y_height+15);
        ofDrawBitmapString("180Hz",  (ECX_start+X_width)/4+30, ECY_start + Y_height+15);
        ofDrawBitmapString("880Hz",  (ECX_start+X_width)/2+25, ECY_start + Y_height+15);
        ofDrawBitmapString("4.35kHz",(ECX_start+X_width)/4*3,  ECY_start + Y_height+15);
        ofDrawBitmapString("20kHz",  (ECX_start+X_width-15),   ECY_start + Y_height+15);
        }
        /* Equalisation Grid */ {
        ofSetColor(colorTheme, 50);
        // Border Lines
        ofDrawRectangle(ECX_start,ECY_start,X_width,Y_height);
        // Vertical Grid lines
        ofDrawRectangle(ECX_start, ECY_start, X_width/4,   Y_height);
        ofDrawRectangle(ECX_start, ECY_start, X_width/2,   Y_height);
        ofDrawRectangle(ECX_start, ECY_start,(X_width/4)*3,Y_height);
        // Horizontal Grid lines
        ofDrawRectangle(ECX_start,ECY_start,X_width,Y_height/8);
        ofDrawRectangle(ECX_start,ECY_start,X_width,Y_height/4);
        ofDrawRectangle(ECX_start,ECY_start,X_width,(Y_height/8)*3);
        ofDrawRectangle(ECX_start,ECY_start,X_width,(Y_height/4)*2);
        ofDrawRectangle(ECX_start,ECY_start,X_width,(Y_height/8)*5);
        ofDrawRectangle(ECX_start,ECY_start,X_width,(Y_height/4)*3);
        ofDrawRectangle(ECX_start,ECY_start,X_width,(Y_height/8)*7);
        }
    }
    
    /* Draw UI elements */
    imenu->draw();
    omenu->draw();
    pinkToggle->draw();
    analyserToggle->draw();
    EQToggle->draw();
    createPresetButton->draw();
    errorButton->draw();
    presetText->draw();
    
    
    /* Calculating the Power Spectrum Values */
    fft.powerSpectrumTransform(leftInput,
                               &magnitude[0],
                               &phase[0],
                               &power[0],
                               &averagePower);
    
    

    /* Cycle through index for mapping frequency */
    if(index < 80){
        index += 1;
    }else{
        index = 0;
    }
    
    /* Mapping the Magnitudes to Frequency */
    for(int j=1; j<FRAMES_PER_BUFFER/2; j++) {
        frequency[index][j] = magnitude[j];
    }

    ofPushMatrix();
    /* Frequency Spectrum Plotter loop */
    ofNoFill();
    ofSetColor(colorTheme);
    ofBeginShape();
    for (int i=0; i<FRAMES_PER_BUFFER/2; i++){
        
        x = y = 0;
        inXrange  = (int)(FRAMES_PER_BUFFER/40)*11;
        inYrange  = 30;
        outXrange = X_width;
        outYrange = Y_height;
        
        /* Scaling the Frequency */
        float log = log10((float)i * inXrange / inXrange);
        float logMax = log10(inXrange);
        x = log / logMax * outXrange;
        if(x<=FSX_start){        x = FSX_start; }
        if(x>FSX_start+X_width){ x = FSX_start; }
        
        
        if (magnitude[i] == 0){
            y = FSY_start;
        }else if(magnitude[i]>Y_height + FSY_start){
            y = Y_height + FSY_start;
        }else{
            y = Y_height + FSY_start - ((magnitude[i]/inYrange) * (outYrange - FSY_start)) ;
            //y = Y_height + FSY_start - ((Y_height - FSY_start) / (X_width)) * (magnitude[i]);
        }
        
        ofVertex(x,y);
        ofDrawLine(x, Y_height+FSY_start,x, y);

        if(EQToggle->getChecked()){
            eq.eqSculptor(y, x);
        }
    }
    ofEndShape(false);
    ofPopMatrix();
    
    
    ofPushMatrix();
    /* Equalisation Curve Plotter loop */
    ofNoFill();
    ofSetColor(colorTheme);
    ofBeginShape();
    for (int i=0; i<FRAMES_PER_BUFFER/2; i++){
        
        x = y = 0;
        inXrange  = (int)(FRAMES_PER_BUFFER/40)*11;
        inYrange  = 30;
        outXrange = X_width;
        outYrange = Y_height;
        
        /* Scaling the Frequency */
        float log = log10((float)i * inXrange / inXrange);
        float logMax = log10(inXrange);
        x = log / logMax * outXrange;
        
        if(x<=ECX_start){        x = ECX_start; }
        if(x>ECX_start+X_width){ x = ECX_start; }
        
        
        if (magnitude[i] == 0){
            y = ECY_start;
        }else{
            y = Y_height/2 + ((magnitude[i]/inYrange)* outYrange) + ECY_start ;
        }
        
        ofVertex(x,y);
        ofDrawLine(x, Y_height/2+ECY_start,x, y);
        
        if(EQToggle->getChecked()){
            eq.eqSculptor(x, y);
        }
    }
    ofEndShape(false);
    ofPopMatrix();
}



/*======================================================================*/
//                              AUDIO IO                                //
/*======================================================================*/
//-----------------------------------------------------------------------
void PATgui::audioIn(float* inputBuffer,
                     int bufferSize,
                     int nChannels){
    /* Storing buffer samples in float* for FFT function use */
    for (int i=0; i<bufferSize; i++){
        leftInput[i] = inputBuffer[i*2];
        rightInput[i] = inputBuffer[i*2+1];
    }
}

//-----------------------------------------------------------------------
void PATgui::audioOut(float* outputBuffer,
                      int bufferSize,
                      int nChannels){
    
    /* Declaration */
    float p0,p1,p2,p3,p4,p5,p6;
    
    /* Pink Noise Generator */
    for(int i=0; i<bufferSize; i++){
        float pink = ofRandomf();
        
        /* applying pseudo random numbers to the pink values*/
        p0 = 0.99886 * p0 + pink * 0.0555179;
        p1 = 0.99332 * p1 + pink * 0.0750759;
        p2 = 0.96900 * p2 + pink * 0.1538520;
        p3 = 0.86650 * p3 + pink * 0.3104856;
        p4 = 0.55000 * p4 + pink * 0.5329522;
        p5 = -0.7616 * p5 - pink * 0.0168980;
        p6 = pink * 0.115926;
        
        /* writing samples */
        float sample = p0 + p1 + p2 + p3 + p4 + p5 + p6 + pink * 0.5362;
        
        /* creating samples with a volume of 1.5*/
        outputBuffer[i * nChannels    ] += sample * 0.1;
        outputBuffer[i * nChannels + 1] += sample * 0.1;
    }
}



/*======================================================================*/
//                      GUI COMPONENTS FUNCTIONS                        //
/*======================================================================*/
//-----------------------------------------------------------------------
void PATgui::onInputDeviceDropdownEvent(ofxDatGuiDropdownEvent e){
    inputDeviceIndex = audioInputDeviceIndex.at(imenu->getSelected()->getIndex());
    if(analyserToggle->getChecked()){
        soundInputStream.stop();
        soundInputStream.close();
        soundInputStream.setDeviceID(inputDeviceIndex);
        soundInputStream.setInput(this);
        soundInputStream.setup(this,
                               0,
                               NUMBER_OF_INPUTS,
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               NUMBER_OF_BUFFERS);
    }
}

//-----------------------------------------------------------------------
void PATgui::onOutputDeviceDropdownEvent(ofxDatGuiDropdownEvent e){
    outputDeviceIndex = audioOutputDeviceIndex.at(omenu->getSelected()->getIndex());
    if(pinkToggle->getChecked()){
        soundOutputStream.stop();
        soundOutputStream.close();
        soundOutputStream.setDeviceID(outputDeviceIndex);
        soundOutputStream.setOutput(this);
        soundOutputStream.setup(this,
                                NUMBER_OF_OUTPUTS,
                                0,
                                SAMPLE_RATE,
                                FRAMES_PER_BUFFER,
                                NUMBER_OF_BUFFERS);
    }
}

//-----------------------------------------------------------------------
void PATgui::onPinkNoiseToggleEvent(ofxDatGuiToggleEvent e){
    
    /* Toggle on will sound Pink Noise Stream*/
    if(pinkToggle->getChecked()){
        soundOutputStream.setDeviceID(outputDeviceIndex);
        soundOutputStream.setOutput(this);
        soundOutputStream.setup(this,
                                NUMBER_OF_OUTPUTS,
                                0,
                                SAMPLE_RATE,
                                FRAMES_PER_BUFFER,
                                NUMBER_OF_BUFFERS);
        
    /* Toggle off will silence Pink Noise Stream */
    }else{
        soundOutputStream.stop();
        soundOutputStream.close();
    }
}

//-----------------------------------------------------------------------
void PATgui::onAudioAnalyserToggleEvent(ofxDatGuiToggleEvent e){
    /* Toggle on will start new Input Stream*/
    if(analyserToggle->getChecked()){
        soundInputStream.setDeviceID(inputDeviceIndex);
        soundInputStream.setInput(this);
        soundInputStream.setup(this,
                               0,
                               NUMBER_OF_INPUTS,
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               NUMBER_OF_BUFFERS);
        
        /* Toggle off will stop + close Input Stream*/
    }else{
        soundInputStream.stop();
        soundInputStream.close();
    }
}

//-----------------------------------------------------------------------
void PATgui::onEQToggleEvent(ofxDatGuiToggleEvent e){
    if(EQToggle->getChecked()){
        if(!analyserToggle->getChecked()){
            analyserToggle->setChecked(true);
            onAudioAnalyserToggleEvent(analyserToggle);
        }
        if(!pinkToggle->getChecked()){
            pinkToggle->setChecked(true);
            onPinkNoiseToggleEvent(pinkToggle);
        }
    }else{
        if(analyserToggle->getChecked()){
            analyserToggle->setChecked(false);
            onAudioAnalyserToggleEvent(analyserToggle);
        }
        if(pinkToggle->getChecked()){
            pinkToggle->setChecked(false);
            onPinkNoiseToggleEvent(pinkToggle);
        }
    }
}

//-----------------------------------------------------------------------
void PATgui::onCreatePresetButtonEvent(ofxDatGuiButtonEvent e){
    PATfile file;
    
    /* Checking for duplicates */
    file.checkForDuplicates(presetText->getText());
}

//-----------------------------------------------------------------------
void PATgui::onErrorButtonEvent(ofxDatGuiButtonEvent e){
    if(errorButton->getVisible()){
        errorButton->setVisible(false);
        errorButton->setBackgroundColor(ofColor::orangeRed);
    }
}

//-----------------------------------------------------------------------
void PATgui::setErrorMessage(string error){
    errorString = error;
}
