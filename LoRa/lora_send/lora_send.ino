#include <LoRa.h>

#define LED_PIN 9

String values[5]{ "test", "hello", "123", "123", "test" };
int i = 0;

void setup() {
  Serial.begin(9600);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  } else {
    Serial.println("LoRa send");
  }

  LoRa.setSyncWord(0xF3);

  // do not remove this. It is needed to be able to upload a new sketch
  delay(5000);

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  send_msg(values[i]);

  Serial.print("Sent: ");
  Serial.println(values[i]);

  i = (i + 1) % 5;

  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  delay(2000);
}

void send_msg(String msg) {
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
}
