/*
Relay Controller Box skeleton code
This could should be used and modified for the specific box being worked on.

Version 0.1
Ryan Becker - November 2013
*/

//import necessary headers/libraries
#include "Wire.h"
#include <ControLeo.h>
#include "RTClib.h"

//setup libraries
ControLeo_LiquidCrystal lcd;
RTC_DS1307 RTC;

//setup RTC arrays
char *months[] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
char *nameOfDay[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

//define variables
boolean startup = false;
boolean estop = false;
boolean booted = false;
boolean schedule = false;
boolean menu = false;
int menuStage = 0;
int menuSubStage = 0;
#define SCREEN_TIMEOUT 5000
#define SCREEN_OFF 10000
unsigned long lastPress = millis();
boolean timeout = false;

void setup() { //setup hardware components
  startup = true;
  lcd.begin(16, 2); //set LCD size

  //show initialization message
  lcd.print("**INITIALIZING**");
  lcd.setCursor(0, 1);
  lcd.print("  PLEASE  WAIT  ");
  lcd.setBuzzer(HIGH);
  lcd.setBacklight(LOW);
  delay(300);
  lcd.setBuzzer(LOW);
  lcd.setBacklight(HIGH);
  
  RTC.begin(); //start RTC
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__)); //set RTC time to sketch compile time, if it's not set
  }
  for (int i=4; i<8; i++) { //setup relay pins, and make sure they're off
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  delay(1000);
  lcd.clear(); //initialization complete, let's go!
  lcd.print(" Startup     RUN");
  lcd.setCursor(0, 1);
  lcd.print("Complete.");
  lcd.setBuzzer(HIGH);
  delay(750);
  lcd.setBuzzer(LOW);
} //control now passes to the main loop

//main program code
void loop() {
  if (startup == true) { //leave initialization mode, begin main loop
    switch (getButton()) {
      case CONTROLEO_BUTTON_TOP:
        startup = false;
        booted = true;
        lcd.clear();
        lcd.print("  NAME    E-STOP");
        displayMainTime();
        timeout = true;
        break;
     }
  }
  else if (booted == true) { //handle main screen
    switch (getButton()) {
      case CONTROLEO_BUTTON_TOP:
        //booted = false;
        //estop = true;
        break;
    }
        displayMainTime();
 delay(50);
  }
  if (timeout == true) {
    if ((lastPress + SCREEN_TIMEOUT) < millis()) {
      lcd.setBacklight(LOW);
    }
    //if ((lastPress + SCREEN_OFF) < millis()) {
    //  lcd.noDisplay();  
    //}
  }
}
//the following code is from the ControLeo HardwareTest sketch

// Determine if a button was pressed (with debounce)
// A button can only be pressed once every 200ms. If a button is
// pressed and held, a button press will be generated every 200ms.
// Returns:
//   CONTROLEO_BUTTON_NONE if no button are pressed
//   CONTROLEO_BUTTON_S2 if the top button was pressed
//   CONTROLEO_BUTTON_S3 if the bottom button was pressed
// Note: If both buttons are pressed simultaneously, CONTROLEO_BUTTON_S3 will be returned

#define DEBOUNCE_INTERVAL  200

int getButton()
{
  static long lastChangeMillis = 0;
  long nowMillis = millis();
  int buttonValue;
  
  // If insufficient time has passed, just return none pressed
  if (lastChangeMillis + DEBOUNCE_INTERVAL > nowMillis) {
    return CONTROLEO_BUTTON_NONE; }
  
  // Read the current button status
  int pinValue = analogRead(CONTROLEO_BUTTON_PIN);
  
  // Interpret the value.  Allow for resistor variance
  if (pinValue < 100) {
    buttonValue = CONTROLEO_BUTTON_BOTTOM;
    lcd.setBuzzer(HIGH);
    delay(50);
    lcd.setBuzzer(LOW);
    screenOn();
  }
  else if (pinValue > 900) {
    buttonValue = CONTROLEO_BUTTON_NONE;
  }
  else {
   buttonValue = CONTROLEO_BUTTON_TOP;
   lcd.setBuzzer(HIGH);
   delay(50);
   lcd.setBuzzer(LOW);
   screenOn();
  }
  
  // Note the time the button was pressed
  if (buttonValue != CONTROLEO_BUTTON_NONE) {
   lastChangeMillis = nowMillis; }
  
  return buttonValue;
}

void screenOn()
{
  lcd.setBacklight(HIGH);
  lcd.display();
  lastPress = millis();
}

void displayMainTime() {
  lcd.setCursor(0, 1);
  DateTime now = RTC.now();
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute() < 10) {
    lcd.print("0");
  }
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second() < 10) {
      lcd.print("0");
    }
    lcd.print(now.second(), DEC);
    lcd.print("    MENU");
}