//
//  midiHandler.cpp
//  midiHandler
//
//  Created by jeronimo on 2017-10-13.
//
//

#include "midiHandler.h"


//--------------------------------------------------------------
void midiHandler::setup()
{
    debug = true;
    // list the number of available input & output ports
    ofxMidiIn::listPorts();
    ofxMidiOut::listPorts();
    
    // create and open input ports
    for(int i = 0; i < ofxMidiIn::getNumPorts(); ++i) {
        
        // new object
        inputs.push_back(new ofxMidiIn);
        
        // set this class to receive incoming midi events
        inputs[i]->addListener(this);
        
        // open input port via port number
        inputs[i]->openPort(i);
    }
    
    // set this class to receieve midi device (dis)connection events
    ofxMidi::setConnectionListener(this);
}

//--------------------------------------------------------------
void midiHandler::exit()
{
    for(int i = 0; i < inputs.size(); ++i) {
        inputs[i]->closePort();
        inputs[i]->removeListener(this);
        delete inputs[i];
    }
}

//--------------------------------------------------------------
void midiHandler::setDebug(bool debug)
{
    this->debug=debug;
}

//--------------------------------------------------------------
void midiHandler::drawDebug()
{
    if (debug) {
        ofSetColor(200);
        ofDrawBitmapString(ofToString("LAST MIDI: ") +
                           ofToString(lastMessage.toString())
                           , ofPoint(20, 170));
    }

}

//--------------------------------------------------------------
void midiHandler::newMidiMessage(ofxMidiMessage& msg)
{
    messageMutex.lock();
    //addMessage(msg.toString());
    //cout << msg.channel <<endl;
    //cout << msg.control <<endl;
    //cout << msg.value <<endl;
    //cout << msg.pitch <<endl;
    //cout << msg.velocity <<endl;
    lastMessage = msg;
    messageMutex.unlock();
}

//--------------------------------------------------------------
void midiHandler::midiInputAdded(string name, bool isNetwork)
{
    stringstream msg;
    msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
    //addMessage(msg.str());
    
    // create and open a new input port
    ofxMidiIn *newInput = new ofxMidiIn;
    newInput->openPort(name);
    newInput->addListener(this);
    inputs.push_back(newInput);
}

//--------------------------------------------------------------
void midiHandler::midiInputRemoved(string name, bool isNetwork)
{
    stringstream msg;
    msg << "ofxMidi: input removed: " << name << " network: " << isNetwork << endl;
    //addMessage(msg.str());
    
    // close and remove input port
    vector<ofxMidiIn*>::iterator iter;
    for(iter = inputs.begin(); iter != inputs.end(); ++iter) {
        ofxMidiIn *input = (*iter);
        if(input->getName() == name) {
            input->closePort();
            input->removeListener(this);
            delete input;
            inputs.erase(iter);
            break;
        }
    }
}