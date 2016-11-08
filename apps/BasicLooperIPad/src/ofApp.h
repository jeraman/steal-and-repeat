#pragma once

#include "ofxiOS.h"
#include "ipad_keyboard.h"
#include "input_interface.h"
#include "state_machine.h"

class ofApp : public ofxiOSApp {
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
    
        void update_keys();
        void setup_sound();
        void set_debug(bool);
    
        void audioIn(float * input, int bufferSize, int nChannels);
        void audioOut(float * input, int bufferSize, int nChannels);
    
    private:
        Ipad_Keyboard*  ipad_keyboard;
        Input_Interface input;
        State_Machine   sm;
        ofSoundStream   soundStream;
        bool            debug;
        vector<Touch>   touches;

};


