#include "ofApp.h"


bool finished_setup = false;
//--------------------------------------------------------------
void ofApp::setup(){
    
    //seting debug
    set_debug(true);
    
    //setting the framerate
    ofSetFrameRate(60);
    
    //setting the background color
    ofBackground(255);
    
    //setting the default orientation OF_ORIENTATION_90_LEFT
    ofSetOrientation(OF_ORIENTATION_90_LEFT);
    
    //init sound
    setup_sound();
    
    //seting up ipad keyboard
    ipad_keyboard = new Ipad_Keyboard(debug);
    
    //seting up state machine
    sm.setup();
    
    //print debug info
    if (debug)
        NSLog(@"end of setup\n");
    
    finished_setup = true;

}

//--------------------------------------------------------------
void ofApp::setup_sound(){
    
    //debug which devices are available in this machine
    //if (debug)
    //    soundStream.printDeviceList();
    
    
    //sets up the input and output interfaces
    //bool result = soundStream.setup(this, N_CHANNELS, N_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, 4);
    //bool result = soundStream.setup(N_CHANNELS, N_CHANNELS, this, SAMPLE_RATE, BUFFER_SIZE, 4);
    //on the ipad
    ofSoundStreamSetup(N_CHANNELS, N_CHANNELS, this, SAMPLE_RATE, BUFFER_SIZE, 4);
    
    if (debug) {
        NSLog(@"sound setup complete!");
        NSLog(@"      N_CHANNELS:  %i \n", N_CHANNELS);
        NSLog(@"      SAMPLE_RATE: %i \n", SAMPLE_RATE);
        NSLog(@"      BUFFER_SIZE: %i \n", BUFFER_SIZE);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if (!finished_setup) return;
    
    update_keys();
    input.set_fingers(touches);
    sm.update(input);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (!finished_setup) return;
    
    sm.draw();
    input.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    if (!finished_setup) return;
    
    sm.audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    if (!finished_setup) return;
    
    sm.audioOut(output, bufferSize, nChannels);
}

void ofApp::set_debug(bool debug)
{
    this->debug=debug;
    sm.set_debug(debug);
    input.set_debug(debug);
}

/**********************************************
 * functions defined for the iOS platform
 **********************************************/

bool paused = false;

void ofApp::update_keys() {
    
    //gets what text has the user typed
    char key = ipad_keyboard->getKeyPressed();
    
    //if (debug)
    //    NSLog(@"key: %c", key);

    //disabling debug condition
    if (key=='d' || key=='D')
        this->set_debug(!debug);
    
    if (key==' ') {
        if (sm.is_loop_empty())
            sm.record();
        else
            sm.overdub();
    }
    
    if (key=='-')
        sm.clear_loops();
    
    if (key=='p' || key=='P') {
        paused = !paused;
        if (paused)
            sm.stop();
        else
            sm.resume();
    }
    
    if (key=='a' || key=='A') {
        sm.set_feedback(sm.get_feedback()+0.05);
    }
    
    if (key=='s' || key=='S') {
        sm.set_feedback(sm.get_feedback()-0.05);
    }
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    //NSLog(@"touch down. id: %i x: %f y:%f total:%i\n", touch.id, touch.x, touch.y, touch.numTouches);
    
    Touch t(touch.x/ofGetWidth(), touch.y/ofGetHeight(), touch.angle, touch.pressure+1, touch.id);
    touches.push_back(t);
    input.set_number_of_fingers(touch.numTouches);
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    //NSLog(@"touch moved. id: %i x: %f y:%f total:%i\n", touch.id, touch.x, touch.y, touch.numTouches);
    
    for (std::vector<Touch>::iterator t=touches.begin(); t!=touches.end(); ++t)
        if (t->id == touch.id) {
            //touches.erase(t);
            t->x = touch.x/ofGetWidth();
            t->y = touch.y/ofGetHeight();
            t->angle = touch.angle;
            t->size = touch.pressure+1;
            break;
        }
    input.set_number_of_fingers(touch.numTouches);
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    //NSLog(@"touch up. id: %i x: %f y:%f total:%i\n", touch.id, touch.x, touch.y, touch.numTouches);
    
    for (std::vector<Touch>::iterator t=touches.begin(); t!=touches.end(); ++t)
        if (t->id == touch.id) {
            touches.erase(t);
            break;
        }
    input.set_number_of_fingers(touch.numTouches);
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

    if (debug)
        NSLog(@"device changed orientation. the input value is: %d", newOrientation);
    
    //sets the new orientation according to the value received in newOrientation
    switch(newOrientation) {
            
        case 1: //if its the normal one
            ofSetOrientation(OF_ORIENTATION_DEFAULT);
            break;
            
        case 2: //if its upside down
            ofSetOrientation(OF_ORIENTATION_180);
            break;
            
        case 3: //if its upside down
            ofSetOrientation(OF_ORIENTATION_90_LEFT);
            break;
            
        case 4: //if its upside down
            ofSetOrientation(OF_ORIENTATION_90_RIGHT);
            break;
    }
}
