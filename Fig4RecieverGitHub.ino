#include <FastLED.h>  //include fast led library
#include <nRF24L01.h> //include RF library
#include <RF24.h>  //Include RF Library
#include <RF24_config.h>  //Include RF library
#include <SPI.h> //Include SPI library


#define NumLeds 15 //Set how many LEDs are on the reciever LED Strips
#define DataPin22 22  //Sets the LED digital pin being used for the station, You will need 1 per transmitter station

CRGB leds22[NumLeds]; //declare a variable for LED, One will be needed for each transmitter station

RF24 radio(9,10); //ce, csn
const byte address[6] = "00001"; //Sets the RF channel

int TwoNum = 0; //Sets a variable for declaring if the recieved transmitter is a single or 2 digit number
int ThreeNum = 0; //Sets a variable for declaring if the recieved transmitter is 2 or 3 digit station number

void setup() {
  // put your setup code here, to run once:
Serial.begin (9600); //Sets the baud rate
FastLED.addLeds<NEOPIXEL, DataPin22>(leds22, NumLeds); //Attached properties to the Fast LED variable created before, will need one per transmitter
 
radio.begin(); //Turns the radio on
radio.openReadingPipe(0, address); //opens the RF reading pipe
radio.setPALevel(RF24_PA_HIGH); //Sets the RF antenna power level
radio.startListening(); //Sets the rf to begin listening


//leds
pinMode(13, OUTPUT); 
digitalWrite(13, LOW); 

}

void loop() {
  // put your main code here, to run repeatedly:


if (radio.available()){ //if statement to skip all code if the radio is unavilable
int value = ""; //Sets a variable called value as nothing, will be used when reading transmitters
radio.read(&value, sizeof(value)); //Reads the transmitted value

//Check for station
//Get total number length
if (value >= 0 && value <= 99) {  //If statement to determine if the station is a single digit or two
  TwoNum = 1; //Sets the variable twonum to 1 so it is read correctly laters
  ThreeNum = 0;  //Sets the variable three num to 0 so it is not read incorrectly
}else if(value >= 100 && value <= 999){ //If statement to determine if the station is two or three digits lonf\g
  ThreeNum = 1; //Sets the variable three num to 1 so it is read correctly later
  TwoNum = 0; //Sets the variable two num to 0 so it is read correctly later
}else{ //end of if statement
  TwoNum =1;  //should result in error
  ThreeNum=1; //should result in error
}

//parse 2 num
if (TwoNum == 1 && ThreeNum == 0){ //If the code is set to read a 2 digit station this code runs
  unsigned a = value; //assiging the variable a with value
  unsigned q = (value / 10U) % 10; //getting the tens number, example it would grab 3 in the station number 34
  unsigned w = (value / 1U) % 10; //getting the singles number, example it would grab 2 in the station number 12

//-----Station 1
if(q == 1 && w ==1){ //This is reading as station 1 and green light, signified by the number 1
  Station1Green(); //runs code at the bottom for the private void to turn on the green leds
  Serial.println("1 G"); //This line of code is what will be sent to a computer program to save/store data
}else if(q==1 && w==2){ //This is reading as station 1 and yellow light, signified by the number 2
  Station1Yellow(); //runs code to turn the leds yellow
  Serial.println("1 Y"); //This line of code is what will signal a computer to store data
}else if(q==1 && w==3){ //This is reading as station 1 and red light, signified by the number 3
  Station1Red(); //this runs code from the bottom to turn the leds to red
  Serial.println("1 R"); //this line of code is what will signal a computer to store data
}else if(q==1 && w==0){ //this signifies station 1 lights off, signified by the number 0
  Station1Off(); //runs code to turn the leds off on the reciever
  Serial.println("1 O"); //This line of code is what will signal the code to the computer to store data
}else if(q==1 && w==4){ //This represents station 1 with error lights, purple error lights are represented by 4
  Station1Purple(); //This code turns on purple lights for the reciever
  Serial.println("1 P"); //This code tells the computer program to store data properly
}


}else if (TwoNum == 0 && ThreeNum == 1){ //This section is for stations with multiple numbers, stations 11 to 999
  unsigned r = value; //assigns the variable value to the variable r
  unsigned z = (value / 100U) % 10; //these variables grab specific letters in the variable value.
  unsigned x = (value / 10U) % 10;
  unsigned c = (value / 1U) % 10;

}
}
}


//-----Station 1---
void Station1Green(){ //turns on station 1 green lights
  //leds 22
  for(int i =0; i<= 25; i++){
    leds22[i] = CRGB::Green;
  }
  FastLED.show();
}

void Station1Yellow(){ //turns on station 2 yellow lights
  for(int i =0; i<= 25; i++){
    leds22[i] = CRGB::Yellow;
  }
  FastLED.show();
}

void Station1Red(){ //turns on station 1 red lights
  for (int i=0; i<= 25; i++){
    leds22[i] = CRGB::Red;
  }
  FastLED.show();
}

void Station1Off(){ //turns off station 1 lights
  for(int i =0; i<= 25; i++){
    leds22[i] = CRGB::Black;
  }
  FastLED.show();
}

void Station1Purple(){ //turns purple lights on for station 1
  for(int i=0; i<= 25; i++){
    leds22[i] = CRGB::Purple;
  }
  FastLED.show();
}
