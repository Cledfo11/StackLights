#include <FastLED.h>  //Includes the FastLED Library
#include <SPI.h>  //Includes the SPI Library
#include <nRF24L01.h>  //Includes the NRF240L01 Library
#include <RF24.h>  //Includes the RF24 library
#define BRIGHTNESS  50  //Sets LED Brightness
#define NumLeds 25  //Sets the number of LEDs on each data pin
#define DataPinGreen 2  //Sets the Green LED strip data pin to digital pin 2 (Arduino)
#define DataPinYellow 3  //Sets the Yellow LED strip data pin to digital pin 3 (Arduino)
#define DataPinRed 4  //Sets the Red LED Strip data pin to digital pin 4 (Arduino)

CRGB ledsGreen[NumLeds];  //Sets the CRGB variable ledsGreen with the amound of LEDS defined by "NumLeds".
CRGB ledsYellow[NumLeds];  //Sets the CRGB variable ledsYellow with the amound of LEDS defined by "NumLeds".
CRGB ledsRed[NumLeds];  //Sets the CRGB variable ledsRed with the amound of LEDS defined by "NumLeds".

RF24 radio(7, 8);  //Sets the digital pins for ce and csn to Arduino pins 7 and 8 respectively

const byte address[6] = "00001";  //Sets the address channel for the RF transmitter/reciever. This channel must match between the reciever and transmitter.

int TotalLights = 0;  //Sets a variable called Total Lights to detect if more than one light is turned on

//Last/New light replacement detection delay
//The variables LastLight and NewLight are used to detected if something changed, if they do not match.
//For LastLight and NewLight 1= green, 2 = yellow, 3 = red, 4 = off, 5 = purple, 6 & 7 are place holders
int LastLight = 6; //Sets an integer variable based on what the last light detected was
int NewLight = 7;  //sets an integer variable based on what the current light detected is
 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); // Set baud rate to 9600

radio.begin();  //begin RF Module
radio.openWritingPipe(address);  //Open the RF Module writing pipe with variable address previously created
radio.setPALevel(RF24_PA_MAX);  //Set the RF power level
radio.stopListening();  //Turn off the RF listening, this is the transmitter so it will only send

FastLED.addLeds<NEOPIXEL, DataPinGreen>(ledsGreen, NumLeds);  //Tie FastLed variable to the data pin for green
FastLED.addLeds<NEOPIXEL, DataPinYellow>(ledsYellow, NumLeds);  //Tie FastLed variable to the data pin for Yellow
FastLED.addLeds<NEOPIXEL, DataPinRed>(ledsRed, NumLeds);  //Tie FastLed variable to the data pin for Red
FastLED.setBrightness(  BRIGHTNESS ); //Set the FastLED Brightness with the variable brightness previously created

pinMode(5, INPUT_PULLUP);  //Set up the green switch as a data input for digital pin 5 on the Arduino.
pinMode(6, INPUT_PULLUP);  //Set up the yellow switch as a data input for digital pin 6 on the Arduino.
pinMode(7, INPUT_PULLUP);  //Set up the red switch as a data input for digital pin 7 on the Arduino.
}  //End set up code

void loop() {
  // put your main code here, to run repeatedly:

//The following code reads which switches are turned on and applies a reading to the variable NewLight
if(digitalRead(5) == 1 && digitalRead(6) == 0 && digitalRead(7) == 0){ //This if statement is for the green light switch on only
  NewLight = 1; //set NewLight variable to 1, green
}
if(digitalRead(6) == 1 && digitalRead(5) == 0 && digitalRead(7) == 0){ //This if statement is for the yellow light switch on only
  NewLight = 2;  //Set NewLight variable to 2, yellow
}
if(digitalRead(7) == 1 && digitalRead(6) == 0 && digitalRead(5) == 0){ //This if statement is for the red light switch on only
  NewLight = 3; //Set NewLight variable to 3, red
}
if(digitalRead(7) == 0 && digitalRead(6) == 0 && digitalRead(5) == 0){ //This if statement is for tall switches being turned off
  NewLight = 4;  //Set NewLight variable to 4, off
}
TotalLights = digitalRead(5) + digitalRead(6) + digitalRead(7); //This light adds all the switches together, 1 for each on that is on, then saves that total to a variable called TotalLights
if (TotalLights > 1){ //If the variable TotalLights is greater than 1 it means more than one switch is pressed
  NewLight = 5;  //Sets the variable NewLight to 5, purple, or error
} //end if statement
Serial.println(NewLight); //Prints the NewLight variable, used for debugging
Serial.println(LastLight);  //Prints the LastLight variable, used for debugging.


//The following if statement is comparing if the previously recorded light matches the new recorded light.
//If the previous lights match the new reading the Arduino re-reads the switches
//If the new reading is different than the last reading the Arduino moves into the "else" portion
if(LastLight == NewLight){
  //do nothing
  Serial.println("DoNothing"); //if no detected switch change then print "do nothing" for debugging.
}else{

TotalLights = digitalRead(5) + digitalRead(6) + digitalRead(7); //Add together all switches that are turned on, 1 for each/
Serial.println(TotalLights); //Print the total lights turned on, used for debugging

if(TotalLights <= 1){ //if TotalLights is less than or equal to 1, so no more than 1 switch turned on then continue.

if(digitalRead(5) == 1 && digitalRead(6) == 0 && digitalRead(7) == 0){  //If statement to catch if just the green switch is on
  int value = 21; //This value is a custom number for my program, it means station 2, green light 1, so 21
  radio.write(&value, sizeof(value)); //Sends the previous value variable over RF to the reciever
  Serial.println("Green"); //Prints the word Green to the serial comm, for debugging
  TurnOnGreen(); //Run a void from the bottom of the program to turn on the green lights and turn off all others.
}

if(digitalRead(6) == 1 && digitalRead(5) == 0 && digitalRead(7) == 0){ //If statement to catch for the yellow button turned on and all other switches are off
  int value1 = 22; // This value is a custom number for my program, it means station 2, yellow light 2, so 22
  radio.write(&value1, sizeof(value1)); //Sends the previous variable value1 over RF to the reciever
  Serial.println("Yellow"); //Prints the word yello in the serial comm, for debugging
  TurnOnYellow(); //Run a void from the bottom of the program code to turn on the yellow lights and off all others
}

if(digitalRead(7) == 1 && digitalRead(6) == 0 && digitalRead(5) == 0){ //If statement to catch for the red light button turned on and all other switches off
  int value2 = 23; //This is a custom number for my program, it means station 2, Red Light 3, so 23
  radio.write(&value2, sizeof(value2)); //Sends the previous variable, value2, over RF to the reciever
  Serial.println("Red"); //Prints the word Red into the serial comm, for debugging
  TurnOnRed(); //Run a void from the bottom of the program code to turn on the red lights and off all others
}

if(digitalRead(7) == 0 && digitalRead(6) == 0 && digitalRead(5) == 0){  //If statement to catch if all switches are turned off
  int value3 = 20; //This is a custom number for my program, it means station 2, all lights off 0, so 20
  radio.write(&value3, sizeof(value3)); //Sends the previous variable, value3, over the RF to the reciever
  Serial.println("Off"); //Prints the word "Off" to the serial comm, for debugging
  TurnOnOff(); //Run a void from the bottom of the program code to turn off all lights.
}
}else{  //begin else statement if total lights was >1, so more than 1 switch turned on

  Serial.println(TotalLights); //Prints the variable total lights to the serial comm, for debugging
  Serial.print("---------------------"); //Prints a line of -s to serial comm for debugging
  Serial.println("Error"); //Prints the word error to the serial comm for debugging
  int value4 = 24; //This is a custon number for my program, it means station 2, error code 4, so 24
  radio.write(&value4, sizeof(value4)); //Writes the previous variable, value4, over RF to the reciever
  TurnOnPurple(); //Runs a void from the bottom of the code to turn on all LEDs to the color purple.
}

TotalLights = 0; //Sets the variable TotalLights back to 0 before the code executes again
 
LastLight = NewLight; //Sets the variable LastLight to the variable NewLight to detect new changes
delay(3000); //Runs a 3 second delay to keep too much information from running into the reciever
}
}

void TurnOnGreen(){ //This is a void to turn on the green lights and turn off all others, called in the above code
  for(int i=0; i<= NumLeds; i++){ //For loop to run through all LEDs
    ledsGreen[i] = CRGB::Green; //Set green LEDs to green
    ledsYellow[i] = CRGB::Black; //Set Yellow LEDs to off
    ledsRed[i] = CRGB::Black; //Set Red LEDs to off
  }
  FastLED.show(); //Show the LEDs assigned color
}

void TurnOnYellow(){ //This is a void to turn on the Yellow lights and turn off all others, called in the above code
  for(int i=0; i<= NumLeds; i++){ //For loop to run through all the LEDs
    ledsGreen[i] = CRGB::Black; //Turn green LEDs off
    ledsYellow[i] = CRGB::Yellow; //Turn Yellow LEDs on
    ledsRed[i] = CRGB::Black; //Turn off the Red LEDs
  }
  FastLED.show(); //Show the LEDs assigned color
}

void TurnOnRed(){ //This is a void to turn on the Red lights and turn off all others, called in the above code
  for(int i=0; i<= NumLeds; i++){ //For loop to run through all the LEDs
    ledsGreen[i] = CRGB::Black; //Turn all Green LEDs off
    ledsYellow[i] = CRGB::Black; //Turn all Yellow LEDs off
    ledsRed[i] = CRGB::Red; //Turn all Red LEDs on
  }
  FastLED.show(); //Show the LEDs assigned colors
}

void TurnOnOff(){ //This is a void to turn off all the LEDs, called in the code above
  for(int i=0; i<= NumLeds; i++){ //For loop to run through all the LEDs
    ledsGreen[i] = CRGB::Black; //Turn off all green LEDs
    ledsYellow[i] = CRGB::Black; //Turn off all yellow LEDs
    ledsRed[i] = CRGB::Black; //Turn off all red LEDs
  }
  FastLED.show(); //Show the LEDs assigned colors 
}

void TurnOnPurple(){ //This is a void to turn on all LEDs and show them in purple, called in the above code
  for(int i=0; i<= NumLeds; i++){ //For loop to run through all the LEDs
    ledsGreen[i] = CRGB::Purple; //Turn the green LEDs on to purple
    ledsYellow[i] = CRGB::Purple; //Turn on the Yellow LEDs to purple
    ledsRed[i] = CRGB::Purple; //Turn the on the Red LEDs to purple
  }
  FastLED.show(); //Show the assigned colors
}
