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


/********************************
 Handles the main software control
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
    
    void record();
    void overdub();
    void stop();
    void resume();
    void clear_loops();
    bool is_loop_empty();
    
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut(float * input, int bufferSize, int nChannels);
    
private:
    Loop            loop;
    Gui             gui;
    State           state, last_state;
    //Input_Interface inter;
    vector<Touch>   fingers;
    bool            debug;
   
};


#endif /* state_machine_hpp */
