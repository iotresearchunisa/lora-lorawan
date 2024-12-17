#include "ArduinoLowPower.h"
#include <MKRWAN.h>

#define LED_PIN 9

LoRaModem modem;

String appEui = "8e1b6aca68cc671b2fa08ae28519c967";
String appKey = "8e1b6aca68cc671b2fa08ae28519c967";

void setup() {
  Serial.begin(115200);
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  if (modem.version() != ARDUINO_FW_VERSION) {
    Serial.println("Please make sure that the latest modem firmware is installed.");
    Serial.println("To update the firmware upload the 'MKRWANFWUpdate_standalone.ino' sketch.");
  }
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);

  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  } else {
    Serial.println("Connected");
  }

  // do not remove this. It is needed to be able to upload a new sketch
  delay(5000);

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  int data = analogRead(0);
  String msg = String(data);
  int err = send_msg(msg);
  if (err > 0) {
    Serial.print("Sent: ");
    Serial.println(msg);
  } else {
    Serial.println("Error sending message");
  }

  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  // max 1 message every 2 minutes. Enforced by firmware
  delay(130000);
  //LowPower.deepSleep(130000);  // 130 secs
  // Serial will not work
}

int send_msg(String msg) {
  modem.beginPacket();
  modem.print(msg);
  return modem.endPacket(true);
}
