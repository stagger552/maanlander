#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

// #include "Led.h"
#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>  // to handle this particular modem driver
#include "RF24.h"      // the library which helps us to control the radio modem

// Initialise Sensors

// Initialise Actuators
// Led led;
int ledState = LOW;  // ledState used to set the LED

#define RF24_PAYLOAD_SIZE 32
#define AAAD_ARO 1
#define AAAD_MODULE 3

#define ONE_WIRE_BUS 3  // Data wire is plugged into digital pin 2 on the Arduino



/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10, which are CE & CSN pins  */
RF24 radio(9, 10);
const uint8_t rf24_channel[] = { 1, 26, 51, 76, 101 };                                                            // Radio channels set depending on satellite number
const uint64_t addresses[] = { 0x4141414430LL, 0x4141414431LL, 0x4141414432LL, 0x4141414433LL, 0x4141414434LL };  //with radioNumber set to zero, the tx pipe will be 'AAAD0', which is basically HEX'4141414430', which is remote DESTINATION address for our transmitted data. The rx pipe code is the local receive address, which is what the remote device needs to set for the remote devices 'tx' pipe code.
uint8_t txData[RF24_PAYLOAD_SIZE];
uint8_t rxData[RF24_PAYLOAD_SIZE];


// tempratuur en servo
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

#define ONE_WIRE_BUS 2                // Data wire is plugged into digital pin 2 on the Arduino
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&oneWire);  // Pass oneWire reference to DallasTemperature library

Servo myservo;

// Define stepper motor connections:
#define dirPin 4
#define stepPin 5

// Main pinout
int BUTTON_SERVO_UP = 8;
int BUTTON_SERVO_DOWN = 9;
int BUTTON_STEPPER_UP = 10;
int BUTTON_STEPPER_DOWN = 11;

int CAPTURE_TEMP = 13;

// Function Prototypes
void requestTemperature();
void readTemperature();

// void controlServo();
// void stepperMotor();
// void rotateMotor();

int servo_position = 55;
unsigned long previousMillis = 0;
unsigned long servoMillis = 0;
unsigned long tempRequestMillis = 0;
const unsigned long TEMP_INTERVAL = 1000;        // Interval to request temperature
const unsigned long TEMP_CONVERSION_TIME = 750;  // Time it takes for temperature conversion
// currentmiles
unsigned long currentMillis;

bool tempRequestPending = false;

OneWire ds(3);  // on pin 10 (a 4.7K resistor is necessary)

void setup(void) {
  // Setup temp sensor
  sensors.begin();
  pinMode(CAPTURE_TEMP, INPUT_PULLUP);

  // Setup servo
  myservo.attach(7);  // attaches the servo on pin 7 to the servo object

  //  servo test


  // pinMode(BUTTON_SERVO_UP, INPUT_PULLUP);
  // pinMode(BUTTON_SERVO_DOWN, INPUT_PULLUP);

  // Setup Stepper motor
  // pinMode(BUTTON_STEPPER_UP, INPUT_PULLUP);
  // pinMode(BUTTON_STEPPER_DOWN, INPUT_PULLUP);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);  // Set the spinning direction CW/CCW

  // Setup serial port
  Serial.begin(9600);


  // actieveer radio
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

  //Test Servo:
  // TestServo();
  delayMicroseconds(1000);

  // rotateMotor(1500, 1);

  rotateMotor(1900, 1);





  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  // rotateMotor(50);
  //Test Steppenmotor:

  // TestStepmotor();
}

void loop(void) {
  unsigned long currentMillis = millis();

  if (!digitalRead(CAPTURE_TEMP)) {
    if (currentMillis - tempRequestMillis >= TEMP_INTERVAL) {
      tempRequestMillis = currentMillis;
      requestTemperature();
    }

    if (tempRequestPending && currentMillis - tempRequestMillis >= TEMP_CONVERSION_TIME) {
      readTemperature();
      tempRequestPending = false;
    }
  }


  // controlServo();
  // stepperMotor();


  // check to see if it's time to change the state of the LED
  currentMillis = millis();



  if (currentMillis - previousMillis >= 1000) {

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

    radio.startListening();
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
    // Serial.print("rxData: ");
    for (size_t i = 0; i < RF24_PAYLOAD_SIZE; ++i) {
      if (i != 0) Serial.print(" ");

      // Serial.print(rxData[i]);

      int Arm1 = rxData[0];
      int Arm2 = rxData[1];

      // Serial.print("Arm 1: ");
      // Serial.print(Arm1);
      // Serial.print(" Arm 2: ");
      // Serial.print(Arm2);

      // Call moveArm() with Arm1 and Arm2 as parameters
      moveArm(Arm1, Arm2);
      delayMicroseconds(3000);
    }

    Serial.println();
  } else {
    // Serial.print("Niks");
  }
}

void requestTemperature() {
  // Send the command to get temperatures
  sensors.requestTemperatures();
  tempRequestPending = true;
}

void readTemperature() {
  // Print the temperature in Celsius

  // Serial.print("Temperature: ");
  // Serial.print(sensors.getTempCByIndex(0));
  // Serial.println("C");

  sendTemperature(sensors.getTempCByIndex(0));
}

// void controlServo() {
//   if (millis() - servoMillis >= 15) {  // Non-blocking delay for the servo
//     servoMillis = millis();

//     if (!digitalRead(BUTTON_SERVO_UP) && servo_position < 175) {
//       servo_position++;
//     } else if (!digitalRead(BUTTON_SERVO_DOWN) && servo_position > 0) {
//       servo_position--;
//     }

//     myservo.write(servo_position);
//   }
// }

// void stepperMotor() {
//   if (!digitalRead(BUTTON_STEPPER_UP)) {
//     digitalWrite(dirPin, HIGH);  // sets the direction
//     rotateMotor();
//   } else if (!digitalRead(BUTTON_STEPPER_DOWN)) {
//     digitalWrite(dirPin, LOW);  // sets the direction
//     rotateMotor();
//   }
// }

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
void moveArm(int Arm1, int servoDegrees) {

  // Move stepper motor
  if (Arm1 >= 0 && Arm1 <= 50) {
    // rotateMotor(Arm1);

  } else {
    Serial.println("Niks Motor");
  }
  // Move servo motor
  if (servoDegrees > 0 && servoDegrees <= 180) {
    Serial.print("Arm2 wordt bewogen naar: " + servoDegrees);
    delayMicroseconds(1000);

    myservo.write(servoDegrees);
  } else {
    // Serial.println("Servo angle out of range (0-180 degrees).");
  }
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
  myservo.write(100);
  delay(1000);
  checkServoPosition(100);

  // Move to position 140 and check
  myservo.write(140);
  delay(1000);
  checkServoPosition(140);

  // Move to position 180 and check
  myservo.write(180);
  delay(1000);
  checkServoPosition(180);

  // Move back to position 140 and check
  myservo.write(140);
  delay(1000);
  checkServoPosition(140);

  // Move back to position 100 and check
  myservo.write(100);
  delay(1000);
  checkServoPosition(100);

  // Move back to position 0 and check
  myservo.write(0);
  delay(1000);
  checkServoPosition(0);

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
// void TestStepmotor() {
//   Serial.print("Stappenmotor test start >>>>");
//   rotateMotor(0);
//   delayMicroseconds(1000);
//   rotateMotor(10);
//   delayMicroseconds(1000);
//   rotateMotor(30);
//   delayMicroseconds(1000);
//   rotateMotor(50);
//   delayMicroseconds(1000);
//   rotateMotor(30);
//   delayMicroseconds(1000);
//   rotateMotor(10);
//   delayMicroseconds(1000);
//   Serial.print("Stappenmotor test Skaal >>>>>");
// }
void sendServo() {
}

void sendStepmotor() {
}
