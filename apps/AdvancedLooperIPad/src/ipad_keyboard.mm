//
//  ipad_keyboard.mm
//  loopingExperiment1
//
//  Created by jeronimo on 2016-01-30.
//
//

#include "ipad_keyboard.h"


//--------------------------------------------------------------
Ipad_Keyboard::Ipad_Keyboard(bool debugMode){
    keyboard = new ofxiOSKeyboard(2,40,1,1);
    keyboard->setBgColor(255, 255, 255, 255);
    keyboard->setFontColor(0,0,0, 255);
    keyboard->setFontSize(26);
    //keyboard->setVisible(false);
    
    lastTextFieldSize = 0;
    
    debug = debugMode;
    
    if (debug)
        NSLog(@"setting up keyboard\n");
}

//--------------------------------------------------------------
Ipad_Keyboard::~Ipad_Keyboard(){
    //
}

//--------------------------------------------------------------
void Ipad_Keyboard::setDebugMode(bool debug){
    this->debug = debug;
}

//--------------------------------------------------------------
char Ipad_Keyboard::getKeyPressed(){
    keyboard->setVisible(true);
    keyboard->openKeyboard();         
    string text = keyboard->getText();
    char result = -1;
    
    //if something has changed in the input string
    if(text.size()!= lastTextFieldSize) {
        
        //gets the last added char
        char actualChar = text.c_str()[text.size()-1];
        
        if (debug)
            NSLog(@"character: %d \n", (int)actualChar);
        
        //processes the key pressed event
        result = actualChar;
    }
    
    //NSLog(@"character: %d \n", (int)text.size());
    
    lastTextFieldSize = text.size();
    
    return result;
}