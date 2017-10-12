//
//  gui.hpp
//  advanced_looper
//
//  Created by jeronimo on 2016-09-15.
//
//

#ifndef gui_h
#define gui_h

#include "ofMain.h"
#include "loop.h"

/********************************
 Handles the graphic user interface
 ********************************/

class Gui
{
   
public:
     Gui();   //constructor
     Gui(bool);   //constructor
    ~Gui();   //desctructor
    
    void draw(Loop*);   //drawclass for visual feedback
    void drawBackground(bool);                            //draws the background
    void drawFirstLoop(Loop*);                            //draws the first loop
    void drawDelayedFirstLoop(Loop*);                     //draws the first loop when delayed
    void drawHead(Loop*);                                 //draws the head of the looper
    void drawAuxHead(Loop*);                              //draws the head of the looper
    void drawMic();                                       //draws the input mic in realtime
    void drawWindow(bool);                                //draws the window
    void drawDebug(Loop*);                                //draw debug info
    
    void init_mic_buffer(int);
    void update_mic_buffer(float *, int, int);
    void set_debug(bool);                                 //debug control
    void set_scale(float);
    void set_window_with_scale(int, int, int, int);
    void set_aux_window_with_scale(int, int, int, int);
    void remove_aux_window();
    void remove_window();
    bool there_is_an_window();
    
private:
    bool debug;
    bool there_is_an_aux_window;
    //int head_offset;
    float scale_whole_loop;
    float scale_window1_start, scale_window1_end;
    float scale_window2_start, scale_window2_end;
    int   position_window1_start, position_window1_end;
    int   position_window2_start, position_window2_end;
    
    vector <float> leftMic;  //stores the left channel of live mic
    vector <float> rightMic; //stores the right channel of live mic
    
    void  drawLoopPartAtindex(Loop*, int);
    float computeScalePerWindowType(int, int, int, int, int, int);
    bool  isIndexWithinMainWindowRange(int);
    bool  isIndexWithinAuxWindowRange(int);
};

#endif /* gui_h */
