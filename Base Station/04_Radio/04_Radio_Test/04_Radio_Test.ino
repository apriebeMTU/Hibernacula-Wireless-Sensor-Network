//libraries
// Title: 
// Author: Alex Riebe
// Description: This program tests the functionality

#include <RHReliableDatagram.h>
#include <RH_RF69.h>

//define addresses
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//define radio freq
#define RF69_FREQ 915.0

#define RFM69_CS    17  
#define RFM69_INT   27  
#define RFM69_RST   26  

// Singleton instance of the radio driver
RH_RF69 driver(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void setup() {
  // serial setup
  Serial.begin(115200);

  //radio setup
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

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
}

void loop() {
  Serial.println("Sending to rf69_reliable_datagram_server");
  delay(100);
  
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
    Serial.println("Message sent, waiting for reply...");
    
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    } else {
      Serial.println("No reply (recvfromAckTimeout)");
    }
  } else {
    Serial.println("sendtoWait failed — likely no ACK received");
  }
  
  delay(1000);
}
