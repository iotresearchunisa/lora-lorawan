#include <LoRa.h>

#define LED_PIN 9

String last_forward = "";

void setup() {
  Serial.begin(115200);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  } else {
    Serial.println("LoRa forward");
  }

  LoRa.setSyncWord(0xF3);

  // do not remove this. It is needed to be able to upload a new sketch
  delay(5000);

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  String msg = "";
  while (msg.length() <= 0) {
    msg = read_msg();
  }

  if (last_forward != msg) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    send_msg(msg);
    last_forward = msg;
    Serial.println("Message forwarded");

    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

String read_msg() {
  int packetSize = LoRa.parsePacket();
  String msg = "";

  if (packetSize) {
    Serial.print("Received packet: ");
    while (LoRa.available()) {
      msg += (char)LoRa.read();
    }
    Serial.println(msg);
  }

  return msg;
}

void send_msg(String msg) {
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
}
