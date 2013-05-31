#include <OneWire.h> 
#include <DallasTemperature.h>


// Transmitiion Constants
const byte start_byte = 0x7E;
const byte end_byte   = 0xE7;

//Node Constants
const byte node_id    = 0x33;
const long delay_interval = 3000;

//Functions Declartions
void flash(int led,int times);

// Light Sensing constants
const byte light_sensor_id = 0x13;
const int ldr_sensor_pin = A1; // pin for sensing light
const int ldr_threshold = 350; // threashold to determine light or dark.

// Light Sensing Variables
int ldr_reading = 0;
byte light = 0x00;
boolean send_light = false;


//Temperature Sensing Constants
const byte temp_sensor_id = 0x13;
const int temp_sensing_max = 10;
int one_wire_bus = 2;
OneWire oneWire(one_wire_bus);
DallasTemperature temp_sensors(&oneWire);

//Temperature Sensing Variables
int temp_sensing_count = 10;
int temp_reading = 0;
int old_temp_reading = 0;
boolean send_temp = false;



void setup() {
   Serial.begin(9600);
   temp_sensors.begin();
   pinMode(12,OUTPUT);
   pinMode(13,OUTPUT);
}

void loop(){
  
  
  //Sensing
  
  
  //Light Sensing
  ldr_reading = analogRead(ldr_sensor_pin); 
  if(ldr_reading >=ldr_threshold && light == 0x00)
  {
    light = 0xFF;
    send_light = true;
  }
  else if(ldr_reading <ldr_threshold && light == 0xFF)
  {
    light = 0x00;
    send_light = true;
  }
  
  
  
  //Temperature Sensing
  temp_sensors.requestTemperatures();
  temp_reading = temp_sensors.getTempCByIndex(0);
  temp_sensing_count ++
  if(temp_reading != old_temp_reading || temp_sensing_count > temp_sensing_max){
    send_temp = true;
    old_temp_reading = temp_reading;
  }  
  //Sending
  
  if (send_light){
    flash(13,3);
    Serial.write(start_byte);
    Serial.write(node_id);
    Serial.write(light_sensor_id);
    Serial.write((byte)0x00);
    Serial.write((byte)light);
    Serial.write(end_byte);
    flash(13,3);    
    send_light = false;
  }
  
    if (send_temp){
    flash(13,1);
    Serial.write(start_byte);
    Serial.write(node_id);
    Serial.write(temp_sensor_id);
    Serial.write((byte)temp_reading/256);
    Serial.write((byte)temp_reading%256);
    Serial.write(end_byte);
    flash(13,2);    
    send_temp = false;
  }

  delay(delay_interval);
  Serial.flush();

}

void flash(int led,int times)
{
  for (int i = 0 ; i < times ; i++){
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
    delay(100);
  }
}
