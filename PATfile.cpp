//
//  PATfile.cpp
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  03/09/2017
//-----------------------------------------------------------------------

#include "PATfile.h"

/*======================================================================*/
//                         WRITE PRESET FILE                            //
/*======================================================================*/

//-----------------------------------------------------------------------
void writePresetFile(string presetName){
    /* Variable Declaration */
    ofstream fileCreate;
    string datecreated;
    string timecreated;
    PATeq  eq;
    
    /* Retrieving system time and date clock */
    time_t dateAndTime = time(0);
    struct tm* currentDateAndTime = localtime(&dateAndTime);
    
    /* Creating Date and Time string variables */
    datecreated = to_string(currentDateAndTime->tm_mday) + "/" +
                  to_string(currentDateAndTime->tm_mon + 1) + "/" +
                  to_string(currentDateAndTime->tm_year + 1900);
    
    timecreated = to_string(currentDateAndTime->tm_hour) +":"+
                  to_string(currentDateAndTime->tm_min);
    
    int freq1 = eq.getEQ1freq();  int gain1 = eq.getEQ1gain();
    int freq2 = eq.getEQ2freq();  int gain2 = eq.getEQ2gain();
    int freq3 = eq.getEQ3freq();  int gain3 = eq.getEQ3gain();
    int freq4 = eq.getEQ4freq();  int gain4 = eq.getEQ4gain();
    int freq5 = eq.getEQ5freq();  int gain5 = eq.getEQ5gain();
    int freq6 = eq.getEQ6freq();  int gain6 = eq.getEQ6gain();
    
    /* Dummy values */
    freq1 = 200;    gain1 = -6;
    freq2 = 400;    gain2 = -3;
    freq3 = 900;    gain3 = -3;
    freq4 = 1250;   gain4 = -3;
    freq5 = 5000;   gain5 = 0;
    freq6 = 10000;  gain6 = 2;


    /* Creating the preset file in preset location */
    fileCreate.open("/Users/PD/Desktop/PAT\ Presets/"+presetName+".chn",ios::out);
    
    
    if(fileCreate.is_open()){
        
        /* Writing the preset code */
        fileCreate << "Preset Name:  " << presetName << endl;
        fileCreate << "Date Created: " << datecreated << endl;
        fileCreate << "Time Created: " << timecreated << endl << endl;
        
        fileCreate << "/eq ON" << endl;
        fileCreate << "/eq/1 LShv "+to_string(freq1)+" "+to_string(gain1)+".00 2.0 " << endl;
        fileCreate << "/eq/2 PEQ " +to_string(freq2)+" "+to_string(gain2)+".00 2.0 " << endl;
        fileCreate << "/eq/3 PEQ " +to_string(freq3)+" "+to_string(gain3)+".00 2.0 " << endl;
        fileCreate << "/eq/4 PEQ " +to_string(freq4)+" "+to_string(gain4)+".00 2.0 " << endl;
        fileCreate << "/eq/5 PEQ " +to_string(freq5)+" "+to_string(gain5)+".00 2.0 " << endl;
        fileCreate << "/eq/6 HShv "+to_string(freq6)+" "+to_string(gain6)+".00 2.0 " << endl;
        
        /* Closing file stream operations */
        fileCreate.close();
 
        /* Using command line we open the location of the preset */
        string command = "open /Users/PD/Desktop/'PAT Presets'/";
        system(command.c_str());
    }
}

//-----------------------------------------------------------------------
int PATfile::checkForDuplicates(string presetName){
    ifstream fileCheck;
    
    PATgui gui;
    
    /* Check for Preset Folder */
    string command = "cd /Users/PD/Desktop/; mkdir 'PAT Presets';";
    if(system(command.c_str())){
        gui.setErrorMessage("ERROR::PATfile : Already a folder under the name : 'PAT Presets'");
    }
    
    /* Check for Preset File */
    fileCheck.open("/Users/PD/Desktop/PAT\ Presets/"+presetName+".chn",ios::in);
    if(fileCheck.is_open()){
        gui.setErrorMessage("ERROR::PATfile : Already a preset file under the name : "+presetName);
        return 1;
    }else{
        writePresetFile(presetName);
        gui.setErrorMessage("SUCCESS::PATfile : "+presetName+".chn created into PAT Presets");
        return 0;
    }
}


