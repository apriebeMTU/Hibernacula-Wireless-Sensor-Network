// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality


// Define pins 
const uint8_t  GREEN_LED{6};  
const uint8_t  ORANGE_LED{7};  
const uint8_t  RED_LED{8};  



void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(GREEN_LED, HIGH);   
  delay(300);                       
  digitalWrite(ORANGE_LED, HIGH);    
  delay(300); 
  digitalWrite(RED_LED, HIGH);   
  delay(300);                       
  digitalWrite(RED_LED, LOW);    
  delay(300); 
  digitalWrite(ORANGE_LED, LOW);   
  delay(300);                       
  digitalWrite(GREEN_LED, LOW);    
  delay(300); 
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(ORANGE_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  delay(500); 
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ORANGE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  delay(500); 
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(ORANGE_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  delay(500); 
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ORANGE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  delay(500); 


}
