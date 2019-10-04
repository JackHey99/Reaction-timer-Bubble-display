#include <Arduino.h>

#include <SevSeg.h>

SevSeg myDisplay;

//Create global variables
bool LED_state = LOW;
volatile bool pause_button_pressed = 0;
volatile bool reset_button_pressed = 0;
uint32_t debouncedelay_ms = 100;
uint32_t BUTTON_PIN = 22;
uint32_t RESET_BUTTON_PIN = 20;
uint32_t LED_PIN = 19;
uint32_t random_start_time_ms = 0;
uint32_t current_time_ms = 0;
unsigned long timer_us = 0;
int display_value_us = 0;

void ISR_pause_button_pressed();
void ISR_reset_button_pressed();

//------------------------------------------//

void setup()

{
  Serial.begin(9600);
  randomSeed(analogRead(9));              // Used for creating random start time
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN, ISR_pause_button_pressed, FALLING);
  attachInterrupt(RESET_BUTTON_PIN, ISR_reset_button_pressed, FALLING);
  pinMode(LED_PIN, OUTPUT);

  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode

  //Declare what pins are connected to the GND pins (cathodes)

  uint8_t digit1 = 2; //Pin 2
  uint8_t digit2 = 3; //Pin 3
  uint8_t digit3 = 1; //Pin 1
  uint8_t digit4 = 0; //Pin 0

  //Declare what pins are connected to the segments (anodes)

  uint8_t segA = 8; //Pin 8
  uint8_t segB = 9; //Pin 9
  uint8_t segC = 17; //Pin 17
  uint8_t segD = 11; //Pin 11
  uint8_t segE = 18; //Pin 18
  uint8_t segF = 10; //Pin 10
  uint8_t segG = 12; //Pin 12
  uint8_t segDP = 16; //Pin 16

  uint8_t numberOfDigits = 4; //4 digit display

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);

  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

  timer_us = micros();
  current_time_ms = millis();
}

void loop()

{

  random_start_time_ms = random(4000, 11000);           //Create a random number used for start time

  if ((millis() - current_time_ms) > random_start_time_ms)
  {
    digitalWrite(LED_PIN, HIGH);
    LED_state = HIGH;
    char tempString[10]; //Used for sprintf
    sprintf(tempString, "%04d", display_value_us); //Convert display_value_us into a string

    //Produce an output on the display
    myDisplay.DisplayString(tempString, 2); //(numberToDisplay, decimal point location in binary number)

    if (pause_button_pressed == false)
    {
      if (micros() - timer_us >= 8155)       // Not 10000 as we need to adjust for process time for other tasks
      {
        timer_us = micros();
        display_value_us++;
        /* while (display_value_us >= 2000)
        {
          pause_button_pressed = true;
          digitalWrite(LED_PIN, LOW);
          char tempString[10]; //Used for sprintf

          sprintf(tempString, "%4d", display_value_us); //Convert display_value_us into a string that is right adjusted

          //Produce an output on the display

          myDisplay.DisplayString(tempString, 2); //(numberToDisplay, decimal point location in binary number [4 means the third digit])
        } */
      }
    }
    /* while (pause_button_pressed == true)
    {
      digitalWrite(LED_PIN, LOW);
      char tempString[10]; //Used for sprintf

      sprintf(tempString, "%4d", display_value_us); //Convert display_value_us into a string that is right adjusted

      //Produce an output on the display

      myDisplay.DisplayString(tempString, 2); //(numberToDisplay, decimal point location in binary number [4 means the third digit])
      
    } */
  }

  if (reset_button_pressed == true)
  {
    current_time_ms = millis();
    digitalWrite(LED_PIN, LOW);
    reset_button_pressed = false;
    display_value_us = 0;           //resets the time on the display
    pause_button_pressed = false;
    LED_state = LOW;
  }
}

void ISR_pause_button_pressed()
{
  noInterrupts();
  uint32_t currenttime_ms = millis();
  static uint32_t previoustime_ms = 0;

  if (((currenttime_ms - previoustime_ms) > debouncedelay_ms) && LED_state == HIGH)
  {
    pause_button_pressed = HIGH;
    previoustime_ms = currenttime_ms;
  }
  interrupts();
}

void ISR_reset_button_pressed()
{
  uint32_t currenttime_ms = millis();
  static uint32_t previoustime_ms = 0;

  if ((currenttime_ms - previoustime_ms) > debouncedelay_ms)
  {
    reset_button_pressed = HIGH;
    previoustime_ms = currenttime_ms;
  }
}
