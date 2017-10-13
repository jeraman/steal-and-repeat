//
//  midiHandler.h
//  midiHandler
//
//  Created by jeronimo on 2017-10-13.
//
//

#ifndef midiHandler_h
#define midiHandler_h

#include "ofxMidi.h"
#include "ofMain.h"
#include <stdio.h>

/*****************************************
 ** MIDI HANDLER *************************
 ** built form: **************************
 ** https://github.com/danomatika/ofxMidi
 *****************************************/

class midiHandler : public ofxMidiListener, public ofxMidiConnectionListener {
    
public:
    void setup();
    void exit();
    void drawDebug();
    void setDebug(bool);
    
    // midi message callback
    void newMidiMessage(ofxMidiMessage& msg);
    
    // midi device (dis)connection event callbacks
    void midiInputAdded(string name, bool isNetwork);
    void midiInputRemoved(string name, bool isNetwork);
    
    vector<ofxMidiIn*> inputs;
    ofxMidiMessage lastMessage;
    ofMutex messageMutex;
    bool    debug;
    //vector<ofxMidiOut*> outputs;
    //void midiOutputAdded(string nam, bool isNetwork);
    //void midiOutputRemoved(string name, bool isNetwork);
    //void addMessage(string msg);
    //stringstream formatString();
    //deque<string> messages;
    //int maxMessages;
    //ofMutex messageMutex; // make sure we don't read from queue while writing
    //int note, ctl;
    //vector<unsigned char> bytes;
};



#endif /* midiHandler_h */
