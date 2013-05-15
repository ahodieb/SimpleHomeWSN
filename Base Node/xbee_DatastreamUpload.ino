#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Cosm.h>



#define API_KEY "26PhWExTPYNIs1UoW3DH6GuqZN2SAKxaMlpDM3B6MTVrVT0g" // your Cosm API key
#define FEED_ID 126694 // your Cosm feed ID

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char sensorId[] = "RoomLight";
CosmDatastream datastreams[] = {
  CosmDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};

// Wrap the datastream into a feed
CosmFeed feed(FEED_ID, datastreams, 1 /* number of datastreams */);

EthernetClient client;
CosmClient cosmclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  
  Serial.println("Initializing network");
  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }

  Serial.println("Network initialized");
  Serial.println();
}

void loop() {
  byte incoming;
  int node_id = 0;
  int reading = 0;
  
  

  
  if (Serial.available() > 1) {
                // read the incoming byte:
                incoming = Serial.read();
                node_id = incoming;
                // say what you got:
                Serial.print("ID ");
                Serial.println(incoming, DEC);
                
                incoming = Serial.read();
                reading = incoming;
                Serial.print("Value ");
                Serial.println(incoming);
                
                sendData(reading);
                
               Serial.println();
               delay(15000);
                
  }
  
}

void sendData(int sensorValue) {
  datastreams[0].setFloat(sensorValue);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  Serial.println("Uploading to Cosm");
  int ret = cosmclient.put(feed, API_KEY);
  Serial.print("PUT return code: ");
  Serial.println(ret);

  Serial.println();
}
