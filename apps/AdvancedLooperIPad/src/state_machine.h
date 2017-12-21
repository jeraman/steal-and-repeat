//
//  state_machine.hpp
//  advanced_looper
//
//  Created by jeronimo on 2016-10-05.
//
//

#ifndef state_machine_hpp
#define state_machine_hpp

#include <stdio.h>
#include "loop.h"
#include "input_interface.h"
#include "state_machine.h"
#include "gui.h"
#include "midiHandler.h"


//state machine for controlling the looper
typedef enum {
    EMPTY,
    RECORD,
    PLAY,
    OVERDUB,
    STOP
} LooperControlState;

/********************************
 Handles the main software control
 * two state machines are used:
 *  1 - touch control (how many fingers ther are on the screen?)
 *  2 - looper control (for simulating Boss RC-1's functionalities
 ********************************/

class State_Machine
{
    
public:
     State_Machine();     //constructor
    ~State_Machine();     //desctructor
    
    void setup();
    void update(Input_Interface);
    void draw();
    
    void set_debug(bool);
    void update_NONE(bool);
    void update_ONE_FINGER(bool);
    void update_TWO_FINGERS(bool);
    void update_THREE_FINGERS(bool);
    void update_FOUR_FINGERS(bool);
    
    
    void process_MIDI_events(ofxMidiMessage);
    void update_pressed_once();
    void update_pressed_twice();
    void update_press_and_hold();
    
    void record();
    void stop_record();
    void overdub();
    void stop_overdub();
    void stop();
    void resume();
    void clear_loops();
    void undo_redo();
    void cancel_recording_or_overdubing();
    void stop_if_feedback_has_cleaned_audio();
    bool is_loop_empty_in_data();
    void set_feedback(float);
    void set_delay(float);
    float get_feedback();
    float get_delay();
    void check_if_volume_has_changed_to_one_and_update_gui();
    
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut(float * input, int bufferSize, int nChannels);
    
    
    LooperControlState  looper_control_state;
    
private:
    Loop                loop;
    Gui                 gui;
    TouchState          touch_state, last_touch_state;
    vector<Touch>       fingers;
    bool                debug;
    //Input_Interface inter;
   
};


#endif /* state_machine_hpp */
