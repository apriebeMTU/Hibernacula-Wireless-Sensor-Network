// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality

const uint8_t  LED_PIN{2};

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // LED ON
  delay(500);

  digitalWrite(LED_PIN, LOW);   // LED OFF
  delay(500);
}
