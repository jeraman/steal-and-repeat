# Prototyping Circuitry
[![video-demo](./images/video.jpg)](https://www.youtube.com/watch?v=3yMFNxZIu5M)

This tutorial describes how to build a DIY controller for the Voice Reaping Machine. As this controller is intended to replicate the controls of the Boss RC-1, it supports four types of events:
- Simple Press button;
- Press button twice (in less than a second);
- Press & Hold (during 2 seconds);
- Knobs events (both 'persistence' and 'phasing').

## Materials & Steps
The circuit is looks like the following:

![wiring](./images/circuit.jpg)

As materials, we are going to need:
- A 5v Arduino;
- A [Bluefruit board](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration) (Bluefruit LE UART Friend);
- A [button](https://www.adafruit.com/product/3489);
- Two [10k potentiometers](https://www.digikey.com/product-detail/en/tt-electronics-bi/P0915N-FC15BR10K/987-1649-ND/4780740) with [plastic knobs](https://www.adafruit.com/product/2047);
- One 10k resistor (you can find such resistor inside a [resistor kit like this one](https://www.sparkfun.com/products/10969));
- A bunch of [jumpers](https://www.sparkfun.com/products/14284);
- A [battery snap holder](https://www.sparkfun.com/products/91);
- A [rocker switch](https://www.sparkfun.com/products/8837).

That said, these are the steps to get the 'pedal' working:
- [x] Get started on Arduino;
- [x] Bluetooth BLE board (Bluefruit & Arduino);
- [x] MIDI on Arduino;
- [x] MIDI BLE from Arduino to a Mobile;
- [x] Connect the button;
- [x] Connect the two potentiometers;
- [x] Connect battery and rocker switch;
- [x] Writing the Arduino code;
- [x] Test the circuit with the VRM app;
- [x] Record a video of the prototype;

Each step is detailed in the following subsections.

## Get started on Arduino!
Because there was no cable (get it ASAP!), decided to use my old Arduino Uno (still working!) instead of the brand new Arduino Micro. I also updated Arduino's IDE software.

## Bluetooth BLE board
First, I soldered the connectors into the board. Then, I tested to see if the module was working all right by following the tutorial [here](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend).

In short, you'll need:

### Wiring
Just like in this image:
![wiring](https://cdn-learn.adafruit.com/assets/assets/000/025/182/medium800/adafruit_products_UARTFriend_bb.png)

### Setting Adafruit's BLE on Arduino
For this I used [this tutorial](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/software).

The tutorial is effective, covering things such as [configuring the board](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration); sketch examples (e.g. using [AT commands](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/atcommand) from your computer to the board). By the end, you'll hopefully get your arduino, your BLE board, and you computer via serial port!

PS: Detailed documentation of AT commands can be found [here](https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/standard-at).

### Testing the BLE with a tablet/smartphone
You can test communication with your Android/iOs doing as demonstrate in [this video](https://www.youtube.com/watch?v=4hWlDKzn7tA).

## Midi on Arduino
Arduino has a [MIDI library](https://playground.arduino.cc/Main/MIDILibrary). By following this tutorial [here](http://bvavra.github.io/MIDI_Accordion/sending-midi/), I was able to send continuous MIDI messages (rate: 1 per second) to my computer via serial. The code is available [here](./sketches/midi-demo/midi-demo.ino).

**Sidenote:** If I was to synthesize these notes on the computer, we would need someone like [the Hairless MIDI](http://projectgus.github.io/hairless-midiserial/) to route messages from serial to your MIDI synth. For my purposes, won't bother doing that.

## MIDI BLE from Arduino to a Mobile
Afterwards, my goal moved towards replicating the same 'MIDI on Arduino' example now over BLE to a mobile device, which will synthesize the notes.

It wasn't hard. The default MIDI example available on the Adafruits'BLE library seemed to have worked (almost) out-of-the-box:

- Inside the code, I needed to adjust the [configurations](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration) to use Software Serial (I have an Arduino Uno);
- Also needed to use Adafruit's BLE connect app to connect to the board;
- To route the MIDI to GarageBand, I used the [MidiMttr](https://itunes.apple.com/us/app/midimittr/id925495245?mt=8) app.

I think that was all. The sketch used is available [here](./sketches/midi-ble/midi-ble.ino).

**Sidenote 1:** Very often, I get errors when uploading the sketches to the board. These errors differ: Sometimes I get "Couldn't factory reset"; Sometimes, I get "Could not enable MIDI". Sometimes, fortunately, it works fine (i.e. "waiting for a connection"). Need to study why that happens.

**Sidenote 2:** Need to study how to make the BLE connection more reliable & straightforward (e.g. no need of external Adafruit's or MidiMttr app).

## Connecting the button
Regarding [Arcade buttons I use](https://www.adafruit.com/product/3489), there are two pairs of connectors: one pair for controlling the LEDs, another for using the button itself. For the LED connectors, [specification](https://www.adafruit.com/product/3489#technical-details-anchor) says there is no need to use resistors so that it is possible to connect the button directly to the 5v and GND (these have built-in 1k resistors). For the button, you can follow [this basic tutorial ](https://www.arduino.cc/en/Tutorial/Button) in order to read the button inside Arduino. Here, I used a 10k resistor.

I've modified this demo to work with the LEDs: if the button is pressed, the LED will light up. The code is [here](./sketches/arcade-button-LED/arcade-button-LED.ino).

## Connect the potentiometers
Also very simple step. Just follow this [example here](https://www.arduino.cc/en/tutorial/potentiometer). The code example is available [here](./sketches/potentiometers/potentiometers.ino).

## Connect battery and rocker switch
Simple as [this](http://www.instructables.com/id/Powering-Arduino-with-a-Battery/)!

## Writing the Arduino code
Once done with the hardware part, it's time to code a Arduino Sketch that will combine all previous points MIDI BLE, the button, and the potentiometers.

The resulting Arduino Sketch is available [here](./sketches/VRM-Interface/).

## Test the circuit with the VRM app
After some updates in the source code, and some fine tuning so that the visuals could match the physical controls, I was able to test this circuit with the VRM app! ;)

The final result can be watched in [this video](https://www.youtube.com/watch?v=3yMFNxZIu5M). Have fun! ;)

# Update: Port to USB
In order to remove intermediary software (Adafruit's BLE connect app) and simplify power supply, I've decided to replace the BLE module by standard USB connection.

After looking for recent references, I've found [this one here](http://www.instructables.com/id/Custom-Arduino-MIDI-Controller/) that seems to work. If you need further info, [this link](https://github.com/tttapa/MIDI_controller/wiki/MIDI-over-USB) covers different approaches to MIDI-over-USB in Arduino.

My case, in short:
1. Get an MIDI-over-USB compatible Arduino (in my case, the Arduino Micro);
2. Test the device with the 'Blink' example;
3. Install the MIDIUSB library, available in Arduino IDE library manager;
4. Use this sketch here as a test.

And we are ready to go! ;)
