/*
DHT-22 Hygrometer with 16 x 2 LCD for control of Heater & Humidifier


4/19/2020 original saketch -  D Wojnar Boulder Co
4/22/2020 added button to pin 6
4/25/2020 added pwr2, added splash screen, screen blanking, & scren chaining
4/26/2020 refactor
4/27/2020 added buttons 2 & 3
5/5/2020 swiched to prototype # 2
5/11/2020 add button 4, refactor
5/12/2020 refactor + add switch
5/14/2020 refactor + add set point adjustment
5/15/2020 function cleanup

 */

// include the libraries:
#include <LiquidCrystal.h>
//#include "DHT.h"  // note that both syntax variants work ??
#include <DHT.h>

//       ************************** DHT 22 Sensor ***************
// set the DHT Pin
#define DHTPIN 8
// DHT type is DHT 22  (AM2302)
#define DHTTYPE DHT22
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE); 

//       ************************** Power Pins ***************
// set power pin #1 to digital pin 9
int pwr1 = 9;
//  Power pin # 2 = digital pin 10
int pwr2 = 10;

//       ************************** Button Input pins ***************
const int button1 = 6;
const int button2 = 7;
const int button3 = 13;
const int button4 = A0;

//       ************************** LCD Interface pins ***************
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//       ************************** Global Variables **************************************** Global Variables ***************
float setPointF = 75;
float setPointH = 30;
int screenState = 0;
String P1State = "OFF";
String P2State = "ON";



void setup() {  //     $$$$$$$$$$$$$$$$$  Setup $$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 2);
  // start the sensor
  dht.begin();
  // set power 1 pin
  pinMode(pwr1, OUTPUT);
  // set power 2 pin
  pinMode(pwr2, OUTPUT);
 // Setup for buttons 1 to 4
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  
 // display splash screen
  SplashScreen();
} // end of setup

void loop() {  //     $$$$$$$$$$$$$$$$$  LOOP $$$$$$$$$$$$$$$$$$$$$$$$$$$$$QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

MainScreen();
ButtonScan();
  

  
  } // End of loop


//*********************************** Sub MainScreen *************
// This is the main interface with the user. It displays temp & Humidity 
// and current status of heater power (P1) and Humidifier power (P2).
// It also shows current setpoints for F and H
// it also controls power 1 and 2 outputs
// it has too much shit
  
void MainScreen(){
  
//read sensor data $$$$$$$$$$

// read humidity
  float h = dht.readHumidity();
//read temperature in Fahrenheit (true) -  () gives C
  float f = dht.readTemperature(true);
  
// isnan = is not a number  - test for invalid sensor output
  if (isnan(h) || isnan(f)) {
    lcd.print("ERROR");
    return;
  } // end if
  
// *************** first screen ****************
//print header line
  lcd.setCursor(0,0);
  lcd.print("Temp:  P1:  SP F");
 
// print data line
  lcd.setCursor(0, 1);
  lcd.print(f);
  lcd.setCursor(4,1);
  lcd.print("F");
  lcd.setCursor(7,1);
  lcd.print(P1State);
  lcd.setCursor(12,1);
  lcd.print(setPointF);
  delay(2000);
  BlankScreen();
// *************** second screen ****************
  
//print header line
  lcd.setCursor(0,0);
  lcd.print("Hum:   P2:  SP H");
  
// print data line
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.setCursor(4,1);
  lcd.print("%");
  lcd.setCursor(7,1);
  lcd.print(P2State);
  lcd.setCursor(12,1);
  lcd.print(setPointH);
  delay(2000);
  BlankScreen();
  
// temperature control  $$$$$$$$$$$

  if(f < setPointF) {
    digitalWrite(pwr1, HIGH);
    P1State = "ON";
  }
    else{
    digitalWrite(pwr1, LOW);
    P1State = "OFF";
    }
    
// humidity control  $$$$$$$$$$$
 if(h < setPointH) {
    digitalWrite(pwr2, HIGH);
    P2State = "ON";
  }
   else{
    digitalWrite(pwr2, LOW);
   P2State = "OFF";
    }

}// End sub main screen


//*********************************** Sub SplashScreen *************
 // This is the Initial Splash Screen displaying version information
 
void SplashScreen(){
  lcd.setCursor(0,0);
  lcd.print("WOJNAR  ");
  lcd.setCursor(0, 1);
  lcd.print("ENGINEERING ");
  delay(2000);
  BlankScreen();
  lcd.setCursor(0,0);
  lcd.print("KEITH'S ");
  lcd.setCursor(0, 1);
  lcd.print("MUSHROOMS");
  delay(2000);
  BlankScreen();
  lcd.setCursor(0,0);
  lcd.print("Version dev 9 ");
  lcd.setCursor(0, 1);
  lcd.print("5/15/2020");
  delay(3000);
  BlankScreen();
  screenState = 0;  // go home
  
} // end SplashScreen


//*********************************** Sub BlankScreen *************
    // erase screen for next data display
void BlankScreen(){
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");

} // end sub BlankScreen

//*********************************** Sub ButtonScan *************
// this sub checks to see if butons 1-4 have been pressed

void ButtonScan(){
 if(digitalRead(button1)== HIGH) {
IncreaseSP1();
 }

if(digitalRead(button2)== HIGH) {
DecreaseSP1();
 }

if(digitalRead(button3)== HIGH) {
IncreaseSP2();
 }

if(digitalRead(button4)== HIGH) {
DecreaseSP2();
 }

 
} // end sub ButtonScan


//*********************************** increase SP 1 *************
// this screen increases current set point F
void IncreaseSP1(){
  BlankScreen();
  lcd.setCursor(0,0);
  lcd.print("increase SP F:");
  delay(500);
  BlankScreen();
  setPointF = setPointF + 1;
  lcd.setCursor(1,10);
  lcd.print(setPointF);
  lcd.setCursor(0, 0);
  lcd.print("New SP F ");
  delay(1000);
  BlankScreen();
 
} // end sub subScreen2

//*********************************** decrease SP1 *************
// this screen decreases current set point F
  void DecreaseSP1(){
  BlankScreen();
  lcd.setCursor(0,0);
  lcd.print("decrease SP F:");
  setPointF = setPointF - 1;
  lcd.setCursor(15,1);
  lcd.print(setPointF);
  lcd.setCursor(0, 1);
  lcd.print("New SP F ");
  delay(2000);
  BlankScreen();
 
} // end sub subScreen4

//*********************************** Increase SP 2 *************
// this screen increases current set point H
  void IncreaseSP2(){
  BlankScreen();
  lcd.setCursor(0,0);
  lcd.print("increase SP H:");
  setPointH = setPointH + 1;
  lcd.setCursor(15,0);
  lcd.print(setPointH);
  lcd.setCursor(0, 1);
  lcd.print("New SP H ");
  delay(2000);
  BlankScreen();
 
} // end sub IncreaseSP2



//*********************************** SubScreen5 *************
// this screen decreases current set point H
void DecreaseSP2(){
   
    BlankScreen();
  lcd.setCursor(0,0);
  lcd.print("decrease Set Point H:");
  setPointH = setPointH - 1;
   lcd.setCursor(15,0);
     lcd.print(setPointH);
  lcd.setCursor(0, 1);
  lcd.print("New SetPointH ");
  delay(2000);
  BlankScreen();
 
} // end sub subScreen3
//************************************************
