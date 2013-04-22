#include <OneWire.h>
#include <DallasTemperature.h>

const int LDR_Sensor = A1;
const int ldr_threshold = 350;
const int sensing_delay = 15000; // read value every 1 sec.
const int light_sensor_id = 13;



#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature temp_sensors(&oneWire);
const int temp_sensor_id = 31;


// these variables will change:
int ldrReading = 0;
int light = LOW;

int tmpReading = 0;

void setup() {
   Serial.begin(9600);       // use the serial port
   
   // Start up the temp library
   temp_sensors.begin();
}

void loop() {
  //Serial.println("Reading LDR");
  ldrReading = analogRead(LDR_Sensor); 
  //Serial.println(ldrReading);
  //Serial.println("LDR Reading Done");
  
  //Serial.println("Sending LDR Reading");
  if (ldrReading >=ldr_threshold && light == LOW) 
  {
    Serial.write(light_sensor_id);
    Serial.write((byte)0xFF);
    light = HIGH;
  }
  else if(ldrReading <ldr_threshold && light == HIGH)
  {
    Serial.write(light_sensor_id);
    Serial.write((byte)0x00);
   
  light = LOW;
  }  
  //Serial.println("LDR Reading Sent.");
  
  //Serial.print("Requesting temperatures...");
  temp_sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");

  //Serial.print("Temperature for Device 1 is: ");
  tmpReading = temp_sensors.getTempCByIndex(0);
  //Serial.print(tmpReading); // Why "byIndex"? 
  //Serial.println();
  
  //Serial.println("Sending Temp Reading");
  Serial.write(temp_sensor_id);
  Serial.write(tmpReading/256);
  Serial.write(tmpReading%256);
  
  //Serial.println("Sent.");
  

  
  
  
  // delay to avoid overloading the serial port buffer
  
  delay(sensing_delay);
}

