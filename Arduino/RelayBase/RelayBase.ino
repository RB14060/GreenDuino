/*
Relay Controller Box skeleton code
This code should be used and modified for the specific box being worked on.

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
boolean startup, estop, booted, schedule, menu, alarm, alarmState, ack, timeout = false;
int menuStage, menuSubStage, buttonCount = 0;
#define SCREEN_TIMEOUT 5000
#define SCREEN_OFF 10000
unsigned long lastPress = millis();

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
        buttonCount = 0;
        break;
      case CONTROLEO_BUTTON_BOTTOM:
        fiveBeep();
        buttonCount = 0;
        break;
     }
  }
  else if (booted == true) { //handle main screen
    displayMainTime();
    switch (getButton()) {
      case CONTROLEO_BUTTON_TOP:
        booted = false;
        estop = true;
        handleAlarm();
        break;
      case CONTROLEO_BUTTON_BOTTOM:
        break;
    }
  }
  else if (alarm == true) {
    handleAlarm();
  }
  if (timeout == true) {
    if ((lastPress + SCREEN_TIMEOUT) < millis()) {
      lcd.setBacklight(LOW);
    }
    //if ((lastPress + SCREEN_OFF) < millis()) {
    //  lcd.noDisplay();  
    //}
  }
  delay(50);
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
    buttonCount++;
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
   buttonCount++;
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

void fiveBeep() {
  for (int i=0;i<5;i++)
  {
    lcd.setBuzzer(HIGH);
    delay(500);
    lcd.setBuzzer(LOW);
    delay(500);
  }
}

void handleAlarm() {
  if (alarm == false)
  {
    lcd.home();
    lcd.print("**ALARM**    ACK");
    screenOn();
    timeout = false;
    if (estop = true);
    {
      lcd.setCursor(0,1);
      lcd.print(" EMERGENCY STOP ");
      for (int i=4;i<8;i++)
      {
        digitalWrite(i, LOW);
      }
    }
    alarm = true;
    alarmState = true;
    lcd.setBuzzer(HIGH);
  }
  else if (ack == false)
  {
    switch(getButton())
    {
      case CONTROLEO_BUTTON_TOP:
        ack = true;
        alarmState = false;
        lcd.setBuzzer(LOW);
        lcd.setCursor(11,0);
        lcd.print("CLEAR");
        break;
      case CONTROLEO_BUTTON_BOTTOM:
        break;
      default:
       if (alarmState == true)
        {
          alarmState = false;
          lcd.setBuzzer(LOW);
          lcd.setCursor(13,0);
          lcd.print("   ");
        }
       else
        {
          alarmState = true;
          lcd.setBuzzer(HIGH);
          lcd.setCursor(13,0);
          lcd.print("ACK");
        }
   }
 }
 else
 {
   switch(getButton())
   {
    case CONTROLEO_BUTTON_TOP:
      alarm = false;
      ack = false;
      estop = false;
      booted = true;
      lcd.home();
      lcd.print("  NAME    E-STOP");
      timeout = true;
      break;
    case CONTROLEO_BUTTON_BOTTOM:
      break;
   }
}
delay(500);
}
