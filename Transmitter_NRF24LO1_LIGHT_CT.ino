/*
  Stanislav Stanchev
  Final Project ECE387
  Home Control System using Wirless Communication
  The units used are nRF24LO1 transcievers and Arduino Uno R3
  Library: TMRh20/RF24
* *****This is the Transmitter Code*******
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


int light1 = 10;
int stateLight = 0;

int sizePayload = 3;

//Transciver initialized with chip enabled and chip select pins.
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};//the address associated with each radio
//For more units more addrresses needs to be
//added up to six per channel.

//Timing variables
unsigned long previousMillis = 0;
long printInterval = 3000;

void setup() {


  Serial.begin(9600); // Debugging only
  Serial.println("This is the Transmitter code.");
  pinMode(light1, OUTPUT);

  radio.begin();
  //Open reading and writing connections using the addres of the two units
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002

  //Set the power level. Currently at the lowest setting.
  radio.setPALevel(RF24_PA_MIN);

  //Set the Data rate. It is set to the lowest possible rate this units are going to produce.
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(124);

}

void loop() {

  digitalWrite(light1, HIGH);

  delay(5);
  //Message containing the state of the lights that is going to be send to the receiver.
  unsigned char message[sizePayload] = {(char)stateLight, 0, 0};
  stateLight = digitalRead(light1);


  //Sends the message
  radio.stopListening();
  //Sending the message
  if (!radio.write(&message, sizeof(message))) {
    Serial.println("No acknoledgement of transmission");
  }
  delay(5);

  //Starting to listen for instructions.
  radio.startListening();

  //Will not listen forever. If there is no message for 200 ms will notify with a message.
  unsigned long startListenAt = millis();
  while (!radio.available()) {
    if (millis() - startListenAt > 200) {
      Serial.println("No respnse");
      return;
    }
  }

  //Receiving instructions
  unsigned char instruction[sizePayload] = {};
  radio.read(&instruction, sizeof(instruction));

  if (instruction[0] == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }


  //If the lights are on, turn them off. If they are off turn them on.
  //for (int j = 0; j < sizePayload; j++) {
  if (instruction[0] == 1 && message[0] == 1)
  {
    stateLight = LOW;
  } else {
    stateLight = HIGH;
  }
  digitalWrite(light1, stateLight);


  //Time reducing print instructions used to trublshoot the sending and receiving payloads.
  unsigned long currentMillis = millis(); // Start timing for debbuging purposes
  if ((currentMillis - previousMillis) >= printInterval) // enough time passed yet?
  {
    previousMillis = previousMillis + printInterval; // yes, so set time for the next print event
    // capture & print data, or print latest data, etc.
    Serial.println("TX_Sent:");
    for (int i = 0; i < sizePayload; i++) {
      Serial.print(message[i]);
    }
    Serial.println("");
    Serial.println("TX_received:");
    for (int i = 0; i < sizePayload; i++) {
      Serial.print(instruction[i]);
    }
    Serial.println("");
  }
}
