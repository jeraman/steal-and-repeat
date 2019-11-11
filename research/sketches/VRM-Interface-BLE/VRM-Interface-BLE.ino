/*
   VRM INTERFACE PROTOTYPE
   Name: Jeronimo Barbosa
   Date: November 27 2017.
*/

//arduino ports
const int  buttonPin  = 2;
const int  buttonLED  = 13;
const int  pot1port  = A1;
const int  pot2port  = A4;

//button control variables
int   buttonState     = 0;
int   buttonLastState = 0;
float elapsedDownTime = 0;
bool  isButtonLightOn = false;
unsigned long  buttonPressedTimestamp = 0;

//potentiometer variables
float  pot1val         = 0;
float  pot2val         = 0;
float  lastPot1val     = 0;
float  lastPot2val     = 0;


//iterative loop
void setup()
{
  while (!Serial);  // required for Flora & Micro
  delay(500);
  Serial.begin(115200);
  pinMode(buttonLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  setupMIDI();
  Serial.println("Arduino initialized");
}


void loop()
{
  arcadeButtonControl();
  potentiometersControl();
  loopMIDI();
  delay(10);
  //debugRaw();
}
