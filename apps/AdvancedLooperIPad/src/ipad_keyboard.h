//
//  ipad_keyboard.h
//  loopingExperiment1
//
//  Created by jeronimo on 2016-01-30.
//
//

#ifndef ipad_keyboard_h
#define ipad_keyboard_h


#pragma once

#include "ofxiOS.h"
#include "ofxiOSKeyboard.h"

class Ipad_Keyboard {
    
public:
    
     Ipad_Keyboard(bool);//constructor
    ~Ipad_Keyboard();//destructor
    
    //functions
    char getKeyPressed();
    void setDebug(bool);
    void hide();
    void show();
    
    //variables
    ofxiOSKeyboard* keyboard;
    int             lastTextFieldSize;
    bool            debug;
};

#endif /* ipad_keyboard_h */
