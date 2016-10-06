#ifndef INPUT_INTERFACE_H
#define INPUT_INTERFACE_H

#include "ofMain.h"
#include <vector>
#include <algorithm>


//state machine for the interface
typedef enum {
    NONE,
    ONE_FINGER,
    TWO_FINGERS,
    THREE_FINGERS,
    FOUR_FINGERS
} State;


//a simple data structure for storing touch events
class Touch {
public:
    int id;
    float x;
    float y;
    float angle;
    float size;
    float distance;
    
    Touch(float x, float y, float angle=0, float size=0, int finger_id=0) {
        this->x = x;
        this->y = y;
        this->angle = angle;
        this->size  = size;
        this->distance = 0;
        this->id = finger_id;
    }
    
    void compute_distance_to_a_point (float posx) {
        this->distance = abs(x-posx);
    }
    
    //overloading the < operator to be used in the sorting
    //bool operator<(const Touch& t2) const { return (x < t2.x); }
};

//sorting function by x axis
static bool x_sorting (const Touch &a, const Touch &b) {
    return a.x < b.x;
}

//sorting function by distance from a given point.
//ATTENTION: you need to compute the distance before using this function!
static bool distance_sorting (const Touch &a, const Touch &b) {
    return a.distance < b.distance;
}


/********************************
 Hides from the system what input interface is currently being used (e.g. ipad, mouse pad, etc).
 ********************************/

class Input_Interface
{
    
public:
     Input_Interface ();     //constructor
     Input_Interface (bool); //constructor
    ~Input_Interface ();     //desctructor
    
    void          draw ();                     //drawclass for visual feedback
    State         get_state ();                //update the input interface update machine
    vector<Touch> get_fingers ();              //returns a vector with the current finger positions
    int           get_number_of_fingers();
    void          set_fingers (vector<Touch>&); //returns a vector with the current finger positions
    void          set_number_of_fingers(int);
    void          set_debug (bool);            //debug control
    
private:
    bool  debug;
    State state;
    int   number_of_fingers;
    
    //ofxMultiTouchPad pad;
    vector<Touch> * touches;
    
};

#endif // INPUT_INTERFACE_H
