/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput
*/

int sensorPin = A0;   // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

String commande;
char commandeRecu;

void setup() {
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT:
  // pinMode(ledPin, OUTPUT);
  Serial.println("Setup done");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  
  while (Serial.available() > 0){
    commande = Serial.readString();
    // Serial.println("WHILE : [" + commande + "]");
    delay(200);
  }
  
  if(commande == "co") {
    //digitalWrite(13, HIGH);
    // Serial.println("allumer");

    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
  }
  else {
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
    // Serial.println("éteind");

    // turn LED off
    //digitalWrite(13, LOW);
    delay(500);
  }
  commande = "";

  //delay(500);
}
