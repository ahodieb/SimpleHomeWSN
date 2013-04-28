SimpleHomeWSN
=============

A simple home wireless sensor network sensing the room temperature and if the room light is on or not, and uploading this live data to an online feed so it can be used to monitor the house remotly even from mobile phones.

This project is presented for the Wireless Sensor Networks course in AAST.

Its goal is to show an example of simple implementations of WSN using Arduino, Xbee and Online Data feeds.

#Work in progress:
* Updating the base-station node to recive the temperature.
* update the sensing frequency in the sensor nodes.
* explore the collision behaviour of multiple nodes.
* create a fault tolerent way to send data without collision.


Sensor Node
===========

##The sensor node is equiped with:
* An LDR to sense the presence of light.
* A Dallas 18b20 digital temperature sensore.
* Xbee S2 Transmitter.
* Xbee Shield for arduino.
* Arduino Uno Board.

##External libraries used :
* [One Wire Library][1] 
* [Arduino Tempreataure Control Library][2] 
* Both used for reading the temperature from the sensor.
* I bundeld the external libraries in this repo in case the external libraries have any future changes that might affect this application.


  [1]: http://www.pjrc.com/teensy/td_libs_OneWire.html
  [2]: https://github.com/milesburton/Arduino-Temperature-Control-Library
