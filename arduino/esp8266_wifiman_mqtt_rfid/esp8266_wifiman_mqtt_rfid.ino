#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//#include <PubSubClient.h>
const char* mqtt_server = "192.168.1.44";

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
String rfiduid ="";


#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D8
#define RST_PIN D0
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
// Init array that will store new NUID
byte nuidPICC[4];

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522
    Serial.println();
    Serial.print(F("Reader :"));
    rfid.PCD_DumpVersionToSerial();
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    Serial.println();
    Serial.println(F("This code scan the MIFARE Classic NUID."));
    Serial.print(F("Using the following key:"));
    printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset settings - for testing
    //wifiManager.resetSettings();
    
    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    wifiManager.setTimeout(180);
    
    //fetches ssid and pass and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    if(!wifiManager.autoConnect("AutoConnectAP")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
    } 
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    client.setServer(mqtt_server, 1883);
    //client.setCallback(callback);
 
}

void loop() {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    memset(msg, '\0', sizeof msg);
    // put your main code here, to run repeatedly:
    if ( ! rfid.PICC_IsNewCardPresent())
    return;
    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
    return;
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));
    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return;
    }
    if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    rfid.uid.uidByte[1] != nuidPICC[1] ||
    rfid.uid.uidByte[2] != nuidPICC[2] ||
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
        Serial.println(F("A new card has been detected."));
        // Store NUID into nuidPICC array
        for (byte i = 0; i < 4; i++) {
            nuidPICC[i] = rfid.uid.uidByte[i];
        }
        Serial.println(F("The NUID tag is:"));
        Serial.print(F("In hex: "));
        printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
        Serial.print(F("In dec: "));
        printDec(rfid.uid.uidByte, rfid.uid.size);
        //UIDDEC = printDec(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
        rfiduid.toCharArray(msg,MSG_BUFFER_SIZE);
        client.publish("outTopic",msg);
    }
    else Serial.println(F("Card read previously."));
    rfuidstring(rfid.uid.uidByte, rfid.uid.size);
    rfiduid.toCharArray(msg,MSG_BUFFER_SIZE);
    client.publish("outTopic",msg);
    // Halt PICC
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

}


/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}

void rfuidstring(byte *buffer, byte bufferSize) {
    rfiduid="";
    for (byte i = 0; i < bufferSize; i++) {
        rfiduid+=String(buffer[i], HEX);
    }
}
