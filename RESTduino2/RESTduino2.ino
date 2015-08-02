#define DEBUG false
#define STATICIP false
#define BUFSIZE 255

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include <SD.h>

// configuration
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char hostname[] = "restduino";

EthernetServer server(80);

void setup(){
  #if DEBUG
    Serial.begin(9600);
  #endif
  
  if(Ethernet.begin(mac) == 0){
    #if DEBUG
      Serial.println("Unable to configure network address using DHCP");
    #endif
      // todo: turn on a light or something to indicate we're not working
      // sit and spin...
      for(;;)
        ;
  }
  
  #if DEBUG
    Serial.println(Ethernet.localIP());
  #endif
  
  server.begin();
  EthernetBonjour.begin(hostname);
}

void loop(){
  // keep bonjour in the loop (ha ha ha)
  EthernetBonjour.run(); 
  char clientline[BUFSIZE];
  int index = 0;
  EthernetClient client = server.available();
  if(client){
    // reset input buffer position
    index = 0;
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        if(c != '\n' && c != '\r' && index < BUFSIZE){
          clientline[index++] = c;
          continue;
        }
        client.flush();
      }
      // todo: extract VERB from client request
      // todo: extract path from client request
      // todo: extract parameters from the client request
      // todo: extract the BODY from the client request
      // todo: *do* something with the request
      // todo: return the result to the client
      
      // give the client time to receive the result
      delay(1);
      
      // close the connection
      client.stop();
      while(client.status() != 0){
        delay(5);
      }
    }
  }
}
