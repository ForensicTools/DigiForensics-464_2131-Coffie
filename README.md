DigiForensics
=============

USB tool for memory imaging orignally intended for the Digispark.

WARNING THIS TOOL IS STILL IN DEVELOPEMENT AND IS NOT STABLE!

I'm currently going to be proceeding with the teensy until I can get the keyboard library adapted for my specific use.
This recently has been proving to be a roadblock. If you would Like to contribute to the project this will definitely be a ideal task.
You can contact me at CalebCoffie@gmail for more details.

Check out https://www.CalebCoffie.com for more detail on the project.

Install
=======

This section is still in it's beginning stages.

One thing you'll want to make sure you change is the STORAGE_SERIAL constant in the code. This serial can be found by plugging the storage device into a windows computer. 
Bring up a command prompt and enter the following command and your see the serial number listed below.

wmic logicaldisk get deviceid, VolumeSerialNumber