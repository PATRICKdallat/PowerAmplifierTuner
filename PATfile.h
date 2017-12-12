//
//  PATfile.h
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  03/09/2017
//-----------------------------------------------------------------------

#ifndef PATfile_h
#define PATfile_h

#include "ofMain.h"
#include "PATeq.h"
#include "PATgui.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


//-----------------------------------------------------------------------
class PATfile{
  
public:    
    int  checkForDuplicates( string presetName);
    
};
#endif
