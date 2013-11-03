/*
	DigiForensics
 	USB tool for memory imaging orignally intended for the Digispark.
 	Developed by Caleb Coffie <CalebCoffie@gmail.com>
 	Special thanks to the people at offensive Security and Digistump
 
 	For more information check out https://CalebCoffie.com
 */

#include <usb_private.h>

// Trigger Pin
const int trigger_pin = 23;

// Teensy has LED on 11
const int led_pin = 11;

void wait_for_drivers(unsigned int speed)
{
	bool numLockTrap = is_num_on();
	while(numLockTrap == is_num_on())
	{
		blink_fast(5,80);
		press_numlock();
		unpress_key();
		delay(speed);
	}
	press_numlock();
	unpress_key();
	delay(speed);
}

// NUM, SCROLL, CAPS Led keys checking. We only use NUMLOCK in this sketch. 
int ledkeys(void)       {return int(keyboard_leds);}
bool is_scroll_on(void) {return ((ledkeys() & 4) == 4) ? true : false;}
bool is_caps_on(void)   {return ((ledkeys() & 2) == 2) ? true : false;}
bool is_num_on(void)    {return ((ledkeys() & 1) == 1) ? true : false;}

void unpress_key(void)
{
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(500);
}

void blink_fast(int blinkrate,int delaytime)
{
  int blinkcounter=0;
  for(blinkcounter=0; blinkcounter!=blinkrate; blinkcounter++)
  {
    digitalWrite(led_pin, HIGH);
    delay(delaytime);
    digitalWrite(led_pin, LOW);
    delay(delaytime);
  }
}

// A Teensy side check for a pressed numlock key. Will check for a pressed numlock key (reps) times, with (millisecs) milliseconds in between checks.
// The "reps" and millisecs" variables are fed to this function from other functions that require timing. For example:
bool check_for_numlock_sucess_teensy(int reps, int millisecs)
{
  unsigned int i = 0;
  do
  {
    delay(millisecs);
    if (is_num_on())
    {
      make_sure_numlock_is_off();
      delay(700);
      return true;
    }
    i++;
  }
  while (!is_num_on() && (i<reps));
  return false;
}

// Minimises all windows
void minimise_windows(void)
{
  Keyboard.set_modifier(MODIFIERKEY_RIGHT_GUI);
  Keyboard.set_key1(KEY_M);
  Keyboard.send_now();
  delay(300);
  unpress_key();
  delay(300);
}

// Minimises all windows 3 times with (sleep) milliseconds in between. Used to provide a fresh start.
void reset_windows_desktop(int sleep)
{
  delay(1000);
  minimise_windows();
  delay(sleep);
  minimise_windows();
  delay(sleep);
  minimise_windows();
  delay(200);
}

//Command to toggle Numlock. Used often to verify actions. 
void press_numlock(void)
{
  Keyboard.set_key1(KEY_NUM_LOCK);
  Keyboard.send_now();
  delay(200);
}

void make_sure_numlock_is_off(void)
{
  if (is_num_on())
  {
    delay(500);
    press_numlock();
    delay(700);
    unpress_key();
    delay(700);
  }
}

//trigger the program
//requires that button be pressed for 3 seconds before triggering program
bool trigger(void)
{
  Keyboard.print("trigger sequence entered");
  Keyboard.set_key1(KEY_ENTER);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();
  int num_of_presses = 0;
  for (int x; x<3; x++)
  {
     Keyboard.print(!digitalRead(trigger_pin));
     Keyboard.set_key1(KEY_ENTER);
     Keyboard.send_now();
     Keyboard.set_key1(0);
     Keyboard.send_now();
    if(digitalRead(trigger_pin))
    {
      num_of_presses++;
      Keyboard.print("Sequence- " + num_of_presses);
      Keyboard.set_key1(KEY_ENTER);
      Keyboard.send_now();
      Keyboard.set_key1(0);
      Keyboard.send_now();
      delay(1000);
    }
    else
    {
      delay(1000);
    }
  }
  if(num_of_presses == 3)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//Main sequence
//Only runs through once
void setup(void)
{
  Serial.begin(9600);
  //Setup LED pin
  pinMode(led_pin, OUTPUT);
  //Setup trigger pin
  pinMode(trigger_pin, INPUT_PULLUP);
  bool triggered = false;

  //Wait till device and computer are ready
  wait_for_drivers(2000);
  
  //Execute main sequence of program
  while (!triggered)
  {
    if (trigger())
    {
      triggered = true; //stop execution of payload more than once
      //Execute main sequence below
      Keyboard.print("triggered");
      Keyboard.set_key1(KEY_ENTER);
      Keyboard.send_now();
      Keyboard.set_key1(0);
      Keyboard.send_now();
    }
  }
}

//Main loop required by arduino
//All execution of the program is happening in the setup loop above
void loop(void){}

