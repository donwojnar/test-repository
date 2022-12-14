/*
Autopointer-6 update to include random point function

Autopointer-5 updated to include laser blink at startup 10/15/19

unit draws a line on garage floor to direct driver then draws line on hood
to designate park location

TV remote functions:

Vol- = reduce servoX value
Vol+ = increase servoX value
CH- = reduce servoY value
CH+ = increase servoY value
mute = fine laser position adjust
sleep = coarse laser position adjust

Button 1 = store x1 and y1 locations
Button 2 = store x2 and y2 locations
Button 3 = Draw line between x1,y1 and x2,y2
Button 4 = Launch parking app

Power   = Laser toggle on/off

D Wojnar 10/15/19

Remote codes:
E0E020DF = 1
E0E0A05F = 2
E0E0609F = 3
E0E040BF = power
E0E0D02F = vol -
E0E0E01F = vol +
E0E008F7 = ch-
E0E048B7 = ch+
E0E0F00F = mute
E0E010EF = 4
E0E0906F = 5
E0E050AF = 6
E0E030CF = 7
E0E0B04F = 8
E0E0708F = 9
E0E08877 = 0
E0E0F00F = Mute
E0E0C03F = sleep

*/

// ********************* Global Declararions **************************$$

// library associations                                       *** $$$ ***

#include <Servo.h>
#include "IRremote.h"


// ****       pin assignment + class instantiation             *** $$$ ***

// Assign servoX 
int servoPinX = 8; 
// Instantiate servoX 
Servo servoX;

// Assign servoY to arduino digital pin 7
int servoPinY = 7;
// Instantiate servoY 
Servo servoY;

// Assign receiver - assignment must happen before instantiation
int receiver = 3; 
// Instantiate 'irrecv' - note that moving IRrecv to setup caused error
IRrecv irrecv(receiver);

// Assign pin 6 for laser power
int laserPower = 6;

// instantiate 'decode_results'           
decode_results results; 
 
// ****    Global Variables               ***                  *** $$$ ***

// servo travel limits - servo behaves badly @ 0-10 and 170-180
int hiLimit = 170;
int loLimit = 10;
int findMidpoint = 80;  // (170-10) / 2 = 80 use forsetting laser to mid point

//  current servo location   
int xVal =0;
int yVal=0;

// variables for laser position recall
int xLoc1 = 0;
int xLoc2 = 0;
int yLoc1 = 0;
int yLoc2 = 0;

// length of parking raster
int parkRasterLength = 3;

// Delay before checking for another button / repeat
#define  REPEAT_DELAY  300 

// delay for between program steps
int stepDelay = 300;  

// delay constant for blinking laser 
int blinkDelay =500;

//servo steps to increment when arrow btn is pushed (gets modified by UI)   
int stepCount = 5;

// **********************    end of global declarations    *****************$$

// ***************** setup - runs at power up and reset  *******************$$

void setup() 
{ 

// Attach servos 
  servoX.attach(servoPinX); 
  servoY.attach(servoPinY);

 // set pin 6  as output (this gives error if moved to global declarations)
 pinMode(laserPower, OUTPUT);

// initialize both axes to middle of travel             $$  -use for loop $$
 servoX.write(loLimit + findMidpoint);  
 servoY.write(loLimit + findMidpoint); 

// verify laser operation
 int blinkTestCount = 3;
     for (blinkTestCount = 0; blinkTestCount < 3; blinkTestCount++) {
     BlinkLaser();
     LaserOn(false);
     }
     
 // get initial position of servos
 xVal = servoX.read();
 yVal = servoY.read();
     
// Start the receiver
 irrecv.enableIRIn(); 
} 

// *************************     end   setup       ************************$$

 
// ***************************   loop  *********************************   $$
// listen for IR input and pass to response engine 



void loop() 
{ 
// wait for IR signal; if received, pass code received to response engine.
if (irrecv.decode(&results)) 
  {
   ResponseEngine();
// Adjust for repeat / lockout time   
    delay(REPEAT_DELAY); 
// receive the next value   
    irrecv.resume();
  }  
} // ********************   End of loop   ******************************$$


// ************************* subroutines *******************************$$

// This routine Reads incoming IR message and drives reponse in hardware
int ResponseEngine(){
   switch(results.value)  
     {    // start switch  

      // vol-  if Vol- is pressed ****** Drive Laser point Left   ******$$
     case 0xE0E0D02F:   
     {
      LaserOn(true);
      delay(stepDelay);
      xVal = servoX.read();
         // if x position beyond loLimit, decrement x
        if (xVal > loLimit){
         xVal = (xVal - stepCount);
         servoX.write(xVal);
      }
      break;
    } // end of case


        // vol+  if Vol+ is pressed ******** Drive Laser point Right ***$$
    case 0xE0E0E01F:   
    {
      LaserOn(true);
      delay(stepDelay);
      xVal = servoX.read();
        // if x position below hiLimit, increment x
        if (xVal < hiLimit){
          xVal = (xVal + stepCount);
          servoX.write(xVal);
        }
     break;
    } // end of case

        // CH-  if CH- is pressed ****** Drive Laser point Down ******* $$
    case 0xE0E008F7:   
    {
      LaserOn(true);
      delay(stepDelay);
      yVal = servoY.read();
        // if y position beyond loLimit, decrement y
        if (yVal > loLimit){
         yVal = (yVal - stepCount);
          servoY.write(yVal);
        }
      break;
    } // end of case

        // CH+  if CH+ is pressed ******* Drive Laser point UP ********$$
    case 0xE0E048B7:   
    {
      LaserOn(true);
      delay(stepDelay);
      yVal = servoY.read();
        // if y position below hiLimit, increment y
        if (yVal < hiLimit){
          yVal = (yVal + stepCount);
          servoY.write(yVal);
        }
      break;
    } // end of case

      // if 1 is pressed ******* Set Location 1 ***********************$$
      // store first laser location
    case 0xE0E020DF:   
    {
      // store servoX and servoY first position values
      xLoc1 = servoX.read();
      yLoc1 = servoY.read();
      BlinkLaser(); // blink laser to confirm
     break;
    }  // end case

      // if 2 is pressed ********** Set Location 2 ********************$$
      // store second laser location
    case 0xE0E0A05F:   
    {
      // store servoX and servoY second position values
      xLoc2 = servoX.read();
      yLoc2 = servoY.read();
      BlinkLaser(); // blink laser to confirm
     break;
    } // end case

      // if 3 is pressed ********** Drive Laser ***********************$$
      // drive laser between 2 locations determined in 1 and 2
      // $$$$ experiment here with laser on/off timing
    case 0xE0E0609F:   
    {  
      int sweepCount = 5;
        for (int i=0; i < sweepCount; i++) {
         servoX.write(xLoc1);
         delay(stepDelay); // delete this to get a triangle
         servoY.write(yLoc1);
         delay(stepDelay);
 
        servoX.write(xLoc2);
        delay(stepDelay);
        servoY.write(yLoc2);
        delay(stepDelay);
      }
      break;
    } // end case

      // if power button is pressed ******** Toggle Laser *************$$
      // toggle laser on/off
    case 0xE0E040BF:
    {
        if (digitalRead(laserPower)== 0){
        LaserOn(true);
        }
          else if (digitalRead(laserPower)== 1){
          LaserOn(false);
           }
     break;
     }// end case
    
      // if mute button is pressed ********* fine stepping ************$$
      // enable fine stepping
    case 0xE0E0F00F:   
    {
        if (stepCount == 5){
        stepCount = 1;
        }
          else if (stepCount ==1) {
          stepCount = 5;
          }
              else if (stepCount == 20) {
              stepCount = 1;
              } 
    // confirm with blink  
    BlinkLaser();
    break;
    } // end case mute(fine)

      // if sleep button is pressed ******* coarse stepping ************$$
      // enable coarse stepping
    case 0xE0E0C03F:   
    {
        if (stepCount == 5){
        stepCount = 20;
        }
            else if (stepCount ==20) {
            stepCount = 5;
            }
                else if (stepCount == 1) {
                stepCount = 20;
                }
    // blink to confirm  
    BlinkLaser();
    break;
    } // end case sleep (coarse) 


      // if button 4 is pressed ************ Parking App ***************$$
    case 0xE0E010EF:   
    {
    LaserOn(true);
    // paint lane marker on garage floor
    int laneRasterCount = 10;
        for (int i=0; i < laneRasterCount; i++) {
         servoX.write(xLoc1);
         delay(stepDelay); 
         servoY.write(yLoc1);
         delay(stepDelay);
 
        servoX.write(xLoc2);
        delay(stepDelay);
        servoY.write(yLoc2);
        delay(stepDelay);
        }

    int parkRasterCount = 10;
      // paint stop marker on hood
        for (int i=0; i < parkRasterCount; i++) {
         servoX.write(xLoc1);
         delay(stepDelay);
         servoY.write(yLoc1);
         delay(stepDelay);
 
        servoX.write(xLoc1+parkRasterLength);
        delay(stepDelay);
        servoY.write(yLoc1);
        delay(stepDelay);

        servoX.write(xLoc1-parkRasterLength);
        delay(stepDelay);
        servoY.write(yLoc1);
        delay(stepDelay);
        }
        
    LaserOn(false);      
    break;
    } // end case park


      // if button 5 is pressed ************ random point selector ***************$$
      // E0E0906F = 5
    case 0xE0E0906F:   
    {
      long randX = random(40, 140);
      servoX.write(randX);
     // delay(stepDelay);
      long randY = random(40, 140);
      servoY.write(randY);
      delay(stepDelay);
            
    }// end case random

  } // end switch

} // end ResponseEngine


      // ****    Turn laser on/off          ***                *** $$$ ***
      void LaserOn(boolean laserState){
        if (laserState == true) {
         digitalWrite(laserPower, HIGH);
         }
            else if(laserState == false) {
            digitalWrite(laserPower, LOW);
            }
 } // end sub


     // ****    Blink Laser               ***                   *** $$$ ***
    void BlinkLaser(){
    LaserOn(false);
    delay(blinkDelay);
    LaserOn(true);
    delay(blinkDelay);
} // end sub
















 
