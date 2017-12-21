#include "loop.h"


Loop::Loop()
{
    vector<float> newsample;
    Loop(newsample, 0, BUFFER_SIZE, N_CHANNELS);
}

Loop::Loop(vector<float> b, int ip, int bufsize, int nchan)
{
    //inicializando as variáveis
    sample     = b;
    lastSample = b;
    bufferSize = bufsize;
    nChannels = nchan;
    debug = false;
    
    setup();
}

Loop::~Loop()
{
    sample.clear();
    output_buf.clear();
    input_buf.clear();
}

void Loop::setup() {
    vector<float> new_sample;
    
    sample    =new_sample;
    lastSample=new_sample;
    bufferSize = BUFFER_SIZE;
    nChannels = N_CHANNELS;
    
    recording  = false;
    playing    = true;
    overdubbing = false;
    
    //começando do início
    outpos = 0;
    
    //setting the full loop
    set_full_looping_area();
    
    //inicializando as variáveis não usadas
    volume = 1.f;
    leftpan = 1.f;
    rightpan = 1.f;
    volume_start = volume;
    volume_end = volume;
    
    //init aux_looping area
    aux_start_index = -1;
    aux_end_index   = -1;
    aux_outpos = -1;
    aux_volume = 1.f;
    
    //variable that stores if the sample is silenced
    sum_that_checks_if_output_is_silenced = 1000;
    
    //sets the feedback
    feedback  = 1.f;
    delay     = 0.f;
    delay_offset_in_main_outpos = 0;
    delay_offset_in_aux_outpos = 0;
    
    //variable that stores if any finger was added
    locker_screen_has_no_finger = true;

    
    //leaves debug as it is! do not modify it!
}

void Loop::stop()
{
    //cancel_recording_or_overdubing();
    playing = false;
    outpos = 0;
}

void Loop::resume()
{
    playing = true;
}

void Loop::overdub()
{
    overdubbing = true;
}

void Loop::stop_overdub()
{
    if (overdubbing) {
        update_last_buffer();
        update_output_buffer();
    }
    
    overdubbing = false;
}

void Loop::cancel_recording_or_overdubing() {
    if (recording)   recording   = false;
    if (overdubbing) overdubbing = false;
    input_buf.clear();
    sample = output_buf;
}


int  Loop::get_size()
{
    return output_buf.size();
}

bool Loop::head_has_restarted()
{
    if ((outpos + (bufferSize*nChannels)) >= end_index)
        return true;
    else
        return false;
}

void Loop::update_head_position()
{
    
    //updating the current position
    outpos = ((outpos + (bufferSize*nChannels))%(end_index));
    
    //checks if the outpos if suitable
    if (outpos < start_index || outpos > end_index)
        
        //goes directly to the start_index
        outpos = start_index;
    
    //repeat the same thing if there is currentyl an aux looping area
    if (there_is_aux_looping_area()) {
        
        aux_outpos = ((aux_outpos + (bufferSize*nChannels))%(aux_end_index));
        
        if (aux_outpos < aux_start_index || aux_outpos > aux_end_index)
            aux_outpos = aux_start_index;
        
    }
    
}


float Loop::interpolate_volume(int buf_index)
{
    //computing the normalized head pos
    float normalized_pos = (buf_index-start_index)/(float)(end_index-start_index);
    
    //computing the current scale
    float current_volume = volume_start*(1-normalized_pos) + volume_end*(normalized_pos);
    
    //normalizing
    return (current_volume*2);
}


float Loop::interpolate_aux_volume(int buf_index)
{
    //computing the normalized head pos
    float normalized_pos = (buf_index-aux_start_index)/(float)(aux_end_index-aux_start_index);
    
    //computing the current scale
    float current_volume = aux_volume_start*(1-normalized_pos) + aux_volume_end*(normalized_pos);
    
    //normalizing
    return (current_volume*2);
}

/***********************************************
 * DELAY FUNCTIONALITIES
 ***********************************************/
// @TODO - BUG
// delay works with no fingers added. howeveer, 
// delay is not working with a single looping area
// not tested yet if it works with aux looping area

//sets the delay value (should be between 0 and 1)
void Loop::set_delay (float value)
{
    delay = ofClamp(value, 0.0, 1.0);
    compute_delay_offset();
}

//computes the outpos offset
void Loop::compute_delay_offset ()
{
    //also updates the offset caused by delay
    compute_delay_offset_in_main_outpos();
    compute_delay_offset_in_aux_outpos();
    
    
}

//computes the outpos offset caused by introduction of delay
void Loop::compute_delay_offset_in_main_outpos ()
{
    if (!is_empty()) {
        delay_offset_in_main_outpos = (int)((end_index-start_index)*delay);
        
        //if the result if odd, make it even
        if ((delay_offset_in_main_outpos%2) != 0)
            delay_offset_in_main_outpos--;
    }
}

//computes the outpos offset caused by introduction of delay
void Loop::compute_delay_offset_in_aux_outpos ()
{
    if (!is_empty() && there_is_aux_looping_area()) {
        delay_offset_in_aux_outpos = (int)((aux_end_index-aux_start_index)*delay);
        
        //if the result if odd, make it even
        if ((delay_offset_in_aux_outpos%2) != 0)
            delay_offset_in_aux_outpos--;
    }
}


//function that return the delayed index for the current index od the MAIN outpos
int Loop::get_delayed_index_from_main_current_index (int index)
{
    /*
    int delay_outpos;
    int index_offset = index - delay_offset_in_main_outpos;
    
    if (index_offset < start_index) {
        int offset_to_start = start_index - index_offset;
        delay_outpos = end_index - offset_to_start;
    } else
        delay_outpos = index_offset;
    
    return delay_outpos;
     */
    return get_delayed_index_based_on_offset_start_and_end(index,
                                                           delay_offset_in_main_outpos,
                                                           start_index,
                                                           end_index);
}

//function that return the delayed index for the current index of the AUX outpos
int Loop::get_delayed_index_from_aux_current_index (int index)
{
    /*
    int delay_outpos;
    int index_offset = index - delay_offset_in_aux_outpos;
    
    if (index_offset < aux_start_index) {
        int offset_to_start = aux_start_index - index_offset;
        delay_outpos = aux_end_index - offset_to_start;
    } else
        delay_outpos = index_offset;
    return delay_outpos;
     */
    return get_delayed_index_based_on_offset_start_and_end(index,
                                                           delay_offset_in_aux_outpos,
                                                           aux_start_index,
                                                           aux_end_index);
}

int Loop::get_delayed_index_based_on_offset_start_and_end (int index, int delay_offset, int start_pos, int end_pos) {
    int delay_outpos;
    int index_offset = index - delay_offset;
    
    if (index_offset < start_pos) {
        int offset_to_start = start_pos - index_offset;
        delay_outpos = end_pos - offset_to_start;
    } else
        delay_outpos = index_offset;
    return delay_outpos;
}

/***********************************************
 * FEEDBACK FUNCTIONALITIES
 ***********************************************/

//sets the feedback value (should be between 0 and 1)
void Loop::set_feedback(float newfeedback)
{
    feedback = ofClamp(newfeedback, 0, 1);
}

//applies feedback to the original sample at index position
void Loop::update_feedback_in_subpart_of_output_and_sample(int index)
{
    
    //applies the feeback on the first channel
    sample[index] = sample[index] * feedback;
    output_buf[index] = output_buf[index] * feedback;
    
    //and on the second channel
    if (nChannels==2) {
        sample[index+1] = sample[index] * feedback;
        output_buf[index+1] = output_buf[index] * feedback;
    }
}

/***********************************************
 * CLEARS LOOPER IF THE SAMPLE IS SILENCED
 * when the head starts...
 *     is the sum of all sample indexes close to zero?
 *          YES - stops and erases loop
 *          NO - re-inits variables storing the sum; and keeps summing while playing
 ***********************************************/


void Loop::initialize_sum_that_checks_if_output_is_silenced()
{
    sum_that_checks_if_output_is_silenced = 0;
}

void Loop::adds_to_sum_that_checks_if_output_is_silenced(float index)
{
    sum_that_checks_if_output_is_silenced += abs(output_buf[index]);
    
    if (nChannels==2)
        sum_that_checks_if_output_is_silenced += abs(output_buf[index+1]);
}

bool Loop::is_output_silenced() {
    return (sum_that_checks_if_output_is_silenced < 0.01);
}

bool Loop::output_is_silenced_and_screen_has_no_fingers()
{
    return (is_output_silenced() && locker_screen_has_no_finger);
}

/*
void Loop::resets_sum_checker()
{
    if (output_is_silenced_and_screen_has_no_fingers())
        clear();
    else
        initialize_sum_that_checks_if_output_is_silenced();
}
 */


//--------------------------------------------------------------
//play function
void Loop::play(float* &output)
{
    //if there is something to play, and looper is set to play
    if ((!is_empty()) & playing)
        process_output_buffer_in_a_loop(output);
    //if it not playing neither there is anything stored to play
    else
        mute_output_buffer(output);
}


//--------------------------------------------------------------
void Loop::mute_output_buffer(float* &output)
{
    for(int i=0; i<bufferSize; i++) {
        output[i*nChannels] = 0;
        output[(i*nChannels)+1] = 0;
    }
}

//--------------------------------------------------------------
void Loop::process_output_buffer_in_a_loop(float* &output)
{
    for(int i=0; i<bufferSize; i++) {
        
        //debug only. hopefully, you can try to delete me latter
        if (output_buf.size() == 0) {
            cout << "output_buf is empty" << endl;
            return;
        }
        
        process_output_buffer_at_one_index(output, i);
    }
    
    update_head_position();
    
    //if (overdubbing)
    //    update_output_buffer();
}


//--------------------------------------------------------------
void Loop::process_output_buffer_at_one_index(float* &output, int i) {
    
    //computing the index
    int index            = i*nChannels;
    
    //computing the index in the output_buf
    int output_buf_index = (outpos + index)%output_buf.size();
    
    //gets the delayed index
    int delayed_index = get_delayed_index_from_main_current_index(outpos+index)%output_buf.size();
    
    //computes the current volume
    float current_volume = interpolate_volume(output_buf_index);
    
    //adds to the sum variables that checks if the output is silenced
    adds_to_sum_that_checks_if_output_is_silenced(output_buf_index);
    
    //applies the feedback to the output and on the sample, if any
    update_feedback_in_subpart_of_output_and_sample(output_buf_index);
    
    //feeds the output with the current position and its delay
    output[index  ]  = output_buf[output_buf_index] * 0.5 * current_volume * leftpan;
    output[index  ] += output_buf[delayed_index] * 0.5 *  current_volume * leftpan;

    //in case it's overdub, feed the buffer as well
    if (overdubbing)
    {
        output[index  ] += sample[output_buf_index] * 0.5 *  current_volume * leftpan;
        output[index  ] += sample[delayed_index] * 0.5 *  current_volume * leftpan;
    }
    
    //in this case, we are getting the value from the right, and feeding the left channel
    //because we are using only the left channel of the focus right
    if (nChannels==2)
    {
        output[index+1]  = output_buf[output_buf_index] * 0.5 *  current_volume * rightpan;
        output[index+1] += output_buf[delayed_index] * 0.5 *  current_volume * rightpan;
        
        //in case it's overdub, feed the buffer as well
        if (overdubbing)
        {
            output[index+1] += sample[output_buf_index] * 0.5 *  current_volume * rightpan;
            output[index+1] += sample[delayed_index] * 0.5 *  current_volume * rightpan;
        }
    }
    
    //in case there are more channels (eg. x channels), remember to update output[index+x]. so far, this code will only work with n channels.
    
    //[AUX] computing the index in the output_buf
    int   aux_output_buf_index = (aux_outpos + index)%output_buf.size();
    
    //gets the delayed index of the aux
    delayed_index = get_delayed_index_from_aux_current_index(aux_outpos + index);
    
    float current_aux_volume = interpolate_aux_volume(aux_output_buf_index);
    
    //if there is currentyl an aux looping area
    if (there_is_aux_looping_area())
    {
        
        //same as before
        output[index] += output_buf[aux_output_buf_index] * 0.5 * current_aux_volume * leftpan;
        output[index] += output_buf[delayed_index] * 0.5 *  current_aux_volume * leftpan;
        
        //in case it's overdub, feed the buffer as well
        if (overdubbing)
        {
            output[index  ] += sample[aux_output_buf_index] * 0.5 *  current_aux_volume * leftpan;
            output[index  ] += sample[delayed_index] * 0.5 *  current_aux_volume * leftpan;
        }
        
        if (nChannels==2)
        {
            output[index+1] += output_buf[aux_output_buf_index] * 0.5 * current_aux_volume * rightpan;
            output[index+1] += output_buf[delayed_index] * 0.5 *  current_aux_volume * rightpan;
            
            //in case it's overdub, feed the buffer as well
            if (overdubbing)
            {
                output[index+1] += sample[aux_output_buf_index] * 0.5 *  current_aux_volume * rightpan;
                output[index+1] += sample[delayed_index] * 0.5 *  current_aux_volume * rightpan;
            }
        }
    }
}


//////////////////////////////////
// grava um determinado loop com o id pedido
// - id - o id do botao dono do loop
//////////////////////////////////
void Loop::record()
{
    if (!recording) //se não tá gravando, comece a gravar
    {
        recording = true;
        input_buf.clear();
    }
    
}

void Loop::stop_record()
{
    if (recording)       //se tá gravando, finalize
    {
        recording = false;
        
        init_last_buffer();
        
        sample=input_buf;
        
        //updates the output buffer
        update_output_buffer();
        
        //sets the loop from its begnning to its end
        set_full_looping_area();
        
        if (debug)
            cout << " loop created! size: " << sample.size() << endl;
    }
}


//--------------------------------------------------------------
void Loop::audio_input(float * &input)
{
    if (recording)
    {
        for(int i=0; i<bufferSize; i++) {
            input_buf.push_back(input[ i*nChannels]);
            if (nChannels==2)
                input_buf.push_back(input[(i*nChannels)+1]);
        }
    }
    
    //this will be executed if only if the current loop is set to overdub
    //for (int i = 0; i < loops.size();i++)
    this->overdub_sample_vector(input);
}



//////////////////////////////////
// funcao que é chamada quando existe saida a ser liberada
// - output - caixa de som
//////////////////////////////////
void Loop::audio_output(float * &output)
{
    this->play(output);    //toca o loop principal
}


void Loop::overdub_sample_vector(float* &input)
{
    //ignores this function if this loop is not setted to overdub
    if (!overdubbing)
        return;
    
    //overdubs
    for(int i=0; i<bufferSize; i++) {
        int   index = i*nChannels;
        
        sample[(outpos + index)%sample.size()]     += input[index  ]   *  leftpan;
        
        if (nChannels==2)
            sample[(outpos + index + 1)%sample.size()] += input[index +1 ] * rightpan;
        
        
        //[AUX] computing the index in the output_buf
        int   aux_output_buf_index = (aux_outpos + index)%sample.size();
        
        //if there is currentyl an aux looping area
        if (there_is_aux_looping_area()) {
            
            //same as before
            sample[aux_output_buf_index] += input[index]   * leftpan;
            
            if (nChannels==2)
                sample[aux_output_buf_index] += input[index+1] * rightpan;
        }
        
    }
}


void Loop::set_head_normalized(float position)
{
    int newHead = position*output_buf.size();
    
    set_head_absolute(newHead);
}


void Loop::set_head_absolute(int position)
{
    //if newHead is an odd number (right channel), makes it even (left channel)
    if (position%2 != 0)
        position -=1;
    
    //cout << newHead <<endl;
    outpos=position;
}


//--------------------------------------------------------------
void Loop::update_output_buffer()
{
    output_buf = sample;
}

//--------------------------------------------------------------
void Loop::update_last_buffer()
{
    lastSample = output_buf;
}

//--------------------------------------------------------------
void Loop::init_last_buffer()
{
    vector<float> empty_buf(input_buf.size(), 0);
    lastSample = empty_buf;
}

//////////////////////////////////
// returns if it's recording or not
//////////////////////////////////
bool Loop::is_recording()
{
    return (this->recording | this->overdubbing);
}

//////////////////////////////////
// returns if this loop is empty/clear nor not
//////////////////////////////////
bool Loop::is_empty()
{
    return (this->output_buf.size()==0);
}


//////////////////////////////////
// clears the loop
//////////////////////////////////
void Loop::clear()
{
    this->sample.clear();
    this->input_buf.clear();
    this->output_buf.clear();
    recording  = false;
    playing    = true;
    overdubbing = false;
    outpos = 0;
    volume = 1.f;
    aux_volume = 1.f;
}

//////////////////////////////////
// undo/redo last looper
//////////////////////////////////
void Loop::undo_redo()
{
    sample = lastSample;
    lastSample = output_buf;
    update_output_buffer();
}

void Loop::set_debug(bool debug)
{
    this->debug=debug;
}

void Loop::set_volume(float volume)
{
    this->volume=volume;
}

void Loop::set_aux_volume(float volume)
{
    this->aux_volume=volume;
}


//////////////////////////////////
// sets the sample to fully loop from its beg to its end
//////////////////////////////////
void Loop::set_full_looping_area()
{
    set_looping_area(0, output_buf.size());
}

//////////////////////////////////
// sets the sample to loop between start and end
//////////////////////////////////
void Loop::set_looping_area(int start, int end)
{
    //if start is an odd number (right channel), makes it even (left channel)
    if (start%2 != 0)
        start -=1;
    
    //if start is an even number (left channel), makes it even (right channel)
    if (end%2 != 0)
        end -=1;
    
    //if (debug)
    //    cout << "start: " << start << " end: " << end <<endl;
    
    start_index = start;
    end_index   = end;
    
    volume_start = volume;
    volume_end   = volume;
}

//////////////////////////////////
// sets the sample to loop in a second area between start and end
//////////////////////////////////
void Loop::set_looping_area_with_volume(int start, int end, float start_volume, float end_volume)
{
    set_looping_area(start, end);
    
    volume_start = start_volume;
    volume_end   = end_volume;
}


//////////////////////////////////
// sets the sample to loop in a second area between start and end
//////////////////////////////////
void Loop::set_aux_looping_area(int start, int end)
{
    //if start is an odd number (right channel), makes it even (left channel)
    if (start%2 != 0)
        start -=1;
    
    //if start is an even number (left channel), makes it even (right channel)
    if (end%2 != 0)
        end -=1;
    
    //setting the window
    aux_start_index = start;
    aux_end_index   = end;
    
    aux_volume_start = aux_volume;
    aux_volume_end   = aux_volume;
    
    //the the aux head
    //aux_outpos = start;
}

//////////////////////////////////
// sets the sample to loop in a second area between start and end
//////////////////////////////////
void Loop::set_aux_looping_area_with_volume(int start, int end, float start_volume, float end_volume)
{
    set_aux_looping_area(start, end);
    
    aux_volume_start = start_volume;
    aux_volume_end   = end_volume;
}

//////////////////////////////////
// sets the sample to loop between start and end
//////////////////////////////////
void Loop::remove_aux_looping_area()
{
    //removes everything
    aux_start_index = -1;
    aux_end_index   = -1;
    aux_outpos = 0;
}

//////////////////////////////////
// returns if there is an aux looping area
//////////////////////////////////
bool Loop::there_is_aux_looping_area()
{
    return (aux_start_index !=- 1 && aux_end_index !=- 1);
}

//////////////////////////////////
// returns if there is an looping area
//////////////////////////////////
bool Loop::there_is_looping_area()
{
    return (start_index != 0 && end_index != output_buf.size());
}

//returns the looping area
ofPoint Loop::get_looping_area()
{
    ofPoint looping_area(start_index, end_index);
    return looping_area;
}

//returns the aux looping area
ofPoint Loop::get_aux_looping_area()
{
    ofPoint aux_looping_area(aux_start_index, aux_end_index);
    return aux_looping_area;
}

