#SimpleHomeWSN

A simple home wireless sensor network sensing the room temperature and if the room's light is on or not, and uploading this live data to an online feed so the house can be monitored remotely even from mobile phones.

This project is presented for the Wireless Sensor Networks course at AAST.

Its goal is to show an example of simple implementations of WSN using Arduino, Xbee and Online Data feeds.


#The Network Structure
this WSN structure can be modified , here is what i used 

#Network Protocol

* Xbee has an API for framing and sending the data reliably but i preferred not to use it and created a simple protocol of my own.

* the frame consists of 

  * Start byte **0x7E** ~ the same as in the Xbee API.
	* Node Id **0x33** , **0x34** ~ for room 1 and room 2.
	* Sensor ID **0x13** , **0x31** ~ for light and temperature.
	* MS byte **0x00** ~ Most Significant byte of the value read. [ probably useless because all the data read can fit in one byte , but since i was sending int it was logical at that time to send 2 bytes and i left it that way].
	* LS byte **0** ~  Least Significant byte of the data.
	* End byte   **0xE7** ~ the reverse of the start bit.
	


* 2 Sensor nodes with only one of them equipped with temperature sensors *[ 'could get another one in time :)']*
* The base node.

#Base Node

The base node is a computer connected with a  Xbee Module connected via [Spark Fun Xbee Explorer][3].


A python script reads the frames sent from the sensor nodes and uploads them to [Xively Cloud][4]

**External libraries used :**

* [pySerial][5] to read serial data from the Xbee.
* [Xively-python][6] to upload the data to Xively online cloud.




#Sensor Node

**Hardware:**

* An LDR to sense the presence of light.
* A Dallas 18b20 digital temperature sensor.
* Xbee S2 Transmitter.
* Xbee Shield for arduino.
* Arduino Uno Board.



**External libraries used :**

* [One Wire Library][1] 
* [Arduino Temperature Control Library][2] 
* Both used for reading the temperature from the sensor.
* I bundled the external libraries in this repo in case the external libraries have any future changes that might affect this application.


#Notes

* Xively has a library that supports sending data directly from Arduino via Ethernet shields. i was planning to use it before they update their services so i had no time to rewrite the node code. 

* this can be better for a home application without the need for a server computer to be on all the time.





  [1]: http://www.pjrc.com/teensy/td_libs_OneWire.html
  [2]: https://github.com/milesburton/Arduino-Temperature-Control-Library
  [3]: https://www.sparkfun.com/products/8687
  [4]: https://xively.com
  [5]: http://pyserial.sourceforge.net/
  [6]: https://github.com/xively/xively-python
