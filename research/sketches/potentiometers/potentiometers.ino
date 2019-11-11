/* Analog Read to LED
 * ------------------ 
 *
 * turns on and off a light emitting diode(LED) connected to digital  
 * pin 13. The amount of time the LED will be on and off depends on
 * the value obtained by analogRead(). In the easiest case we connect
 * a potentiometer to analog pin 2.
 *
 * Created 1 December 2005
 * copyleft 2005 DojoDave <http://www.0j0.org>
 * http://arduino.berlios.de
 *
 */

int val1 = 0;       // variable to store the value coming from the sensor
int val2 = 0;       // variable to store the value coming from the sensor

int ledPin = 13;   // select the pin for the LED


void setup() {
  while (!Serial);  // required for Flora & Micro
  delay(500);
  
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);  // declare the ledPin as an OUTPUT

  Serial.println("Arduino initialized");
}

void loop() {
  val1 = analogRead(A1);    // read the value from the sensor
  val2 = analogRead(A4);  // read the value from the sensor
  
  digitalWrite(LED_BUILTIN, HIGH);      // turn the ledPin on
  delay(val1);                          // stop the program for some time
  digitalWrite(LED_BUILTIN, LOW);   // turn the ledPin off
  delay(val1);                          // stop the program for some time

  Serial.print("val pot 1: ");
  Serial.println(val1);
  Serial.print("val pot 2: ");
  Serial.println(val2);
  
  delay(1);   
}
