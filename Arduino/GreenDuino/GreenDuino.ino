// Greenhouse Controller Project
// Ryan Becker
// Version 0.1 ALPHA
// May 2013

#include <SerialCommand.h>

#define intTemp A0
#define extTemp A1
#define intRH A2
#define extRH A3
#define water A4


SerialCommand controller;  // Define the SerialCommand object

void setup()
{
  Serial.begin(9600); // Start serial communication
  Serial.println("SCI: Initializing...");
  
  //Create commands
  controller.addCommand("HELP",help); // Help system
  controller.addCommand("SENSOR",sensor); // Retrieve sensor readings
  controller.addCommand("LEDON",led_on); // Turn status LEDs on
  controller.addCommand("LEDOFF",led_off); // Turn status LEDs off
  controller.addCommand("RELAYON",relay_on); // Turn relays on
  controller.addCommand("RELAYOFF",relay_off); // Turn relays off
  controller.addCommand("DEBUG",debug); // Test argument handling
  controller.addDefaultHandler(unrecognized); // Unrecognized command

  Serial.println("SCI: Initialization complete.");
}

void loop()
{
  controller.readSerial(); // Check for new command
}

void help() // Help system
{
  Serial.println("GreenDuino Serial Command Interface (SCI)");
  Serial.println("               Help System               ");
  Serial.println("=========================================");
  Serial.println("");
  Serial.println("HELP: This help text.");
  Serial.println("SENSOR: Take a sensor reading.");
  Serial.println("LEDON: Turn on a status LED.");
  Serial.println("LEDOFF: Turn off a status LED.");
  Serial.println("RELAYON: Turn on a relay.");
  Serial.println("RELAYOFF: Turn off a relay.");
  Serial.println("DEBUG: Check command arguments.");
}
void sensor()
{
}

void led_on()
{
}

void led_off()
{
}

void relay_on()
{
}

void relay_off()
{
}

void debug() // Argument test system
{
  String arg = "";

  Serial.println("SCI: Debug System launched.");
  arg = controller.next();

  if (arg != NULL)
  {
    Serial.print("SCI: Debug: The argument was: ");
    Serial.println(arg);
  }
  else {
    Serial.println("No argument.");
  }
  Serial.println("SCI: Debug: Exiting debug system.");
}

void unrecognized() // Unrecognized command
{
  Serial.println("SCI: Error: Command not found. Use the HELP command for assitance.");
}




