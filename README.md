# NCFPUSH

### Network connection

```
#include <WiFiClientSecure.h>   // Include the HTTPS library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
```
> We should add.

```
ESP8266WiFiMulti mWifi;
WiFiClientSecure client;

...
void setupWifi() {
  mWifi.addAP("xxx", "xxx");
  mWifi.addAP("xxx", "xxx");
  mWifi.addAP("xxx", "xxx");

  Serial.println("Connecting ...");
  // Wait for the Wi-Fi to connect
  while (mWifi.run() != WL_CONNECTED) {
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
```

> We connection network with that code. We must write our wifi name and password on *mWifi.addAP*

```
 const char* host = "api.pushbullet.com";
 const char* PushBulletAPIKEY = "XXX";
 const char* fingerprint = "2C BC 06 10 0A E0 6E B0 9E 60 E5 96 BA 72 C5 const int httpsPort = 443;
 const String url = "/v2/pushes";

```

```
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

```
> We should write api pushbullet host and other config. We should call ``pushNotification()``. This func request api.
