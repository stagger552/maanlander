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
#include <Servo.h>


Servo myservo;

// Define stepper motor connections:
#define dirPin 4
#define stepPin 5
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


/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10, which are CE & CSN pins  */
RF24 radio(9, 10);
const uint8_t rf24_channel[] = { 1, 26, 51, 76, 101 };                                                            // Radio channels set depending on satellite number
const uint64_t addresses[] = { 0x4141414430LL, 0x4141414431LL, 0x4141414432LL, 0x4141414433LL, 0x4141414434LL };  //with radioNumber set to zero, the tx pipe will be 'AAAD0', which is basically HEX'4141414430', which is remote DESTINATION address for our transmitted data. The rx pipe code is the local receive address, which is what the remote device needs to set for the remote devices 'tx' pipe code.
uint8_t txData[RF24_PAYLOAD_SIZE];
uint8_t rxData[RF24_PAYLOAD_SIZE];

// Timing configuration

int servo_position = 55;
unsigned long previousMillis = 0;
unsigned long servoMillis = 0;
unsigned long tempRequestMillis = 0;
const unsigned long TEMP_INTERVAL = 2000;        // Interval to request temperature
const unsigned long TEMP_CONVERSION_TIME = 750;  // Time it takes for temperature conversion
// currentmiles
unsigned long currentMillis;

bool tempRequestPending = false;


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

  // Setup sensors and pins
  sensors.begin();
  pinMode(3, INPUT_PULLUP);  // Assuming CAPTURE_TEMP is pin 13

  // Setup servo and stepper motor
  setupServo();
  // Test servo and stepper motor
  // TestServo();



  // TestStepmotor();
}

void setupServo() {
  myservo.attach(7);  // attaches the servo on pin 7 to the servo object
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);  // Set the spinning direction CW/CCW
}
// Loop
void loop() {
  // check to see if it's time to change the state of the LED
  currentMillis = millis();


  if (currentMillis - previousMillis >= tempRequestMillis) {

    requestTemperature();




    unsigned long timeStamp = millis() / 1000;
    uint8_t cursor = 0;
    txData[cursor++] = timeStamp >> 24;
    txData[cursor++] = timeStamp >> 16;
    txData[cursor++] = timeStamp >> 8;
    txData[cursor++] = timeStamp;

    // float tempratuur = sensors.getTempCByIndex(0);
    // int16_t scaledTemp = static_cast<int16_t>(tempratuur * 100);

    // // Convert int16 to bytes and store in txData
    // txData[cursor++] = (scaledTemp >> 8) & 0xFF;
    // txData[cursor++] = scaledTemp & 0xFF;

    // // Assuming `Graden` is an integer servo angle value
    // int Graden = myservo.read();
    // txData[cursor++] = Graden >> 8;
    // txData[cursor++] = Graden & 0xFF;

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
    radio.write(&txData, sizeof(txData));

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

    uint8_t value1 = rxData[0];
    uint8_t value2 = rxData[1];

    // Print the extracted values
    Serial.print("Value 1: ");
    Serial.println(value1);
    Serial.print("Value 2: ");
    Serial.println(value2);

    moveArm(value1, value2);
  }



  // tempratuur lees

  // byte i;
  // byte present = 0;
  // byte type_s;
  // byte data[9];
  // byte addr[8];
  // float celsius, fahrenheit;

  // if (!ds.search(addr)) {
  //   // Serial.println("No more addresses.");
  //   Serial.println();
  //   ds.reset_search();
  //   delay(250);
  //   return;
  // }


  // ds.reset();
  // ds.select(addr);
  // ds.write(0x44, 1);  // start conversion, with parasite power on at the end

  // delay(1000);  // maybe 750ms is enough, maybe not
  // // we might do a ds.depower() here, but the reset will take care of it.

  // present = ds.reset();
  // ds.select(addr);
  // ds.write(0xBE);  // Read Scratchpad





  // int16_t raw = (data[1] << 8) | data[0];
  // if (type_s) {
  //   raw = raw << 3;  // 9 bit resolution default
  //   if (data[7] == 0x10) {
  //     // "count remain" gives full 12 bit resolution
  //     raw = (raw & 0xFFF0) + 12 - data[6];
  //   }
  // } else {
  //   byte cfg = (data[4] & 0x60);
  //   // at lower res, the low bits are undefined, so let's zero them
  //   if (cfg == 0x00) raw = raw & ~7;       // 9 bit resolution, 93.75 ms
  //   else if (cfg == 0x20) raw = raw & ~3;  // 10 bit res, 187.5 ms
  //   else if (cfg == 0x40) raw = raw & ~1;  // 11 bit res, 375 ms
  //   //// default is 12 bit resolution, 750 ms conversion time
  // }
  // celsius = (float)raw / 16.0;
  // fahrenheit = celsius * 1.8 + 32.0;
  // Serial.print("  Temperature = ");
  // Serial.print(celsius);
  // Serial.print(" Celsius   ");
  // Serial.print("  send");

  // sendTemperature(celsius);

  // Serial.print(fahrenheit);
  // Serial.println(" Fahrenheit");
}


void rotateMotor(int numSteps, int Directie) {

  if (Directie == 1) {
    digitalWrite(dirPin, HIGH);  // Set the direction
  } else if (Directie == 0) {
    digitalWrite(dirPin, LOW);  // Set the direction
  }
  for (int i = 0; i < numSteps; i++) {
    Serial.print("Roatating");
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);  // Adjust the pulse width as needed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}
void moveArm(uint8_t Arm1, uint8_t servoDegrees) {

  // Move stepper motor
  if (Arm1 == 1) {
    rotateMotor(300, 0);
  } else if (Arm1 == 2) {
    Serial.print("Niks");
  } else if (Arm1 == 3) {
    // rotateMotor(Arm1);
    rotateMotor(300, 1);
  } else {

    Serial.print("Error");
  }

  Serial.print("servo: " + servoDegrees);

  int Arm2Degree = servoDegrees;
  // Move servo motor
  if (Arm2Degree >= 80 && Arm2Degree <= 180) {

    // setupServo();

    Serial.print("Arm2 wordt bewogen naar: " + Arm2Degree);
    delayMicroseconds(1000);

    myservo.write(Arm2Degree);
  } else {
    Serial.println("Servo angle out of range (0-180 degrees).");
  }
}
void sendServo() {
  int actualPos = myservo.read();
}
void sendTemperature(float temperature) {

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
  // Serial.print("txData: ");
  // for (size_t i = 0; i < cursor; ++i) {
  //   if (i != 0) Serial.print(" ");
  //   Serial.print(txData[i], HEX);
  // }

  Serial.println();
  radio.stopListening();  // First, stop listening so we can talk.
  // Transmit data to radio
  radio.write(&txData, sizeof(txData));
  radio.startListening();  // Now, continue listening
}
void TestServo() {
  Serial.println("Servo test begin <<<<<");

  // Move to position 0 and check
  myservo.write(0);
  delay(1000);  // Wait for 1 second
  checkServoPosition(0);

  // Move to position 100 and check

  // Move to position 140 and check
  myservo.write(140);
  delay(1000);
  checkServoPosition(140);

  // Move to position 180 and check
  myservo.write(180);
  delay(1000);
  checkServoPosition(180);

  // Move back to position 0 and check
  myservo.write(0);
  delay(1000);
  checkServoPosition(0);

  myservo.write(75);

  Serial.println("Servo test Klaar >>>>>");
}

void checkServoPosition(int expectedPos) {
  int actualPos = myservo.read();
  if (actualPos == expectedPos) {
    Serial.print("Servo reached position ");
    Serial.println(expectedPos);
  } else {
    Serial.print("Error: Servo did not reach position ");
    Serial.print(expectedPos);
    Serial.print(". Current position is ");
    Serial.println(actualPos);
  }
}
void requestTemperature() {
  // Send the command to get temperatures
  sensors.requestTemperatures();
  tempRequestPending = true;

  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println("C");

  sendTemperature(sensors.getTempCByIndex(0));
}
void TestStepmotor() {
  rotateMotor(1500, 1);
  rotateMotor(1500, 0);
}
