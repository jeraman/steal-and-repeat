//
//  gui.cpp
//  advanced_looper
//
//  Created by jeronimo on 2016-09-15.
//
//

#include "gui.h"


Gui::Gui()
{
    debug=false;
    scale_whole_loop = 1;
    position_window1_start = -1, position_window1_end=-1;
    scale_window1_start = 1, scale_window1_end = 1;
    scale_window2_start = 1, scale_window2_end = 1;
}

Gui::Gui(bool debug)
{
    this->set_debug(debug);
}

Gui::~Gui()
{
    
}

//@TODO - update the visuals
void Gui::draw(Loop* first) {
    
    drawBackground(first->is_recording());
    drawFirstLoop(first);
    drawHead(first);
    drawAuxHead(first);
    drawMic();
    
    
    if (position_window1_start != -1 && position_window1_end != -1)
        drawWindow(first->is_recording());
    
    if (debug) {
        ofSetColor(200);
        string fps("FPS: ");
        ofDrawBitmapString(fps + ofToString(ofGetFrameRate()), 20, 20);
    }
}

//draws the background
void Gui::drawBackground(bool is_recording) {
    
    //draws the background
    ofBackground(255, 150);
    
    //if it's recording and there is no windows
    if (is_recording && position_window1_start == -1)
        
        //draws the window
        ofBackground(255, 100, 100, 150);
}


//draws the first loop
void Gui::drawFirstLoop(Loop* first)
{
    //checks if there is first. continues to execute if there is
    if (first->is_empty()) {
        return;
    }
    
    //start drawing first waveform
    ofSetColor(30);
    ofSetLineWidth(4);
    
    float posy = ofGetHeight()/2.0;
    
    int loopsize = first->sample.size();
    
    //iterates over the screenpixels
    for (unsigned int i = 0; i < ofGetWidth(); i++){
        //gets the correspond position of the index of the loop and the screen width
        //float posx = ofMap(i, 0, loopsize, 0, ofGetWidth());
        
        //mapping the screen width to the position in the array
        int convWidthToSamples = (int)((i/(float)ofGetWidth())*loopsize);
        
        //drawing the corresponding rectangle
        //float sizey = ofMap(first->sample[convWidthToSamples], -1, 1, -posy, posy);
        float sizey = 4*ofMap(abs(first->sample[convWidthToSamples]), -1, 1, -posy, posy);
        float final_scale;
        
        final_scale = sizey*scale_whole_loop;
        
        //scaling according to the window
        if (there_is_an_window()) {
        
            //if the header is inside the window area
            if (i > position_window1_start && i < position_window1_end) {
                
                //computing the normalized head pos
                float normalized_head_pos = (i-position_window1_start)/(float)(position_window1_end-position_window1_start);
                
                //computing the current scale
                float current_scale = scale_window1_start*(1-normalized_head_pos) + scale_window1_end*(normalized_head_pos);
                
                //normalizing
                current_scale = (1-(current_scale/ ofGetHeight()))*2;
                
                //applying the scale to this excerpt
                final_scale = sizey * current_scale;
            }
        }
        
        //scaling according to the aux window
        if (there_is_an_aux_window) {
            
            //if the header is inside the window area
            if (i > position_window2_start && i < position_window2_end) {
                
                //computing the normalized head pos
                float normalized_head_pos = (i-position_window2_start)/(float)(position_window2_end-position_window2_start);
                
                //computing the current scale
                float current_scale = scale_window2_start*(1-normalized_head_pos) + scale_window2_end*(normalized_head_pos);
                
                //normalizing
                current_scale = (1-(current_scale/ ofGetHeight()))*2;
                
                //applying the scale to this excerpt
                final_scale = sizey * current_scale;
            }
        }
        
        ofDrawRectangle(i,posy,1,final_scale);
        ofDrawRectangle(i,posy,1,-final_scale);
        
    }
}


void Gui::drawHead(Loop* first) {
    
    //getting the first loop if available
    //Loop* first = lm.get_loop(' ');
    
    //checks if there is first. continues to execute if there is
    if (first == nullptr)
        return;
    
    //stores the loop size
    int loopsize = first->sample.size();
    
    //start drawing first waveform
    ofSetColor(100);
    ofSetLineWidth(20);
    
    //gets the correspond position of the index of the loop and the screen width
    float posx = ofMap(first->outpos, 0, loopsize, 0, ofGetWidth());
    //computing the y size of each rectangle
    
    ofDrawLine(posx,0,posx,ofGetHeight());
    
}

void Gui::drawAuxHead(Loop* first) {
    
    //getting the first loop if available
    //Loop* first = lm.get_loop(' ');
    
    //checks if there is first. continues to execute if there is
    if (!first->there_is_aux_looping_area())
        return;
    
    //stores the loop size
    int loopsize = first->sample.size();
    
    //start drawing first waveform
    ofSetColor(100);
    ofSetLineWidth(20);
    
    //gets the correspond position of the index of the loop and the screen width
    float posx = ofMap(first->aux_outpos, 0, loopsize, 0, ofGetWidth());
    //computing the y size of each rectangle
    
    ofDrawLine(posx,0,posx,ofGetHeight());
    
}



void Gui:: drawWindow(bool is_recording) {
    //sets color
    ofSetColor(150, 100);
    
    //if there is an aux window
    if (there_is_an_aux_window) {
        
        //who is the first to be drawn? in case its window1...
        if (position_window1_start < position_window2_start) {
        
            //drawing first overlay
            ofDrawRectangle(0, 0, position_window1_start, ofGetHeight());
        
            //drawing second overlay
            ofDrawRectangle(position_window1_end, 0, position_window2_start-position_window1_end, ofGetHeight());
        
            //drawing third overlay
            ofDrawRectangle(position_window2_end, 0, ofGetWidth()-position_window2_end, ofGetHeight());
        
        //in case it's window 2...x
        } else {
            
            //drawing first overlay
            ofDrawRectangle(0, 0, position_window2_start, ofGetHeight());
            
            //drawing second overlay
            ofDrawRectangle(position_window2_end, 0, position_window1_start-position_window2_end, ofGetHeight());
            
            //drawing third overlay
            ofDrawRectangle(position_window1_end, 0, ofGetWidth()-position_window1_end, ofGetHeight());
        }
        
    //if there is no aux window
    } else {
    
        //drawing first overlay
        ofDrawRectangle(0, 0, position_window1_start, ofGetHeight());
    
        //drawing second overlay
        ofDrawRectangle(position_window1_end, 0, ofGetWidth()-position_window1_end, ofGetHeight());
    }
    
    
    //if it's recording, draw the recording area
    if (is_recording){
        
        //sets the red color
        ofSetColor(255, 100, 100, 150);
        
        //draw the first recording area
        ofDrawRectangle(position_window1_start, 0, position_window1_end-position_window1_start, ofGetHeight());
        
        if (there_is_an_aux_window)
            
            //draw the first recording area
            ofDrawRectangle(position_window2_start, 0, position_window2_end-position_window2_start, ofGetHeight());
    }
}

// draw the left channel:
void Gui::drawMic() {
    
    ofPushStyle();
    ofPushMatrix();
    
    ofSetColor(200, 0, 0);
    ofSetLineWidth(3);
    
    ofBeginShape();
    
    for (unsigned int i = 0; i < leftMic.size(); i++){
        float average=leftMic[i];
        if (N_CHANNELS==2) //if there are two channels
            average=(leftMic[i]+rightMic[i])/2;
        float xpos= ((i/(float)leftMic.size())*ofGetWidth());
        float ypos=((ofGetHeight()/2) - average*1000.0f);
        
        ofVertex(xpos, ypos);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
}


//--------------------------------------------------------------
void Gui::init_mic_buffer(int bufferSize) {
    
    //for (int i = 0; i < bufferSize; i++)
    //    leftMic.push_back(0);
    
    leftMic.assign(bufferSize, 0.0);
    
    if (N_CHANNELS==2) //if there are two channels
        rightMic.assign(bufferSize, 0.0);
    
}


//--------------------------------------------------------------
void Gui::update_mic_buffer(float * input, int bufferSize, int nChannels) {
    
    for (int i = 0; i < bufferSize; i++){
        leftMic[i]	= input[i*nChannels];
        if (nChannels==2) //if there are two channels
            rightMic[i]	= input[(i*nChannels)+1]*0.5;
    }
    
}

bool Gui::there_is_an_window() {
    return (position_window1_start != -1 && position_window1_end!=-1);
}

void Gui::set_window_with_scale(int x1, int x2, int y1, int y2)
{
    position_window1_start = x1;
    position_window1_end=x2;
    scale_window1_start = y1;
    scale_window1_end = y2;
    
}

void Gui::set_aux_window_with_scale(int x1, int x2, int y1, int y2)
{
    position_window2_start = x1;
    position_window2_end=x2;
    scale_window2_start = y1;
    scale_window2_end = y2;
    there_is_an_aux_window = true;
}

void Gui::remove_window()
{
    position_window1_start = -1;
    position_window1_end=-1;
    scale_window1_start = 1;
    scale_window1_end = 1  ;
}

void Gui::remove_aux_window()
{
    position_window2_start = -1;
    position_window2_end=-1;
    scale_window2_start = 1;
    scale_window2_end = 1;
    there_is_an_aux_window = false;
}


void Gui::set_debug(bool debug)
{
    this->debug=debug;
}

/*
void Gui::set_head_offset (int head_offset)
{
    this->head_offset=head_offset;
}

int Gui::get_head_offset ()
{
    return this->head_offset;
}*/

void Gui::set_scale (float scale)
{
    this->scale_whole_loop=scale;
}