//Ring It! Source code
//Luca Mahoney
//Zach Hartman
// roger roger
//James Bickerstaff

//MUST IMPORT TMRpcm
//MUST IMPORT Adafruit LED Backpack
//MUST IMPORT Adafruit GFX - may come with above library
//https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric-9b21a470-83ad-459c-af02-209d8d82c462

#include <Keypad.h>
#include <Adafruit_LEDBackpack.h>

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

//score, timer, rounds, and loss boolean
int score = 0;
unsigned long currTime = 0;
int timeOut = 8; //start at 8 second timer 
int decreaseTime = 0.05; //decrease timer by 0.05 seconds every successful round
//int rounds = 0;
bool loss = false;

//alphanumeric display var
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

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
  
  //reset score, time, rounds, loss boolean
  score = 0;
  timeOut = 8;
  //rounds = 0;
  loss = false;
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

        //call dial it function, returning a bool true for losing, false for winning
        loss = dial();
        //check loss bool
        if(loss)
        {
          endGame();
          return;
        }
        else
        {
          score++;
        }
       
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
  //generate the 4 random numbers
  int randOne = random(0, 10); //generates a random number 0 to 9
  int randTwo = random(0, 10); //generates a random number 0 to 9
  int randThree = random(0, 10); //generates a random number 0 to 9
  int randFour = random(0, 10); //generates a random number 0 to 9

  

  //grab the time at which this task is starting
  currTime = millis() / 1000.0; //millimeters, so going to divide by 1000
  int gameOver = currTime + timeOut; //2 + 8 -> 10

  //just stay in a while loop until the task is completed, or the user fails
  while(true)
  {

    //update curr time
    currTime = millis() / 1000.0; //millimeters, so going to divide by 1000
    if(currTime > gameOver) //check if time elapsed for this task is > the timeout
    {
      return true; //game over 
    }
  }
}


//method for lock
int lock()
{

}


//method for answer
int answer()
{

}




bool check_others(int func)
{
  if(func != 1) //dial it check
  {
    //read character being pressed on keypad
    char currKey = customKeypad.getKey();

    //check if a key was pressed
    if(currKey != NO_KEY)
      return true; //if key pressed when it shouldn't be, return true, meaning the game is lost
  }

  if(func != 2) //lock it check
  {
    
  }

  if(func != 3)
  {
    
  }

}
