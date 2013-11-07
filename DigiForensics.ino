/*
	DigiForensics
 	USB tool for memory imaging orignally intended for the Digispark.
 	Developed by Caleb Coffie <CalebCoffie@gmail.com>
 	Special thanks to the people at Offensive Security, Digistump, and PJRC.
 
 	For more information check out https://CalebCoffie.com
 */

#include <usb_private.h>

//Storage "VolumeSerialNumber"
const String STORAGE_SERIAL = "02B4C4D4";

//Trigger Pin
const int trigger_pin = 23;

//Teensy has LED on 11
const int led_pin = 11;

//Waits till device is ready. Allows you to gurantee you're not sending data before the computer is ready for it.
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

//NUM, SCROLL, CAPS Led keys checking. We only use NUMLOCK in this sketch. 
int ledkeys(void)       {return int(keyboard_leds);}
bool is_scroll_on(void) {return ((ledkeys() & 4) == 4) ? true : false;}
bool is_caps_on(void)   {return ((ledkeys() & 2) == 2) ? true : false;}
bool is_num_on(void)    {return ((ledkeys() & 1) == 1) ? true : false;}

//Depresses all keys by sending nothing.
void unpress_key(void)
{
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(500);
}

//Blinks to provide visual feedback on the device
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
// The "reps" and millisecs" variables are fed to this function from other functions that require timing.
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

//Check if numlock is on. If so turns it off.
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

//Checks if numlock is off. If so turns it on.
void make_sure_numlock_is_on(void)
{
  if (!is_num_on())
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
  int num_of_presses = 0;
  for (int x=0; x<3; x++)
  {
    if(!digitalRead(trigger_pin))
    {
      num_of_presses++;
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

//Open command prompt
void open_command_prompt()
{
  //Opens start menu
  Keyboard.set_modifier(MODIFIERKEY_LEFT_GUI);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.send_now();
  delay(1500);
  
  //Open cmd.exe with search field
  Keyboard.print("cmd.exe");
  delay(200);
  Keyboard.set_key1(KEY_ENTER);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(2000);  
}

//Find drive letter with given volume name
//returns drive letter as a string
void change_to_storage_directory(String volume_serial)
{
  // Command that need to be passed:
  // FOR /F %D IN ('wmic logicaldisk where "VolumeSerialNumber='02B4C4D4'" get deviceid') do %D
  // Not exactly the most graceful way of doing it but it does work
  Keyboard.println("FOR /F %D IN (\'wmic logicaldisk where \"VolumeSerialNumber=\'" + volume_serial + "\'\" get deviceid\') do %D");
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
      
//      //Simple testing if program is being triggered
//      Keyboard.print("triggered");
//      Keyboard.set_key1(KEY_ENTER);
//      Keyboard.send_now();
//      Keyboard.set_key1(0);
//      Keyboard.send_now();
      
      //Minimizes all windows to provide a fresh start
      reset_windows_desktop(200);
      
      //Opens command prompt
      open_command_prompt();
      
      //Get executables off storage / Access them
      //Exploring possibilities of whether to type them from internal storage
      //or just copy them from sd storage. Typing them out will provide better compatibility
      //but the device might not have enough storage.
      //Below I decided to just access the binaries on the sd card
      change_to_storage_directory(STORAGE_SERIAL);
      delay(1000);  //prevents first character from being cut off
      
      //Keyboard.println("dir");  //For testing purposes
      
      //Get Administrative Priveleges
      //using this exploit (http://www.exploit-db.com/exploits/25912/) to get system prompt.
      //Above exploit only works with 32 bit systems. So I will have to explore alternatives for 64 bit
      //privilege escalation on Windows.
      
      
      //Launch Memory Imager
      
      
      //Locate storage for memory image and place it there
      
      
      //Clean up activities. Not sure what but just a reminder
      
      
    }
  }
}

//Main loop required by arduino
//All execution of the program is happening in the setup loop above
void loop(void){}

