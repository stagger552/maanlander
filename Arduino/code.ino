#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

// #include "Led.h"
#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>  // to handle this particular modem driver
#include "RF24.h"      // the library which helps us to control the radio modem

// #define LEDPIN 3        // Ditital pin connected to the LED.
#include <OneWire.h>
#include <DallasTemperature.h>

// Initialise Sensors

// Initialise Actuators
// Led led;
int ledState = LOW;  // ledState used to set the LED

#define RF24_PAYLOAD_SIZE 32
#define AAAD_ARO 1
#define AAAD_MODULE 3

#define ONE_WIRE_BUS 3  // Data wire is plugged into digital pin 2 on the Arduino

OneWire oneWire(3);
DallasTemperature sensors(&oneWire);  // Create sensors object using oneWire reference


uint8_t arm1;
uint8_t arm2;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10, which are CE & CSN pins  */
RF24 radio(9, 10);
const uint8_t rf24_channel[] = { 1, 26, 51, 76, 101 };                                                            // Radio channels set depending on satellite number
const uint64_t addresses[] = { 0x4141414430LL, 0x4141414431LL, 0x4141414432LL, 0x4141414433LL, 0x4141414434LL };  //with radioNumber set to zero, the tx pipe will be 'AAAD0', which is basically HEX'4141414430', which is remote DESTINATION address for our transmitted data. The rx pipe code is the local receive address, which is what the remote device needs to set for the remote devices 'tx' pipe code.
uint8_t txData[RF24_PAYLOAD_SIZE];
uint8_t rxData[RF24_PAYLOAD_SIZE];

// Timing configuration
unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long currentMillis;
unsigned long tempRequestMillis = 1000;  // milliseconds of on-time


OneWire ds(3);  // on pin 10 (a 4.7K resistor is necessary)

void setup() {
  Serial.begin(9600);
  Serial.println("nRF24 Application ARO" + String(AAAD_ARO) + ", Module" + String(AAAD_MODULE) + " Started!\n");


  // Activate Radio
  radio.begin();                  // Ativate the modem
  radio.setPALevel(RF24_PA_MIN);  // Set the PA Level low to prevent power supply related issues
                                  // since this is a getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setDataRate(RF24_1MBPS);  // choosing 1 Mega bit per second radio frequency data rate
                                  // radio frequency data rate choices are:  //RF24_250KBPS    //RF24_2MBPS  //RF24_1MBPS
  radio.setChannel(rf24_channel[AAAD_ARO]);
  radio.setPayloadSize(RF24_PAYLOAD_SIZE);
  radio.openWritingPipe(addresses[AAAD_MODULE]);
  radio.openReadingPipe(1, addresses[AAAD_MODULE]);

  // Start the radio listening for data
  radio.startListening();

  sensors.begin();
  pinMode(3, INPUT_PULLUP);  // Assuming CAPTURE_TEMP is pin 13
}
// Loop
void loop() {
  // check to see if it's time to change the state of the LED
  currentMillis = millis();

  // Check if there are incoming bytes in the serial buffer
  if (Serial.available() >= 32) {
    // Create a buffer to store the incoming data
    uint8_t buffer[32];

    // Read 32 bytes from the serial buffer
    Serial.readBytes(buffer, 32);

    // Extract the values of arm1 and arm2
    arm1 = buffer[0];
    arm2 = buffer[1];

    // Print the values for debugging
    Serial.print("arm1: ");
    Serial.println(arm1);
    Serial.print("arm2: ");
    Serial.println(arm2);
  }

  if (Serial.available() >= 32) {
    // Create a buffer to store the incoming data
    uint8_t buffer[32];

    // Read 32 bytes from the serial buffer
    Serial.readBytes(buffer, 32);


    // Extract the values of arm1 and arm2
    arm1 = buffer[0];
    arm2 = buffer[1];

    // Print the values for debugging
    Serial.print("arm1: ");
    Serial.println(arm1);
    Serial.print("arm2: ");
    Serial.println(arm2);
  }
  if (currentMillis - previousMillis >= tempRequestMillis) {

    unsigned long timeStamp = millis() / 1000;
    uint8_t cursor = 0;
    txData[cursor++] = timeStamp >> 24;
    txData[cursor++] = timeStamp >> 16;
    txData[cursor++] = timeStamp >> 8;
    txData[cursor++] = timeStamp;
    while (cursor < RF24_PAYLOAD_SIZE) {
      txData[cursor++] = 0;
    }

    /****************** Transmit Mode ***************************/

    // Print transmit data in Hex format


    // Serial.print("txData: ");
    // for (size_t i=0; i<cursor; ++i) {
    //   if (i != 0) Serial.print(" ");
    //   printHex2(txData[i]);
    // }
    // Serial.println();

    radio.stopListening();  // First, stop listening so we can talk.
    // Serial.println(F("Now Sending"));

    // Transmit data to radio
    // radio.write(&txData, sizeof(txData));

    radio.startListening();  // Now, continue listening
    // Serial.println(F("Now Listing"));

    previousMillis = currentMillis;
  }

  /****************** Receive Mode ***************************/

  if (radio.available()) {  //'available' means whether valid bytes have been received and are waiting to be read from the receive buffer
    // Receive data from radio
    while (radio.available()) {             // While there is data ready
      radio.read(&rxData, sizeof(rxData));  // Get the payload
    }
    // Print received data in Hex format
    Serial.print("rxData: ");
    for (size_t i = 0; i < RF24_PAYLOAD_SIZE; ++i) {
      if (i != 0) Serial.print(" ");
      Serial.print(rxData[i]);
    }
    Serial.println();

    // Switch led on Received command
    if (rxData[0] == 0xFF) {
      Serial.println("Led=on");
      // led.setState(HIGH);
    }
    if (rxData[0] == 0x7F) {
      Serial.println("Led=off");
      // led.setState(LOW);
    }
  }



  // tempratuur lees

  byte i;
  byte present = 0;
  byte type_s;
  byte data[9];
  byte addr[8];
  float celsius, fahrenheit;

  if (!ds.search(addr)) {
    // Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  // // Serial.print("ROM =");
  // for (i = 0; i < 8; i++) {
  //   Serial.write(' ');
  //   Serial.print(addr[i], HEX);
  // }

  // if (OneWire::crc8(addr, 7) != addr[7]) {
  //   Serial.println("CRC is not valid!");
  //   return;
  // }
  // Serial.println();

  // the first ROM byte indicates which chip
  // switch (addr[0]) {
  //   case 0x10:
  //     Serial.println("  Chip = DS18S20");  // or old DS1820
  //     type_s = 1;
  //     break;
  //   case 0x28:
  //     Serial.println("  Chip = DS18B20");
  //     type_s = 0;
  //     break;
  //   case 0x22:
  //     Serial.println("  Chip = DS1822");
  //     type_s = 0;
  //     break;
  //   default:
  //     Serial.println("Device is not a DS18x20 family device.");
  //     return;
  // }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);  // start conversion, with parasite power on at the end

  delay(1000);  // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);  // Read Scratchpad

  // Serial.print("  Data = ");
  // Serial.print(present, HEX);
  // Serial.print(" ");
  // for (i = 0; i < 9; i++) {  // we need 9 bytes
  //   data[i] = ds.read();
  //   Serial.print(data[i], HEX);
  //   Serial.print(" ");
  // }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3;  // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;       // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3;  // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1;  // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius   ");
  Serial.print("  send");

  sendTemperature(celsius);

  // Serial.print(fahrenheit);
  // Serial.println(" Fahrenheit");
}

void sendTemperature(float temperature) {

  temperature = 19.5;
  uint8_t cursor = 0;

  // Scale the float temperature to an int16
  int16_t scaledTemp = static_cast<int16_t>(temperature * 100);

  // Convert int16 to bytes and store in txData
  txData[cursor++] = (scaledTemp >> 8) & 0xFF;
  txData[cursor++] = scaledTemp & 0xFF;

  // Fill the rest of txData with zeros
  while (cursor < RF24_PAYLOAD_SIZE) {
    txData[cursor++] = 0;
  }

  Serial.print("txData: ");
  for (size_t i = 0; i < cursor; ++i) {
    if (i != 0) Serial.print(" ");
    Serial.print(txData[i], HEX);
  }
  Serial.println();

  radio.stopListening();  // First, stop listening so we can talk.
  // Serial.println(F("Now Sending"));

  // Transmit data to radio
  radio.write(&txData, sizeof(txData));

  radio.startListening();  // Now, continue listening
  // Serial.println(F("Now Listening"));

  previousMillis = currentMillis;
  // uint8_t cursor = 0;


}