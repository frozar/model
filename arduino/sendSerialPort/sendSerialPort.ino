/*
  Button
  Turns on and off a light emitting diode(LED)
  connected to digital  pin 13, when pressing a
  pushbutton attached to pin 2.

   The circuit:
  * LED attached from pin 13 to ground
  * pushbutton attached to pin 2 from +5V
  * 10K resistor attached to pin 2 from ground
  * Note: on most Arduinos there is already an LED
  on the board  attached to pin 13.

  created 2005  by DojoDave http://www.0j0.org/

  modified 30 Aug 2011  by Tom Igoe

  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/Button
*/

int buttonState = 0;
String commande;
char commandeRecu;
void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  // read the state of the pushbutton value
  buttonState = digitalRead(2);
  // check if pushbutton is pressed.  if it is, the
  // buttonState is HIGH
  while (Serial.available() > 0){
    
    commande = Serial.readString();
    Serial.println("WHILE : [" + commande + "]");

  }
  
  if(commande == "coca") {
    digitalWrite(13, HIGH);
    Serial.println("allumer");
    
    delay(1000);
    
    digitalWrite(13, LOW);
    Serial.println("eteint");
    
    delay(1000);
    
    digitalWrite(13, HIGH);
    Serial.println("allumer");
    
    delay(1000);

    digitalWrite(13, LOW);
    Serial.println("eteint");

    Serial.println("ENDIF : " + commande);
  }
  else {
    // turn LED off
    digitalWrite(13, LOW);
    // commande="";
   //Serial.print("ici");
    // Serial.println("ELSE : " + commande);
  }
  commande = "";
  
}
