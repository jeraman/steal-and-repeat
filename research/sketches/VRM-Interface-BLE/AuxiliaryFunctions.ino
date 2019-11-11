
//functions
void turnOnOffLED()
{
  isButtonLightOn = !isButtonLightOn;

  if (isButtonLightOn)
    digitalWrite(buttonLED, HIGH);
  else
    digitalWrite(buttonLED, LOW);
}


bool isButtonDown()
{
  return (buttonState == 1);
}


bool buttonHasChangedState()
{
  return (buttonState != buttonLastState);
}


bool buttonHasBeenPressed()
{
  return (isButtonDown() && buttonHasChangedState());
}


bool buttonHasBeenDownForTwoSeconds()
{
  if (buttonPressedTimestamp == 0) return false;
  elapsedDownTime = (millis() - buttonPressedTimestamp) / 1000;
  return (isButtonDown() && !buttonHasChangedState() && elapsedDownTime >= 2.0);
}


bool isSecondTimeButtonPressedInASecond()
{
  if (buttonPressedTimestamp == 0) return false;
  elapsedDownTime = (millis() - buttonPressedTimestamp) / 1000;
  return (elapsedDownTime < 1.0);
}


void resetButtonDownCounter()
{
  buttonPressedTimestamp = 0;
  elapsedDownTime        = 0;
}


void arcadeButtonControl()
{
  buttonState = digitalRead(buttonPin);

  if (buttonHasBeenPressed())
  {
    turnOnOffLED();

    if (isSecondTimeButtonPressedInASecond())
      buttonPressedTwiceEvent();
    else
      buttonPressedEvent();
  }

  if (buttonHasBeenDownForTwoSeconds())
    buttonPressedAndHoldEvent();
  else if (elapsedDownTime >= 2.0)
    resetButtonDownCounter();

  buttonLastState = buttonState;
}


bool hasPotentiometerValueChanged (int value, int lastValue)
{
  return (abs(value - lastValue) > 1);
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


void potentiometer1Event()
{
  Serial.print("Pot 1: ");
  Serial.println((float)(pot1val/1023.));
  sendFeedbackMIDI((float)(pot1val/1023.));
}


void potentiometer2Event()
{
  Serial.print("Pot 2: ");
  Serial.println((float)(pot2val/1023.));
  sendPhasingMIDI((float)(pot2val/1023.));
}


void buttonPressedEvent()
{
  buttonPressedTimestamp = millis();
  Serial.println("Red button pressed!");
  sendButtonPressMIDI();
}


void buttonPressedTwiceEvent()
{
  resetButtonDownCounter();
  Serial.println("Red button pressed TWICE in a second!");
  sendButtonPressTwiceMIDI();
}


void buttonPressedAndHoldEvent()
{
  resetButtonDownCounter();
  Serial.println("Red button pressed and HOLD for 2 seconds!");
  sendButtonHoldMIDI();
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
