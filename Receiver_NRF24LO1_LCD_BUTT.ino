/*
  Stanislav Stanchev
  Final Project ECE387
  Home Control System using Wirless Communication
  The chips used are nRF24LO1 transcievers and Arduino Uno R3
  Library: TMRh20/RF24
* *****This is the Receiver Code*******

  Currently the receiver receves and Sends character arrays that represent the states of the
  LEDs or the state of Buttons....
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//Transciver initialized with chip enabled and chip select pins.
RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};//the address associated with each radio
//For more units more addrresses needs to be
//added up to six per channel.


// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 3;

//Message received from Transmitter with state of the lights
unsigned char states[MAX_INPUT] = {};



//Message to be send by the radio
unsigned char instructions[MAX_INPUT] = {};


//Keep track of time for debbugging purposes.
unsigned long currentMillis = millis(); // Start timing for debbuging purposes
int printInterval = 3000;
boolean received = false;

//Timer that willalternate between sending and receiving data trough the serial.
const unsigned long timeOut = 20;
unsigned long TxRx;

void setup() {

  //Using the serial monitor to connect to the LCD unit.
  Serial.begin(9600);
  //Start the time
  TxRx = millis();

  //Open communication line
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(124);
}



void loop() {
  while(radio.available())
{
  //Establishing a receiving connection.
  delay(5);
  radio.startListening();

  radio.read(&states, sizeof(states));

  //Radio stops listening an sends the data recieved
  radio.stopListening();
}

delay(5);
  //Receiver sends back instructions
  if (!radio.write(&instructions, sizeof(instructions)))
  {
    Serial.println("No acknoledgement of transmission");
  }



  //Time reducing print instructions used to trublshoot the sending and receiving payloads.
  //Timing variables
  if ((millis() - currentMillis) >= printInterval) { // enough time passed yet?

    // capture & print data, or print latest data, etc.
    Serial.print("RX_Received: ");

    for (int i = 0; i < MAX_INPUT; i++) {
      Serial.print(states[i]);
    }

    Serial.println("");

    Serial.print("RX_Send: ");
    for (int i = 0; i < MAX_INPUT; i++) {
      Serial.print(instructions[i]);
    }
    Serial.println("");
    currentMillis = millis();
  }

}



