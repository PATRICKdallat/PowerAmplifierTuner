//
//  PATmain.cpp
//  PowerAmplifierTuner
//
//  Created by Patrick Dallat
//  17/08/2017
//-----------------------------------------------------------------------

#include "ofMain.h"
#include "PATgui.h"

/*======================================================================*/
//                            MAIN FUNCTION                             //
/*======================================================================*/
//-----------------------------------------------------------------------
int main(){
    ofSetupOpenGL(1000,750, OF_WINDOW);
    ofRunApp(new PATgui());
}
