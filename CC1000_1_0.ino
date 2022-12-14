/*

Quasi production release of can crusher code. Updated from Can_Crush_3_5

d wojnar 10/18/17

 
*/

// ************ Declare Variables ***********
int motorhome = 0;
int endoftravel = 0;
int cansense = 0;
int motorPower = 0;
int motorDirection = 0;
int motorDirection1 = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // *************** establish pin control assignments *************
pinMode(3, OUTPUT); // motor direction control - signal line to relay ok2 (2 lines are needed due to arduino output current limit)
pinMode(5, OUTPUT); // motor direction control #2 - signal line to relay ok3
pinMode(4, OUTPUT); // motor power - signal line to relay ok1
pinMode(9, INPUT); // motor home
pinMode(6, INPUT); // end of travel
pinMode(7, INPUT); // can sense


// **********  set outputs low **************   
digitalWrite(3, LOW);   
digitalWrite(4, LOW);
digitalWrite(5, LOW);
}

// the loop function runs over and over again forever
void loop() {
  
  // ******** read sensor inputs & assign to variables **********
  motorhome = digitalRead(9);
  endoftravel = digitalRead(6);
  cansense =  analogRead(A0);
  motorPower = digitalRead(4);
  motorDirection = digitalRead(3);
   

   if (cansense < 30 && motorhome == HIGH) {   //crush if motor is home and a can is sensed
crush();
  }

  if (endoftravel == HIGH) {     // change direction at end of travel
park();        
  }
          
 
  if (motorhome == HIGH && cansense > 30) { // rest if motor is home and no can is sensed
rest();
  }
 
}// end of main loop

// ******* subroutines ******

// crush invoked when cansense < 30
void crush(){
  
   digitalWrite(3, HIGH);   // Set motor direction forward
   digitalWrite(5, HIGH);   // Set motor direction forward
   delay(50);
   digitalWrite(4, HIGH);   // Power on motor
   delay(10); 
   digitalWrite(4, LOW);   // Power cycle for transient reduction
   delay(10);
   digitalWrite(4, HIGH);   // Power on motor
   delay(20); 
   digitalWrite(4, LOW);   // Power cycle for transient reduction
   delay(10);
   digitalWrite(4, HIGH);   // Power on motor
   
   
   delay(300); // allow crush head to clear home limit switch
   
    
}

// this routine changes motor direction when EOT is reached
void park(){
delay(400); // delay after EOT sensor is tripped - bigger number = longer crush & possible motor stall
digitalWrite(4, LOW);    // Power off motor
           delay(500);
           digitalWrite(3, LOW);   // Set motor direction reverse
           digitalWrite(5, LOW);   // Set motor direction reverse
           delay(1000); // wait for motor start cap to charge
   digitalWrite(4, HIGH);   // Power on motor
   delay(10); 
   digitalWrite(4, LOW);   // Power cycle for transient reduction
   delay(10);
           digitalWrite(4, HIGH);   // Power on motor 
           delay(2000);   // this delay is required to allow the crush head to clear the can sensor and EOT sensor
}



void rest() {
   
  delay(100);  // allow crush head to move past limit switch
    digitalWrite(4, LOW);    // Power off motor
    
}

