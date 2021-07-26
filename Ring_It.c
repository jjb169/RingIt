//Ring It! Source code
//Luca Mahoney
//Zach Hartman
//James Bickerstaff

//MUST IMPORT TMRpcm
//MUST IMPORT Adafruit LED Backpack
//MUST IMPORT Adafruit GFX - may come with above library
//https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric-9b21a470-83ad-459c-af02-209d8d82c462

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

/* STEAL KEYPAD CODE FOR 4X3 FROM INTERNET WITH USE OF MATRIX AND STUFF */

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
  //NEED KEYPAD ASSIGNMENTS

  
}


//entry loop
void loop() 
{
  //reset score, time, rounds
  score = 0;
  timer = 0;
  rounds = 0;
  //if( * and & are presed)
    //game()
}


//main game method/loop
void game()
{
  //win or game over
  //return
}


//method for dial
void dial()
{

}


//method for lock
void lock()
{

}


//method for answer
void answer()
{

}





