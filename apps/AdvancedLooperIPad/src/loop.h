#ifndef LOOP_H
#define LOOP_H

#include "ofMain.h"
#include <vector>


#define BUFFER_SIZE 256
#define SAMPLE_RATE	44100
#define N_CHANNELS 1

class Loop
{
    public:
         Loop();
         Loop(vector<float>, int, int = BUFFER_SIZE, int = N_CHANNELS);
        ~Loop();
    
        void setup();
    
        void audio_input (float*&); //should be called when there's any audio input
        void audio_output(float*&); //should be called when there's any audio output
    
        void play (float* &output);                   //plays the loop
        void overdub_sample_vector (float* &input);   //adds more audio info inside this loop
        void overdub();                               //sets the overdubbing boolean to true
        void record ();                               //records and stops recording a loop
        void stop ();                                 //stops this sample
        void resume ();                               //resumes the sample if stopped
        void clear ();                                //cleans the loop
        void set_volume (float);                      //sets the volume
        void set_aux_volume (float);                  //sets the aux volume
        void set_debug (bool);                        //debug control
        void set_head_normalized (float);             //sets the loop head position (between 0 and 1)
        void set_head_absolute (int);                 ///using the size of the sample
        void set_looping_area(int, int);              //sets the looping area between int and int
        void set_looping_area_with_volume(int, int, float, float); //sets the looping area between the two former ints. volume is a interpolation between the two last ints.
        void set_full_looping_area();                 //sets the sample to fully loop (from beg to end)
        void set_aux_looping_area(int, int);          //sets the aux looping area between int and int
        void set_aux_looping_area_with_volume(int, int, float, float); //sets the aux looping area between the two former ints. volume is a interpolation between the two last ints.
        void remove_aux_looping_area();
    
        ofPoint get_looping_area();                   //sets the looping area between int and int
        ofPoint get_aux_looping_area();               //sets the looping area between int and int
    
        void update_output_buffer ();                 //updates the output buffer
        void update_looping_area (int, int);
        void update_aux_looping_area (int, int);
    
    
        void  update_feedback_in_subpart_of_the_sample(int index); //updates the level of feedback
        void  set_feedback(float);                  //sets the feedback to a new value
        float get_feedback();                       //gets the current feedback
        void  initialize_sum_that_checks_if_sample_is_silenced();
        void  adds_to_sum_that_checks_if_sample_is_silenced(float);
        bool  is_sample_silenced();
        void  checks_if_sample_is_silenced_and_screen_has_no_fingers();
        bool  head_has_restarted();
    
        bool is_recording ();                      //returns if it's recording or not
        bool is_empty ();                          //if this loop is clear/empty or not
        bool there_is_looping_area();              //verifies if the looping area is currently working
        bool there_is_aux_looping_area();          //verifies if the aux looping area is currently working
        int  get_size ();                          //gets the size of this loop
        void update_head_position();               //updates the head position
        float interpolate_volume(int);             //function that interpolates the volume depending on volume_start, volume_end, based on a certain index (int)
        float interpolate_aux_volume(int);         //same thing, but for the aux
    
        bool muted;          //to mute the loop
        bool overdubbing;    //indicates if this loop is currently overdubbing or not
        bool recording;      //is it recording right now?
        bool playing;        //is it playing right now?
        bool debug;          //is it debugging right now?
    
        bool locker_screen_has_no_finger;
    
        int outpos;          //a agulha do vinil (para este loop)
        int start_index;     //index where the looping starts
        int end_index;       //index where the loop ends
    
        int aux_outpos;      //[AUX] a agulha do vinil (para este loop)
        int aux_start_index; //[AUX] index where the looping starts
        int aux_end_index;   //[AUX] index where the loop ends
        int bufferSize;      //tamanho do buffer quando gravado
        int nChannels;       //número de canais do loop
    

        float volume;        //stores the current volume
        float volume_start,     volume_end;     //stores the current volume for the start and the end
        float aux_volume_start, aux_volume_end; //stores the current volume for the start and the end for the aux
        float aux_volume;    //[AUX] stores the current volume
        float feedback;      //stores the feedback value
        float sum_that_checks_if_sample_is_silenced;

    
        vector<float> input_buf;        //stores the loop being currently recorded
        vector<float> output_buf;       //stores the loop that is being played
        vector<float> sample;           //stores the raw loop
    
        //unused variables
        float leftpan;       //stores the amount of left pan
        float rightpan;      //stores the amount of right pan

};

#endif // LOOP_H
