// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client

#include <SPI.h>
#include <RadioHead.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95(20, 18); //RFGateway
int led = D37_LED;

void setup() {  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);      
  Serial.begin((115200));
  Serial.println("Connecting..."); 
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init()){
    Serial.println("init failed"); 
    }else { 
      Serial.print("init OK - ");
      rf95.setModeRx();
      Serial.print("Rx mode on");
  } 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  #define RF95_FREQ 915
  rf95.setFrequency(RF95_FREQ);
  // You can change the modulation parameters with eg
  // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);

// The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 2 to 20 dBm:
 rf95.setTxPower(13, false);
}

void loop(){
  // Serial.println("Listening..."); 
  if (rf95.available()){
      Serial.println("Checking messages"); 
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)){
        Serial.println("Mesage found");
      // we know that for one message, the firt byte is temperature data and the second is humidity data [Baba]
      Serial.print("humidity: "); //[Baba]
      Serial.println(buf[0]); //[Baba]
      Serial.print("temperature: "); //[Baba]
      Serial.println(buf[1]); //[Baba]
        digitalWrite(led, HIGH);
        digitalWrite(led, LOW);
        digitalWrite(led, HIGH);
        digitalWrite(led, LOW);

    //  Serial.print("RSSI: ");
    //  Serial.println(rf95.lastRssi(), DEC);
      
      //Send a reply
//      uint8_t data[] = "Oh hello";
//      rf95.send(data, sizeof(data));
//      rf95.waitPacketSent();
//      Serial.println("Sent a reply");
    }
    else{
      Serial.println("recv failed");
    }
  }
}
