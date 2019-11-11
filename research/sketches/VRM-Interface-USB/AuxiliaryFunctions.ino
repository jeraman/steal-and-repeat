
//functions
void arcadeButtonControl()
{
  buttonState = digitalRead(buttonPin);

  lightControl();

  if (buttonHasBeenPressed())
  {
    if (isSecondTimeButtonPressedInASecond())
      buttonPressedTwiceEvent();
    else
      buttonPressedEvent();
  }

  if (buttonHasBeenDownForTwoSeconds())
    buttonPressedAndHoldEvent();
  //else if (elapsedDownTime >= 2.0)
  //  resetButtonDownCounter();

  buttonLastState = buttonState;
}

void lightControl()
{
  if (isButtonDown())  
    turnOnLED();
  else                 
    turnOffLED();
}

bool isButtonDown()
{
  return (buttonState == 1);
}

bool wasLastButtonStateDown()
{
  return (buttonLastState == 1);
}


bool buttonHasChangedState()
{
  return (buttonState != buttonLastState);
}


bool buttonHasBeenPressed()
{
  //return (isButtonDown() && buttonHasChangedState());
  return (wasLastButtonStateDown() && buttonHasChangedState());
}


bool buttonHasBeenDownForTwoSeconds()
{
  startCountingPressHoldTimestampIfButtonIsDown();

  if (buttonPressHoldTimestamp == 0) return false;
  unsigned long elapsed = (millis() - buttonPressHoldTimestamp) / 1000;
  //Serial.println(elapsed);
  return (isButtonDown() && !buttonHasChangedState() && elapsed >= 2.0);
}


bool isSecondTimeButtonPressedInASecond()
{
  if (buttonPressedTimestamp == 0) return false;
  elapsedDownTime = (millis() - buttonPressedTimestamp) / 1000;
  //return (elapsedDownTime < 1,0);
  //resetButtonDownCounter(); //bug?
  return (elapsedDownTime < 0.5);
}


void startCountingPressHoldTimestampIfButtonIsDown()
{
  if (isButtonDown() && buttonHasChangedState())
    buttonPressHoldTimestamp = millis();

  //if (!isButtonDown())
  //  buttonPressHoldTimestamp = 0;
}

void resetButtonDownCounter()
{
  buttonPressedTimestamp = 0;
  elapsedDownTime        = 0;
}

void resetPressHoldTimestamp()
{
  buttonPressHoldTimestamp = 0;
  hasHadPressHoldEvent = true;
}


void potentiometersControl()
{
  pot1val = analogRead(pot1port);
  pot2val = analogRead(pot2port);

  if (hasPotentiometerValueChanged(pot1val, lastPot1val))  potentiometer1Event();
  if (hasPotentiometerValueChanged(pot2val, lastPot2val))  potentiometer2Event();

  lastPot1val = pot1val;
  lastPot2val = pot2val;
}


bool hasPotentiometerValueChanged (int value, int lastValue)
{
  return (abs(value - lastValue) > 1);
}


void potentiometer1Event()
{
  if (debug) Serial.print("Pot 1: ");
  if (debug) Serial.println((float)(pot1val / 1023.));
  sendFeedbackMIDI((float)(pot1val / 1023.));
}


void potentiometer2Event()
{
  if (debug) Serial.print("Pot 2: ");
  if (debug) Serial.println((float)(pot2val / 1023.));
  sendPhasingMIDI((float)(pot2val / 1023.));
}


void buttonPressedEvent()
{
  if (hasHadPressHoldEvent)
  {
    hasHadPressHoldEvent = false;
    return;
  }

  buttonPressedTimestamp = millis();
  if (debug) Serial.println("Red button pressed!");
  sendButtonPressMIDI();

  if (debug) switchLEDOnOff();
}


void buttonPressedTwiceEvent()
{
  resetButtonDownCounter();
  if (debug) Serial.println("Red button pressed TWICE in a second!");
  sendButtonPressTwiceMIDI();
  if (debug) turnOffLED();
}


void buttonPressedAndHoldEvent()
{
  resetPressHoldTimestamp();
  if (debug) Serial.println("Red button pressed and HOLD for 2 seconds!");
  sendButtonHoldMIDI();
  if (debug) turnOffLED();
}


void switchLEDOnOff()
{
  isButtonLightOn = !isButtonLightOn;

  if (isButtonLightOn)
    digitalWrite(buttonLED, HIGH);
  else
    digitalWrite(buttonLED, LOW);
}

void turnOnLED()
{
  isButtonLightOn = true;
  digitalWrite(buttonLED, HIGH);
}

void turnOffLED()
{
  isButtonLightOn = false;
  digitalWrite(buttonLED, LOW);
}

void setsLEDSfromMIDI(bool turnOn)
{
  if (turnOn)
    turnOnLED();
  else
    turnOffLED();
}

void debugRaw()
{
  if (isButtonLightOn)
    Serial.print("Light On! ");
  else
    Serial.print("Light Off... ");

  Serial.print("   pot 1: ");
  Serial.print(pot1val);
  Serial.print("   pot 2: ");
  Serial.print(pot2val);
  Serial.println();
}
