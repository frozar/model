/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * ------------------------------------------------------------------------------------------------------------
 *             MFRC522      NodeMCU Lolin      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   V3 Module          Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          ESP8266 (ESP-12F)  Pin           Pin       Pin        Pin              Pin
 * ------------------------------------------------------------------------------------------------------------
 * RST/Reset   RST          D3                 9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      D4                 10            53        D10        10               10
 * SPI MOSI    MOSI         D7                 11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         D6                 12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          D5                 13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "HUAWEI-B535-07AA"
#define STAPSK "rozar97431"
#endif

#define SERVER_IP "192.168.8.100:3000"

// #define RST_PIN         9          // Configurable, see typical pin layout above
// #define SS_PIN          10         // Configurable, see typical pin layout above

// #define RST_PIN 18 // D3
// #define SS_PIN 17  // D4
// Documentation link
// https://esp8266-shop.com/esp8266-guide/esp8266-nodemcu-pinout/
#define RST_PIN D3 // 0
#define SS_PIN D4	 // 2

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

const char *ssid = STASSID;
const char *password = STAPSK;

// byte newUid[4] = {0x71, 0x85, 0x13, 0x09};

void setup()
{
	Serial.begin(115200); // Initialize serial communications with the PC
	while (!Serial)
		; // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	delay(5000);
	Serial.println("Begin setup");
	// Serial.println("D3");
	// Serial.println(D3);
	// Serial.println("D4");
	// Serial.println(D4);
	SPI.begin();											 // Init SPI bus
	mfrc522.PCD_Init();								 // Init MFRC522
	delay(4);													 // Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	/* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.println("End setup");
}

String getUID(MFRC522::Uid *uid)
{
	String res = "";

	// Serial.print(F("Card UID:"));
	for (byte i = 0; i < uid->size; i++)
	{
		// if (uid->uidByte[i] < 0x10)
		// {
		// 	// Serial.print(F(" 0"));
		// 	res += F(" 0");
		// }
		// else
		// {
		// 	// Serial.print(F(" "));
		// 	res += F(" ");
		// }
		// Serial.print(uid->uidByte[i], HEX);
		res += String(uid->uidByte[i], HEX);
	}
	return res;
}

void loop()
{
	// Serial.println("Begin loop");
	// delay(1000);
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent())
	{
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
	{
		return;
	}

	// wait for WiFi connection
	if ((WiFi.status() == WL_CONNECTED))
	{

		WiFiClient client;
		HTTPClient http;

		Serial.print("[HTTP] begin...\n");
		// configure traged server and url
		http.begin(client, "http://" SERVER_IP "/postplain/"); //HTTP
		http.addHeader("Content-Type", "application/json");

		Serial.print("[HTTP] BEFORE POST\n");
		// start connection and send HTTP header and body
		int httpCode = http.POST("{\"uid\":" + getUID(&(mfrc522.uid)) + "}");

		// httpCode will be negative on error
		if (httpCode > 0)
		{
			// HTTP header has been send and Server response header has been handled
			Serial.printf("[HTTP] POST succeeded\n");
		}
		else
		{
			Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
		}

		http.end();
		delay(1000);
	}
}
