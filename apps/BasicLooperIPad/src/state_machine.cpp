//
//  state_machine.cpp
//  advanced_looper
//
//  Created by jeronimo on 2016-10-05.
//
//

#include "state_machine.h"



//constructor
State_Machine::State_Machine() {
    
    //initializes the first state
    //state = inter.get_state();
    
    //initializes the last state
    //last_state = state;
}

//destructor
State_Machine::~State_Machine() {
    
}

//--------------------------------------------------------------
void State_Machine::setup() {
    //initializing the loop
    loop.setup();
    
    //setting up buffers that will store the lic mic input
    gui.init_mic_buffer(loop.bufferSize);

}


//--------------------------------------------------------------
void State_Machine::update(Input_Interface inter) {
    
    //if there are no loops, don't even bother processing the rest
    if (loop.is_empty())
        return;
    
    //gets the input_interface current state
    state = inter.get_state();
    fingers = inter.get_fingers();
    
    //verifies if this is the first time the current state is accessed
    bool is_first_time_state_is_accessed = (last_state != state);
    
    //case-by-case analysis
    switch(state) {
        case NONE: //no fingers in the screen
            update_NONE(is_first_time_state_is_accessed);
            break;
        case ONE_FINGER: //one finger in the screen
            update_ONE_FINGER(is_first_time_state_is_accessed);
            break;
        case TWO_FINGERS: //two fingers in the screen
            update_TWO_FINGERS(is_first_time_state_is_accessed);
            break;
        case THREE_FINGERS: //three fingers in the screen
            update_THREE_FINGERS(is_first_time_state_is_accessed);
            break;
        case FOUR_FINGERS: //four fingers in the screen
            update_FOUR_FINGERS(is_first_time_state_is_accessed);
            break;
    }
    
    //updates last state
    last_state = state;
}

//--------------------------------------------------------------
void State_Machine::update_NONE(bool is_first_time_state_is_accessed) {
    if (is_first_time_state_is_accessed) {
        //updates to the full looping area
        loop.set_full_looping_area();
        
        //removes any window the gui might have
        gui.remove_window();
        
        //removes any aux area
        loop.remove_aux_looping_area();
    }
    
    //if (debug)
    //    cout << "update_NONE!" << endl;
}


//--------------------------------------------------------------
void State_Machine::update_ONE_FINGER(bool is_first_time_state_is_accessed) {
    Touch f1 = fingers[0];
    
    //computing the position in the sound
    float newx = f1.x*loop.get_size();
    
    //computing value for the volume
    float newy = (1-f1.y)*2;
    
    //new attempt
    //loop.set_head_absolute(newx);
    
    //updates the looping area
    loop.set_looping_area(newx, newx + 15*loop.bufferSize);
    
    //sets the new volume
    loop.set_volume(newy);
    
    //updates the drawing scale for the waveform
    gui.set_scale(newy);
    
    //removes any possible window
    gui.remove_window();
    
    //removes any loooping area
    loop.remove_aux_looping_area();
    
    //removes aux window
    gui.remove_aux_window();
    
    if (debug) {
        cout << "update_ONE_FINGER!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
    }
    
    
}

//--------------------------------------------------------------
void State_Machine::update_TWO_FINGERS(bool is_first_time_state_is_accessed) {
    Touch f1 = fingers[0];
    Touch f2 = fingers[1];
    
    //computing the position in the sound
    int newf1x = f1.x*loop.get_size();
    int newf2x = f2.x*loop.get_size();
    
    //float newy = (1-((f1.y+f2.y)/2))*2;
    
    //updates the looping area
    loop.set_looping_area_with_volume(newf1x, newf2x, 1-f1.y, 1-f2.y);
    
    //sets the window
    gui.set_window_with_scale(f1.x*ofGetWidth(), f2.x*ofGetWidth(), f1.y*ofGetHeight(), f2.y*ofGetHeight());
    
    //sets the volue
    //loop.set_volume(newy);
    
    //sets the scale
    //gui.set_scale(newy);
    
    //removes any loooping area
    loop.remove_aux_looping_area();
    
    //removes aux window
    gui.remove_aux_window();
    
    //organizes each one should go first and updates the head of the gui
    
    if (debug) {
        cout << "update_TWO_FINGERS!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
        cout << "           x: " << f2.x << " y: " << f2.y << endl;
    }
}

//--------------------------------------------------------------
void State_Machine::update_THREE_FINGERS(bool is_first_time_state_is_accessed) {
    //these variables are organized incrementally
    Touch f1 = fingers[0]; //window start
    Touch f2 = fingers[1]; //window end
    Touch f3 = fingers[2]; //independent voice
    
    
    //if there is already a window, update the previous variables accordinly
    if (loop.there_is_looping_area()) {
        
        /////////////////////
        // GETS WHAT FINGER IS THE BEGINING OF THE WINDOW AND SETS IT TO F1
        //getting the window
        ofPoint window = loop.get_looping_area();
        
        //getting the start position
        float start = window.x/loop.get_size();
        
        //computing the distance of each finger against the window's start
        for (int i = 0; i < fingers.size(); i++)
            fingers[i].compute_distance_to_a_point(start);
        
        //sorting using this distance
        std::sort(fingers.begin(), fingers.end(), distance_sorting);
        
        //getting the begining of the window
        f1 = fingers[0];
        
        //removing the selected from the vector
        fingers.erase(fingers.begin());
        
        /////////////////////
        // GETS WHAT FINGER IS THE END OF THE WINDOW AND SETS IT TO F2
        //getting the end position
        float end   = window.y/loop.get_size();
        
        //computing the distance of each finger against the window's end
        for (int i = 0; i < fingers.size(); i++)
            fingers[i].compute_distance_to_a_point(end);
        
        //sorting using this distance
        std::sort(fingers.begin(), fingers.end(), distance_sorting);
        
        //getting the begining of the window
        f2 = fingers[0];
        
        //removing the selected from the vector
        fingers.erase(fingers.begin());
        
        //sets the remaining as finger 3
        f3 = fingers[0];
        
    }
    
    
    //computing the position in the sound
    int newf1x = f1.x*loop.get_size();
    int newf2x = f2.x*loop.get_size();
    
    //float new12y = (1-((f1.y+f2.y)/2))*2;
    
    //updates the looping area
    loop.set_looping_area_with_volume(newf1x, newf2x, 1-f1.y, 1-f2.y);
    
    //sets the window
    gui.set_window_with_scale(f1.x*ofGetWidth(), f2.x*ofGetWidth(), f1.y*ofGetHeight(), f2.y*ofGetHeight());
    
    //sets the volume
    //loop.set_volume(new12y);
    
    
    /////////////////////
    // AUX
    int newf3x = f3.x*loop.get_size();
    
    //getting y position
    float new3y = (1-f3.y)*2;
    
    //updates the aux looping area
    loop.set_aux_looping_area(newf3x, newf3x + 15*loop.bufferSize);
    
    //sets the aux volume
    loop.set_aux_volume(new3y);
    
    //sets the scale
    gui.set_scale(new3y);
    
    //removes aux window
    gui.remove_aux_window();
    
    
    if (debug) {
        cout << "update_THREE_FINGERS!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
        cout << "           x: " << f2.x << " y: " << f2.y << endl;
        cout << "           x: " << f3.x << " y: " << f3.y << endl;
    }
}


//--------------------------------------------------------------
void State_Machine::update_FOUR_FINGERS(bool is_first_time_state_is_accessed) {
    Touch f1 = fingers[0];
    Touch f2 = fingers[1];
    Touch f3 = fingers[2];
    Touch f4 = fingers[3];
    
    
    //if there is already a window, update the previous variables accordinly
    if (loop.there_is_looping_area()) {
        
        /////////////////////
        // GETS WHAT FINGER IS THE BEGINING OF THE WINDOW AND SETS IT TO F1
        //getting the window
        ofPoint window = loop.get_looping_area();
        
        //getting the start position
        float start = window.x/loop.get_size();
        
        //computing the distance of each finger against the window's start
        for (int i = 0; i < fingers.size(); i++)
            fingers[i].compute_distance_to_a_point(start);
        
        //sorting using this distance
        std::sort(fingers.begin(), fingers.end(), distance_sorting);
        
        //getting the begining of the window
        f1 = fingers[0];
        
        //removing the selected from the vector
        fingers.erase(fingers.begin());
        
        /////////////////////
        // GETS WHAT FINGER IS THE END OF THE WINDOW AND SETS IT TO F2
        //getting the end position
        float end   = window.y/loop.get_size();
        
        //computing the distance of each finger against the window's end
        for (int i = 0; i < fingers.size(); i++)
            fingers[i].compute_distance_to_a_point(end);
        
        //sorting using this distance
        std::sort(fingers.begin(), fingers.end(), distance_sorting);
        
        //getting the begining of the window
        f2 = fingers[0];
        
        //removing the selected from the vector
        fingers.erase(fingers.begin());
        
        //sorting using this x axis
        std::sort(fingers.begin(), fingers.end(), x_sorting);
        
        //sets the remaining as finger 3 and 4
        f3 = fingers[0];
        
        //sets the remaining as finger 3
        f4 = fingers[1];
        
    }
    
    //computing the position in the sound
    int newf1x = f1.x*loop.get_size();
    int newf2x = f2.x*loop.get_size();
    
    //float newy = (1-((f1.y+f2.y)/2))*2;
    
    //updates the looping area
    //loop.set_looping_area(newf1x, newf2x);
    loop.set_looping_area_with_volume(newf1x, newf2x, 1-f1.y, 1-f2.y);
    
    //sets the window
    gui.set_window_with_scale(f1.x*ofGetWidth(), f2.x*ofGetWidth(), f1.y*ofGetHeight(), f2.y*ofGetHeight());
    
    //sets the volue
    //loop.set_volume(newy);
    
    //sets the scale
    //gui.set_scale(newy);
    
    //computing the position in the sound
    int newf3x = f3.x*loop.get_size();
    int newf4x = f4.x*loop.get_size();
    
    //float newy = (1-((f3.y+f4.y)/2))*2;
    
    //updates the looping area
    //loop.set_aux_looping_area(newf3x, newf4x);
    loop.set_aux_looping_area_with_volume(newf3x, newf4x, 1-f3.y, 1-f4.y);
    
    //sets the aux window
    gui.set_aux_window_with_scale(f3.x*ofGetWidth(), f4.x*ofGetWidth(), f3.y*ofGetHeight(), f4.y*ofGetHeight());
    
    //sets the volue
    //loop.set_aux_volume(newy);
    
    //sets the scale
    //gui.set_scale(newy);
    
    if (debug) {
        cout << "update_FOUR_FINGERS!"<< endl;
        cout << "           x: " << f1.x << " y: " << f1.y << endl;
        cout << "           x: " << f2.x << " y: " << f2.y << endl;
        cout << "           x: " << f3.x << " y: " << f3.y << endl;
        cout << "           x: " << f4.x << " y: " << f4.y << endl;
    }
}


//--------------------------------------------------------------
void State_Machine::draw() {
    gui.draw(&loop);
    //inter.draw();
}


//--------------------------------------------------------------
void State_Machine::audioIn(float * input, int bufferSize, int nChannels) {
    loop.audio_input(input);
    gui.update_mic_buffer(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void State_Machine::audioOut(float * output, int bufferSize, int nChannels) {
    loop.audio_output(output);
}

//--------------------------------------------------------------
void State_Machine::set_debug(bool debug) {
    this->debug=debug;
    gui.set_debug(debug);
    //inter.set_debug(debug);
    loop.set_debug(debug);
}

//--------------------------------------------------------------
void State_Machine::record() {
    loop.record();
}

//--------------------------------------------------------------
void State_Machine::overdub() {
    loop.overdub();
}

//--------------------------------------------------------------
void State_Machine::stop() {
    loop.stop();
}

//--------------------------------------------------------------
void State_Machine::resume() {
    loop.resume();
}

//--------------------------------------------------------------
void State_Machine::clear_loops() {
    loop.clear();
}

//--------------------------------------------------------------
bool State_Machine::is_loop_empty() {
    return loop.is_empty();
}