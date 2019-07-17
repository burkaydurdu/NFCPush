
#define SS_PIN D4
#define RST_PIN D2


#include <WiFiClientSecure.h>   // Include the HTTPS library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <SPI.h>
#include <MFRC522.h>



ESP8266WiFiMulti mWifi;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const char* ssid = "";
const char* pass = "";

// Push Notification
const char* host = "api.pushbullet.com";
const char* PushBulletAPIKEY = "o.IQnj3KHununjIAevHjgtu1gk8LWm9GTm";

const char* fingerprint = "2C BC 06 10 0A E0 6E B0 9E 60 E5 96 BA 72 C5 63 93 23 54 B3";
const int httpsPort = 443;

void setup() {
	Serial.begin(9600);
	setupWifi();
	delay(500);
	setupWebClient();
	setupMfrc();
}

void loop() {
	if ( ! mfrc522.PICC_IsNewCardPresent()) return;
	if ( ! mfrc522.PICC_ReadCardSerial()) return;
	Serial.println(readRfid().substring(1));
	delay(300);
}

String readRfid() {
	String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
 	content.toUpperCase();
	return content;
}


void setupMfrc() {
	SPI.begin();
	mfrc522.PCD_Init();

	Serial.println("Ready MFRC\n");
}

void setupWifi() {
	mWifi.addAP("VODAFONE_D905", "Hal1Bu2Ev");
	mWifi.addAP("Pisano DEV-2G", "p15@n0wifi");
	
	Serial.println("Connecting ...");
	// Wait for the Wi-Fi to connect
	while(mWifi.run() != WL_CONNECTED) {
		delay(250);
		Serial.print(".");
	}
	// When it connected 
	Serial.println('\n');
	Serial.println("Connected to ");
	Serial.println(WiFi.SSID());
	Serial.println("IP address:\t");
	Serial.println(WiFi.localIP());
	Serial.println('\n');
}

void setupWebClient() {
	// Use WiFiClientSecure class to create TLS connection
		WiFiClientSecure client;
		Serial.print("connecting to ");
		Serial.println(host);
		if (!client.connect(host, httpsPort)) {
			Serial.println("connection failed");
			return;
		}

		if (client.verify(fingerprint, host)) {
			Serial.println("certificate matches");
		} else {
	 		Serial.println("certificate doesn't match");
		}
		String url = "/v2/pushes";
		String messagebody = "{\"type\": \"note\", \"title\": \"ESP8266\", \"body\": \"Hello World!\"}\r\n";
		Serial.print("requesting URL: ");
		Serial.println(url);
		
		client.print(String("POST ") + url + " HTTP/1.1\r\n" + 
												"Host: " + host + "\r\n" +
												"Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
												"Content-Type: application/json\r\n" +
											  "Content-Length: " + 
												String(messagebody.length()) + "\r\n\r\n");
	
		client.print(messagebody);
		Serial.println("request sent");
}
