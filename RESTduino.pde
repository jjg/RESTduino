/*
  RESTduino
 
 A REST-style interface to the Arduino via the 
 Wiznet Ethernet shield.
 
 Based on David A. Mellis's "Web Server" ethernet
 shield example sketch.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 04/12/2011
 by Jason J. Gullickson
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192,168,1,177};

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
Server server(80);

void setup()
{
  //  turn on serial (for debuggin)
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

//  url buffer size
#define BUFSIZE 255

void loop()
{
  char clientline[BUFSIZE];
  int index = 0;

  // listen for incoming clients
  Client client = server.available();
  if (client) {

    //  reset input buffer
    index = 0;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //  fill url the buffer
        if(c != '\n' && c != '\r'){
          clientline[index] = c;
          index++;

          //  if we run out of buffer, overwrite the end
          if(index >= BUFSIZE)
            index = BUFSIZE -1;

          continue;
        } 

        //  convert clientline into a proper
        //  string for further processing
        String urlString = String(clientline);

        //  extract the operation
        String op = urlString.substring(0,urlString.indexOf(' '));

        //  we're only interested in the first part...
        urlString = urlString.substring(urlString.indexOf('/'), urlString.indexOf(' ', urlString.indexOf('/')));

        //  put what's left of the URL back in client line
        urlString.toCharArray(clientline, BUFSIZE);

        //  get the first two parameters
        char *pin = strtok(clientline,"/");
        char *value = strtok(NULL,"/");

        //  this is where we actually *do something*!
        char outValue[10];
        String jsonOut = String();

        if(pin != NULL){
          if(value != NULL){

            //  set the pin value
            Serial.println("setting pin");
            
            //  select the pin
            int selectedPin = pin[0] -'0';
            Serial.println(selectedPin);
            
            //  set the pin for output
            pinMode(selectedPin, OUTPUT);
            
            //  determine digital or analog (PWM)
            if(strncmp(value, "HIGH", 4) == 0 || strncmp(value, "LOW", 3) == 0){
              
              //  digital
              Serial.println("digital");
              
              if(strncmp(value, "HIGH", 4) == 0){
                Serial.println("HIGH");
                digitalWrite(selectedPin, HIGH);
              }
              
              if(strncmp(value, "LOW", 3) == 0){
                Serial.println("LOW");
                digitalWrite(selectedPin, LOW);
              }
              
            } else {
              
              //  analog
              Serial.println("analog");
              
              //  get numeric value
              int selectedValue = atoi(value);              
              Serial.println(selectedValue);
              
              analogWrite(selectedPin, selectedValue);
              
            }
 
            //  return status
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            
          } 
          else {

            //  read the pin value
            Serial.println("reading pin");

            //  determine analog or digital
            if(pin[0] == 'a' || pin[0] == 'A'){

              //  analog
              int selectedPin = pin[1] - '0';

              Serial.println(selectedPin);
              Serial.println("analog");

              sprintf(outValue,"%d",analogRead(selectedPin));
              
              Serial.println(outValue);

            } 
            else if(pin[0] != NULL) {

              //  digital
              int selectedPin = pin[0] - '0';

              Serial.println(selectedPin);
              Serial.println("digital");

              pinMode(selectedPin, INPUT);
              sprintf(outValue,"%d",digitalRead(selectedPin));
              
              Serial.println(outValue);
            }

            //  assemble the json output
            jsonOut += "{\"";
            jsonOut += pin;
            jsonOut += "\":\"";
            jsonOut += outValue;
            jsonOut += "\"}";

            //  return value
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println(jsonOut);
          }
        } 
        else {
          
          //  error
          Serial.println("erroring");
          client.println("HTTP/1.1 404 Not Found");
          client.println("Content-Type: text/html");
          client.println();
          
        }
        break;
      }
    }

    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
  }
}


