#define SS_PIN D4 // RFID SDA PIN
#define RST_PIN D2 // RFID RST PIN

#include <WiFiClientSecure.h>   // Include the HTTPS library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <SPI.h>
#include <MFRC522.h>


ESP8266WiFiMulti mWifi;
WiFiClientSecure client;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Push Notification
const char* host = "api.pushbullet.com";
const char* PushBulletAPIKEY = "XXX";

bool isPush = false;

const char* fingerprint = "2C BC 06 10 0A E0 6E B0 9E 60 E5 96 BA 72 C5 63 93 23 54 B3";
const int httpsPort = 443;

void setup() {
	Serial.begin(9600);
	setupWifi();
	setupMfrc();
	isPush = setupWebClient();
}

void loop() {
	if ( ! mfrc522.PICC_IsNewCardPresent()) return;
	if ( ! mfrc522.PICC_ReadCardSerial()) return;
	Serial.println(readRfid().substring(1));
	if (isPush) pushNotification();
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
	mWifi.addAP("xxx", "xxx");
	mWifi.addAP("xxx", "xxx");
	mWifi.addAP("xxx", "xxx");
	
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

bool setupWebClient() {
	// Use WiFiClientSecure class to create TLS connection	
	Serial.print("connecting to ");
	Serial.println(host);
	
	client.setInsecure();

	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return false;
	}

	if (client.verify(fingerprint, host)) {
		Serial.println("certificate matches");
	} else {
	 	Serial.println("certificate doesn't match");
		return false;
	}

	return true;
}

void pushNotification() {
	String url = "/v2/pushes";
	String messagebody = "{\"type\": \"note\", \"title\": \"ESP8266\", \"body\": \"Hello World!\"}\r\n";

	String messagebody_link = "{\"type\": \"link\", \"title\": \"Pis\", \"body\": \"Hii^^!\",\"device_iden\": \"ujCdJBKOTEOsjyaFXj1GXA\", \"url\": \"https://www.google.com\"}\r\n";

	Serial.print("requesting URL: ");
	Serial.println(url);
		
	client.print(
		String("POST ") + url + " HTTP/1.1\r\n" + 
		"Host: " + host + "\r\n" +
		"Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
		"Content-Type: application/json\r\n" +
		"Content-Length: " + 
		String(messagebody_link.length()) + "\r\n\r\n");
	
	client.print(messagebody_link);
	Serial.println("request sent");

	while (client.available() == 0);
	while (client.available()) {
		String line = client.readStringUntil('\n');
		Serial.println(line);
	}
}
