//Ring It! Source code
//Luca Mahoney
//Zach Hartman
//James Bickerstaff

//MUST IMPORT TMRpcm
//MUST IMPORT Adafruit LED Backpack
//MUST IMPORT Adafruit GFX - may come with above library
//https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric-9b21a470-83ad-459c-af02-209d8d82c462

#include <Keypad.h>

//Mic input - pin 23 - A0
int micIn = A0;
//blue LED control - pin 24 - A1
int blueOut = A1;
//red LED control - pin 25 - A2
int redOut = A2;
//green LED control - pin 26 - A3
int greenOut = A3;
//display Data - pin 27 - A4
//int dispData = A4;
//display CLK - pin 28 - A5
//int dispClk = A5;
//lock it SW1 - pin 2 - D0
int lockSw1 = 0;
//lock it SW2 - pin 3 - D1
int lockSw2 = 1;

//speaker PWM output - pin 16 - D10
int speakerOut = 10;
//SD Card MOSI - pin 17 - D11
int sdMosi = 11;
//SD Card MISO - pin 18 - D12
int sdMiso = 12;
//SD Card SCK - pin 19 - D13
int sdSck = 13;

//score, timer, and rounds
int score = 0;
int timer = 10; 
int rounds = 0;

/* KEYPAD INFO AND ASSIGNMENTS */
const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {4, 5, 15, 11}; 
byte colPins[COLS] = {12, 13, 14}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//setup function to assign pinModes
void setup() 
{
  //set pin modes  like below
  pinMode(micIn, INPUT);  
  pinMode(blueOut, OUTPUT);    
  pinMode(redOut, OUTPUT);
  pinMode(greenOut, OUTPUT);
  pinMode(lockSw1, INPUT);
  pinMode(lockSw2, INPUT);
  pinMode(speakerOut, OUTPUT);
  pinMode(sdMosi, INPUT); //may need changing
  pinMode(sdMiso, INPUT); //may need changing
  pinMode(sdSck, INPUT); //may need changing

  Serial.begin(9600);
}


//entry loop
void loop() 
{
  //read character being pressed on keypad
  char currKey = customKeypad.getKey();
  
  //reset score, time, rounds
  score = 0;
  timer = 0;
  rounds = 0;
  if(currKey == '#')
    game();
}


//main game method/loop
void game()
{
  //for loop going to 99, break out and return if loss (?)
  for(int i  = 0; i < 100; i++)
  {
    int num = 0; // = rand(0-3) idk how to generate a random number
    //generate rand num 1 to 3
    switch(num)
    {
      case 1:
       //call function, if 1 returned, win, 0, fail (?)
      break;
      case 2:
        //call function, if 1 returned, win, 0, fail (?)
      break;
      case 3:
        //call function, if 1 returned, win, 0, fail (?)
      break;
    }
  }
  //win or game over
  endGame();
}


//end game func for display/sound/light
void endGame()
{

}


//method for dial
int dial()
{

}


//method for lock
int lock()
{

}


//method for answer
int answer()
{

}





