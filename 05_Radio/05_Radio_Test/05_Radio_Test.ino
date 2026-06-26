// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality

// Libraries
#include <RHReliableDatagram.h>
#include <RH_RF69.h>

//define addresses
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//define radio freq
#define RF69_FREQ 915.0


//define pins 
const uint8_t RFM69_CS{10};  
const uint8_t RFM69_INT{2};  
const uint8_t RFM69_RST{14}; 

const uint8_t  GREEN_LED{6};  
const uint8_t  ORANGE_LED{7};  
const uint8_t  RED_LED{8};  

const uint8_t  POWER_PIN{9};  

// Singleton instance of the radio driver
RH_RF69 driver(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void setup() {
  Serial.begin(9600);

  // initialize digital LED pins and power gate pin as outputs.
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);


  // turn on load switch
  digitalWrite(POWER_PIN, HIGH);

  //radio setup
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  pinMode(RFM69_CS, OUTPUT);   // cs
  digitalWrite(RFM69_CS, LOW);
  delay(10);
  digitalWrite(RFM69_CS, HIGH);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  Serial.println("Initializing radio...");
  if (!driver.init()) {
    Serial.println("Driver init failed — check wiring");
    while (1);
  }
  Serial.println("Driver init OK");

  digitalWrite(ORANGE_LED, HIGH);  
  
  if (!manager.init()) {
    Serial.println("Manager init failed — check interrupt pin or address");
    while (1);
  }
  Serial.println("Manager init OK");

  
  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  if (!driver.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }  

  driver.setTxPower(20, true);

  digitalWrite(RED_LED, HIGH); 
}

void loop() {
  if (manager.available())
  {
    //digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    //digitalWrite(GREEN_LED, HIGH);
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);

      // Send a reply back to the originator client
      if (!manager.sendtoWait(data, sizeof(data), from))
        Serial.println("sendtoWait failed");
    }
  }
  Serial.println("Waiting for packet...");
  digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
  delay(200);
}
