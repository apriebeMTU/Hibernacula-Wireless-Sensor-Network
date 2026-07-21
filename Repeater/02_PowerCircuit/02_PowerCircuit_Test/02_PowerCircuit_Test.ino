// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality


// Define pins 
const uint8_t  LED_PIN{8};  
const uint8_t  POWER_PIN{9};  

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(POWER_PIN, HIGH); 
  delay(5000);                       
  digitalWrite(POWER_PIN, LOW);    
  delay(5000);  
}
