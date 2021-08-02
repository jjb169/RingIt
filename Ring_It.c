//Ring It! Source code
//Luca Mahoney
//Zach Hartman
//James Bickerstaff

//MUST IMPORT TMRpcm
//MUST IMPORT Adafruit LED Backpack
//MUST IMPORT Adafruit GFX - may come with above library
//https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric-9b21a470-83ad-459c-af02-209d8d82c462

#include <Keypad.h>
#include <Adafruit_LEDBackpack.h>
#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

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

byte rowPins[ROWS] = {2, 3, 9, 5};
byte colPins[COLS] = {6, 7, 8};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//sd card object
TMRpcm tmrpcm;

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

  tmrpcm.speakerPin = 10;
  tmrpcm.setVolume(5);

  Serial.begin(9600);
  alpha4.begin(0x70); //zach added this line - needed to initialize display
}


bool first = true;
//entry loop
void loop()
{
  //if first time in loop, check SD card
  if(first)
  {
    //check if sd card is available
    if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
      alpha4.writeDigitAscii(0, 'X');
      alpha4.writeDisplay();
      //delay(100);
    }
    //play sound
    tmrpcm.play("Intro.wav");
    first = false;
  }
  /*
  //tmrpcm.play("test.wav");
  
  if (tmrpcm.isPlaying())
  {
    alpha4.writeDigitAscii(0, 'E');
    alpha4.writeDisplay();
  }
  else
  {
    alpha4.writeDigitAscii(0, 'P');
    alpha4.writeDisplay();
  }
  

  delay(500);
  */
  //make sure all lights are off
  digitalWrite(redOut, LOW);
  digitalWrite(blueOut, LOW);
  digitalWrite(greenOut, LOW);

  //read character being pressed on keypad
  char currKey = customKeypad.getKey();

  //reset score, time, rounds, loss boolean
  score = 0;
  timeOut = 8;
  //rounds = 0;
  loss = false;
  if (currKey == '#')
    game();
  
}


//main game method/loop
void game()
{
  //declare bool light here to reuse in this loop
  bool light = false;

  //for loop going to 99, break out and return if loss (?)
  for (int i  = 0; i < 100; i++)
  {
    //blank the display to start each loop
    blank();

    randomSeed(analogRead(3));

    //gen a number 1-3 to determine which task to issue
    int num = random(1, 4); //generates a random number 1 to 3
    switch (num)
    {
      case 1:
        //turn on blue light for this task
        digitalWrite(blueOut, HIGH);
        light = true;
        //call dial it function, returning a bool true for losing, false for winning
        loss = dial();
        //check loss bool
        if (loss)
        {
          //if fail, turn off the light
          digitalWrite(blueOut, LOW);
          /*
            // compare score to 10. If it's less, then it will only take up one of the 4 displays
            char dispScore[2];
            int tensDigit;
            int onesDigit;
            if (score < 10)
            {
            dispScore[0] = score + 48;//the score is from 0-9, so the score can go right into dispScore
            dispScore[1] = ' '; // this is just to fill out the array. blank char
            }
            else if (score < 100) //the max score is 99, but in case there's a leak/math error, this will allow only 10-99 to be displayed. If over 99, nothing will be displayed
            {
            tensDigit = score / 10; //dividing an int by an int gives an int, so dividing a 2 digit number by 10 will give the tens digit
            onesDigit = score % 10; //modulus to get the remainder of score/10. This gets the ones digit

            dispScore[0] = onesDigit + 48; //convert to char
            dispScore[1] = tensDigit + 48; //convert to char
            }
            //display final score
            alpha4.writeDigitAscii(3, dispScore[0]);
            alpha4.writeDigitAscii(2, dispScore[1]);
            alpha4.writeDigitAscii(1, '-');
            alpha4.writeDigitAscii(0, 'S');
            alpha4.writeDisplay();
            delay(500);
          */
          endGame(score);
          return;
        }
        else
        {
          score++; //increase the player score
          timeOut = timeOut - decreaseTime; //decrease time to complete a task

          //set to flick the LED on and off
          for (int i = 0; i < 11; i++)
          {
            //if light is on, turn it off and switch the bool
            if (light == true)
            {
              digitalWrite(blueOut, LOW);
              light = false;
            }
            else
            {
              digitalWrite(blueOut, HIGH);
              light = true;
            }
            delay(1);
          }
        }

        break;
      case 2:
        //call function, if 1 returned, win, 0, fail (?)
        digitalWrite(redOut, HIGH);
        light = true;
        loss = lock();
        //check loss bool
        if (loss)
        {
          digitalWrite(redOut, LOW);//turn light off
          /*
            //light
            // compare score to 10. If it's less, then it will only take up one of the 4 displays
            char dispScore[2];
            int tensDigit;
            int onesDigit;
            if (score < 10)
            {
            dispScore[0] = score + 48;//the score is from 0-9, so the score can go right into dispScore
            dispScore[1] = ' '; // this is just to fill out the array. blank char
            }
            else if (score < 100) //the max score is 99, but in case there's a leak/math error, this will allow only 10-99 to be displayed. If over 99, nothing will be displayed
            {
            tensDigit = score / 10; //dividing an int by an int gives an int, so dividing a 2 digit number by 10 will give the tens digit
            onesDigit = score % 10; //modulus to get the remainder of score/10. This gets the ones digit

            dispScore[0] = onesDigit + 48; //convert to char
            dispScore[1] = tensDigit + 48; //convert to char
            }
            //display final score
            alpha4.writeDigitAscii(3, dispScore[0]);
            alpha4.writeDigitAscii(2, dispScore[1]);
            alpha4.writeDigitAscii(1, '-');
            alpha4.writeDigitAscii(0, 'S');
            alpha4.writeDisplay();
            delay(500);
          */
          endGame(score);
          return;
        }
        else
        {
          score++;
          timeOut = timeOut - decreaseTime; //decrease time to complete a task
          //set to flick the LED on and off. ends off
          for (int i = 0; i < 11; i++)
          {
            //if light is on, turn it off and switch the bool
            if (light == true)
            {
              digitalWrite(redOut, LOW);
              light = false;
            }
            else
            {
              digitalWrite(redOut, HIGH);
              light = true;
            }
            delay(1);
          }
        }

        break;
      case 3:
        //call function, if 1 returned, win, 0, fail (?)
          //turn on green light for answer it
          digitalWrite(greenOut, HIGH);
          light = true;
          //call answer it function, returning a bool true for losing, false for winning
          loss = answer();

          //check loss bool
          if(loss)
          {
          //if fail, turn off the light
          digitalWrite(greenOut, LOW);
          endGame(score);
          return;
          }
          else
          {
          score++;//increase the player's score by 1
          timeOut = timeOut - decreaseTime;

          //set to flick the LED on and off. ends off
          for(int i=0;i<11;i++)
          {
            //if light is on, turn it off and flip bool
            if(light)
            {
              digitalWrite(greenOut, LOW);
              light =false;
            }
            else //if light is off, turn it on and flip bool
            {
              digitalWrite(greenOut, HIGH);
              light = true;
            }
            delay(1);
          }
          }
        

        break;
    }
  }
  //win or game over
  endGame(score);
}






//end game func for display/sound/light
void endGame(int score)
{
  alpha4.writeDigitAscii(0, 'E');
  alpha4.writeDigitAscii(1, 'N');
  alpha4.writeDigitAscii(2, 'D');
  alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY
  delay(500);


  // compare score to 10. If it's less, then it will only take up one of the 4 displays
  char dispScore[2];
  int tensDigit;
  int onesDigit;
  if (score < 10)
  {
    dispScore[0] = score + 48;//the score is from 0-9, so the score can go right into dispScore
    dispScore[1] = ' '; // this is just to fill out the array. blank char
  }
  else if (score < 100) //the max score is 99, but in case there's a leak/math error, this will allow only 10-99 to be displayed. If over 99, nothing will be displayed
  {
    tensDigit = score / 10; //dividing an int by an int gives an int, so dividing a 2 digit number by 10 will give the tens digit
    onesDigit = score % 10; //modulus to get the remainder of score/10. This gets the ones digit

    dispScore[0] = onesDigit + 48; //convert to char
    dispScore[1] = tensDigit + 48; //convert to char
  }
  //display final score
  /*
    alpha4.writeDigitAscii(3, dispScore[0]);
    alpha4.writeDigitAscii(2, dispScore[1]);
    alpha4.writeDigitAscii(1, '-');
    alpha4.writeDigitAscii(0, 'S');

    alpha4.writeDisplay();
  */
  //now the score is saved in dispScore as characters. Now the final score will be displayed
  //if they won, display special lights
  if (score == 99)
  {
    //display victory blue-green
    digitalWrite(greenOut, HIGH);
    digitalWrite(blueOut, HIGH);
  }
  else
  {
    //if the score were not 99, then the user lost
    //display loser lights - purple
    digitalWrite(blueOut, HIGH);
    digitalWrite(redOut, HIGH);

  }

  //display final score
  alpha4.writeDigitAscii(3, dispScore[0]);
  alpha4.writeDigitAscii(2, dispScore[1]);
  alpha4.writeDigitAscii(1, '-');
  alpha4.writeDigitAscii(0, 'S');
  alpha4.writeDisplay();
}







//method for dial
bool dial()
{
  //play sound
  tmrpcm.play("DialIt.wav");

  alpha4.writeDigitAscii(0, 'D');
  alpha4.writeDigitAscii(1, 'I');
  alpha4.writeDigitAscii(2, 'A');
  alpha4.writeDigitAscii(3, 'L');
  alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY

  randomSeed(analogRead(0));

  //generate the 4 random numbers
  int randOne = random(0, 10); //generates a random number 0 to 9
  int randTwo = random(0, 10); //generates a random number 0 to 9
  int randThree = random(0, 10); //generates a random number 0 to 9
  int randFour = random(0, 10); //generates a random number 0 to 9
  //need these random ints in char format to display properly
  char randChars[4] = {randOne + 48, randTwo + 48, randThree + 48, randFour + 48}; // zach changed this line - need to add 48 to convert int to proper ascii char

  //put these numbers on the display
  alpha4.writeDigitAscii(0, randChars[0]);
  alpha4.writeDigitAscii(1, randChars[1]);
  alpha4.writeDigitAscii(2, randChars[2]);
  alpha4.writeDigitAscii(3, randChars[3]);
  alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY

  //int to keep track of what digit is being pressed
  int currDigit = 0;

  //grab the time at which this task is starting
  currTime = millis() / 1000; //millimeters, so going to divide by 1000
  int gameOver = currTime + timeOut; //2 + 8 -> 10

  //just stay in a while loop until the task is completed, or the user fails
  while (true)
  {
    //watch for user input from left to right - digit 0 to 3
    //read character being pressed on keypad
    char currKey = customKeypad.getKey();

    //check if currDigit >= 4, meaning task is done
    if (currDigit >= 4)
      return false; //task is done successfully

    //check if the correct key is pressed
    if (currKey == randChars[currDigit]) //if correct key pressed
    {
      currDigit++; //move to next character
      //blank the key just pressed
      for (int i = 0; i < currDigit; i++)
      {
        alpha4.writeDigitAscii(i, ' ');
      }
      alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY
    }
    else if (currKey != NO_KEY) //if key other than expected was pressed
      return true; //return true as the incorrect button was pressed and game is lost

    //check if the user did the incorrect task
    if (check_others(1))
    {
      //return true;//game over. The user performed the wrong task
    }

    //update curr time
    currTime = millis() / 1000;//millimeters, so going to divide by 1000
    if (currTime > gameOver) //check if time elapsed for this task is > the timeout
    {
      return true; //game over
    }
  }
}


//method for lock
bool lock()
{
  //play sound
  tmrpcm.play("LockIt.wav");

  alpha4.writeDigitAscii(0, 'L');
  alpha4.writeDigitAscii(1, 'O');
  alpha4.writeDigitAscii(2, 'C');
  alpha4.writeDigitAscii(3, 'K');
  alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY


  currTime = millis() / 1000; //millimeters, so going to divide by 1000
  int gameOver = currTime + timeOut; //2 + 8 -> 10

  //just stay in a while loop until the task is completed, or the user fails
  while (true)
  {
    int valSw1 = digitalRead(lockSw1);
    int valSw2 = digitalRead(lockSw2);

    if (valSw1 == HIGH && valSw2 == HIGH)
    {
      return false;
    }

    if (check_others(2))
    {
      return true;//game over. The user performed the wrong task
    }

    //grab the time at which this task is starting
    //update curr time
    currTime = millis() / 1000; //millimeters, so going to divide by 1000
    if (currTime > gameOver) //check if time elapsed for this task is > the timeout
    {
      return true; //game over
    }

  }
}


//method for answer
bool answer()
{
  //play sound
 // tmrpcm.play("AnswerIt.wav");

  alpha4.writeDigitAscii(0, 'C');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'L');
  alpha4.writeDigitAscii(3, 'L');
  alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY


  currTime = millis() / 1000; //millimeters, so going to divide by 1000
  int gameOver = currTime + timeOut; //2 + 8 -> 10

  while(true)
  {
    //read input from mic
    int valMic = digitalRead(micIn);

    //active low, so check if LOW signal
    if(valMic == LOW)
    {
      return false; //return false to indicate success
    }

    //check if other tasks were performed instead
    if (check_others(2))
    {
      return true;//game over. The user performed the wrong task
    }

    //grab the time at which this task is starting
    //update curr time
    currTime = millis() / 1000; //millimeters, so going to divide by 1000
    if (currTime > gameOver) //check if time elapsed for this task is > the timeout
    {
      return true; //game over
    }
 
    /*
    if(valMic == HIGH)
    {
      alpha4.writeDigitAscii(0, 'H');
      alpha4.writeDigitAscii(1, 'I');
      alpha4.writeDigitAscii(2, 'G');
      alpha4.writeDigitAscii(3, 'H');
      alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY
      digitalWrite(greenOut, HIGH);
      digitalWrite(redOut, LOW);
      digitalWrite(blueOut, LOW);
    }
    else if(valMic == LOW)
    {
      alpha4.writeDigitAscii(0, 'L');
      alpha4.writeDigitAscii(1, 'O');
      alpha4.writeDigitAscii(2, 'W');
      alpha4.writeDigitAscii(3, ' ');
      alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY
      digitalWrite(greenOut, LOW);
      digitalWrite(redOut, HIGH);
      digitalWrite(blueOut, LOW);
      delay(1000);
    }
    */    
  }
}






bool check_others(int func)
{
  if (func != 1) //dial it check
  {
    //read character being pressed on keypad
    char currKey = customKeypad.getKey();

    //check if a key was pressed
    if (currKey != NO_KEY)
      return true; //if key pressed when it shouldn't be, return true, meaning the game is lost
  }

  if (func != 2) //lock it check
  {

    int valSw1 = digitalRead(lockSw1);
    int valSw2 = digitalRead(lockSw2);

    if (valSw1 == HIGH || valSw2 == HIGH)
    {
      return true;
    }

    // need to get input from two switches.
    // if either are pressed, the user should fail


  }

  if (func != 3) //answer it check
  {

  }

  return false; // returns false if all three if loops pass eval checks
}

//helper function for blanking the display
void blank()
{
  //put blank things on display
  alpha4.writeDigitAscii(0, ' ');
  alpha4.writeDigitAscii(1, ' ');
  alpha4.writeDigitAscii(2, ' ');
  alpha4.writeDigitAscii(3, ' ');
  alpha4.writeDisplay(); //MUST CALL THIS TO WRITE TO DISPLAY
}
