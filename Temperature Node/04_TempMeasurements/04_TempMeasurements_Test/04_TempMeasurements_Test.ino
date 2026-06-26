// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality


// Define pins 
const uint8_t  GREEN_LED{6};  
const uint8_t  RTD_HI{A3}; 
const uint8_t  RTD_LO{A2}; 
const uint8_t  POWER_PIN{9}; 

//#define RTD_HI A3
//#define RTD_LO A2

int voltageRTD = 0;
int pwmDuty = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED, OUTPUT);  // sets the pin as output
  pinMode(POWER_PIN, OUTPUT);

  digitalWrite(POWER_PIN, HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:
  voltageRTD = analogRead(RTD_HI) - analogRead(RTD_LO);

  pwmDuty = map(voltageRTD, 0, 1023, 0, 255);
  
  analogWrite(GREEN_LED, pwmDuty);

  delay(20); 

}
