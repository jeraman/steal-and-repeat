/****************************************
 * MIDI-Computer via serial port
 * From: http://bvavra.github.io/MIDI_Accordion/sending-midi/
 * 
 * Date: 24 Nov. 2017
 */

#include <MIDI.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

struct MySettings : public midi::DefaultSettings
{
  static const long BaudRate = 115200;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings);

int channel = 1;
int velo    = 127;

void setup() {
    while (!Serial);  // required for Flora & Micro
    delay(500);
    Serial.begin(115200);
    MIDI.begin();
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Arduino ready.");
}

void loop() {
 
  for (int note = 42; note < 127; note++) 
  {
    MIDI.sendNoteOn(note, velo, channel);  // Send a Note (pitch 42, velo 127 on channel 1)
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);                           // Wait for a second
    MIDI.sendNoteOff(note, velo, channel); // Stop the note
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000); 
  }
}
