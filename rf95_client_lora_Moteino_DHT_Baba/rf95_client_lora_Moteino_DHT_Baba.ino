// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95

#include <SPI.h>
#include <RadioHead.h>
#include <RH_RF95.h>
#include <DFRobot_DHT11.h>
#include <RTCZero.h>

int count = 0;
uint8_t temperature; // test init to 10, uint8_t insure 8 bits length (i.e. unsigned char)
uint8_t humidity; // test init to 15

// Singleton instance of the radio driver
RH_RF95 rf95(A2, 9); //SPI CS init with MoteinoM0
// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB
DFRobot_DHT11 DHT;
#define DHT11_PIN A1

// instance of the RTC driver
RTCZero zerortc;
// Set how often alarm goes off here
const byte alarmSeconds = 0;
const byte alarmMinutes = 15;
const byte alarmHours = 0;

// alarm flag to know if awake or not
volatile bool alarmFlag = false; // Start awake

void setup() 
{
  Serial.begin(115200);
  Serial.println("Connecting..."); 
  //while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init()){
    Serial.println("init failed");
  }else { 
      Serial.print("init OK - ");
  } 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // You can change the modulation parameters with eg
  // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  #define RF95_FREQ 915
  rf95.setFrequency(RF95_FREQ);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 2 to 20 dBm:
   rf95.setTxPower(13, false);

   // put radio to sleep
  Serial.println("LoRa going to sleep");
  rf95.sleep();

  // Set up clocks and such
  Serial.println("clock init");
  zerortc.begin();

  resetAlarm();  // Set alarm
  Serial.println("timer is set");
  zerortc.attachInterrupt(alarmMatch); // Set up a handler for the alarm
}

void loop()
{
    if (alarmFlag == true) {
    alarmFlag = false;  // Clear flag
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Alarm went off - I'm awake!");
    Serial.println("Sending LoRa to pi_gateway");
    
    transmission();
    delay(1000);
  }
  resetAlarm();  // Reset alarm before returning to sleep
  Serial.println("Alarm reset, going to sleep now.");
  zerortc.standbyMode();    // Sleep until next alarm match
}

void alarmMatch(void) {
  alarmFlag = true; // Set flag
}

void resetAlarm(void) {
  byte seconds = 0;
  byte minutes = 0;
  byte hours = 0;
  byte day = 0;
  byte month = 0;
  byte year = 0;
  
  zerortc.setTime(hours, minutes, seconds);
  zerortc.setDate(day, month, year);

  zerortc.setAlarmTime(alarmHours, alarmMinutes, alarmSeconds);
  zerortc.enableAlarm(zerortc.MATCH_HHMMSS);
}

void transmission(){
  Serial.println(count);
  Serial.println("Preparing to send to rf95_server");
  //compiling sensor data
  Serial.println("Compiling sensor data...");
//  here we would read sensors data [Baba]
  //Serial.println(temperature); //[Baba]
  //Serial.println(humidity); //[Baba]
  DHT.read(DHT11_PIN);
  delay(2000);
  temperature = DHT.temperature; // test init to 10, uint8_t insure 8 bits length (i.e. unsigned char)
  humidity = DHT.humidity; // test init to 15
  // temperature = 10; // test init to 10, uint8_t insure 8 bits length (i.e. unsigned char)
  // humidity = 15; // test init to 15
  // Send a message to rf95_server
  uint8_t data[] = {temperature, humidity}; //we want to send an array of two bytes [Baba]
  //Serial.print("data "); Serial.print(data[0]); Serial.print(" "); Serial.println(data[1]);  
  //data[0] = temperature; // first byte is the temperature measurement [Baba]
  //data[1] = humidity; // seond byte is the humidity measurement [Baba]
  rf95.send(data, sizeof(data));
    Serial.println("Message sent");
  rf95.waitPacketSent();
  count++;
//  delay(5000);
} 

void reply(){
//  // Now wait for a reply
// uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
// uint8_t len = sizeof(buf);
//
// if (rf95.waitAvailableTimeout(5000))
// { 
//   // Should be a reply message for us now   
//   if (rf95.recv(buf, &len))
//  {
//     Serial.print("got reply: ");
//     Serial.println((char*)buf);
////      Serial.print("RSSI: ");
////      Serial.println(rf95.lastRssi(), DEC);    
//   }
//   else
//   {
//     Serial.println("recv failed");
//   }
// }
// else
// {
//   Serial.println("No reply, is rf95_server running?");
// }
}
