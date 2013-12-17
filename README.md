DigiForensics
=============

USB tool for memory imaging orignally intended for the Digispark.

WARNING THIS TOOL IS STILL IN DEVELOPEMENT AND IS NOT STABLE!

I'm currently going to be proceeding with the teensy until I can get the keyboard library adapted for my specific use.
This recently has been proving to be a roadblock. If you would Like to contribute to the project this will definitely be a ideal task.
You can contact me at CalebCoffie@gmail.com for more details.

Check out https://www.CalebCoffie.com for more details on the project.

Install
=======

To install DigiForensics you will want to start off by installing the Arduino software from here: http://arduino.cc/en/Main/Software

You will then want to install the Teensyduino addon from here: http://www.pjrc.com/teensy/teensyduino.html

This will give you all the tools you need to program your teensy. The next step is then configuring your Arduino IDE to program your Teensy. This can be done by changing a couple settings in the menus. First switch the selected board to Teensy 2.0 or which ever teensy you are using in the following menu- Tools > Board Type > Teensy 2.0. Then you will need to switch the USB Type. This should be switched to Keyboard + Mouse + Joystick. This can be done in the following menu- Tools > USB Type > Keyboard + Mouse + Joystick. This is all you'll have to do to setup arduino.
 
You can now open the code and change a couple things to work in your configuration. One thing you'll want to make sure you change is the STORAGE_SERIAL constant in the code. This serial number can be found by plugging the storage device into a windows computer. 
Bring up a command prompt and enter the following command and you'll see the serial number listed below.

wmic logicaldisk get deviceid, VolumeSerialNumber

Change the Serial Number in the code and you should be ready to flash your teensy. All you have to do is press the arrow key in the arduino IDE whileyour teensy is plugged in. This will program your teensy.

The next step is to set up you external storage. This will need to be the same storage device you configured above. Simply copy all of the contents of the disk folder into the root of your storage device. This is all you need to do for the storage device.

The next step that I haven't mention yet is that you will need a button soldered onto you Teensy. This button is the trigger button. You can find out more about it in the blog post listed here: https://calebcoffie.com/digiforensics-a-usb-memory-imager/

The last and final step is to test it out. This can be done simply by plugging both the Teensy and the external storage into a 32bit windows system and pressing and holding the button for more than 3 seconds. This will then rip a runing copy of memory onto the external storage. If you get a copy of memory it's successfully set up. If not feel free to contact me and I can try and help you troubleshoot it.

Feel free to look in the code. I tried to make it really readable.
