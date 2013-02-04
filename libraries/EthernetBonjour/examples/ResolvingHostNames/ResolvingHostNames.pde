//  Copyright (C) 2010 Georg Kaindl
//  http://gkaindl.com
//
//  This file is part of Arduino EthernetBonjour.
//
//  EthernetBonjour is free software: you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public License
//  as published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  EthernetBonjour is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with EthernetBonjour. If not, see
//  <http://www.gnu.org/licenses/>.
//

//  Illustrates how to resolve host names via MDNS (Multicast DNS)

#if defined(ARDUINO) && ARDUINO > 18
#include <SPI.h>
#endif
#include <Ethernet.h>
#include <EthernetBonjour.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// substitute an address on your own network here
byte ip[] = { 192, 168, 0, 154 };

// NOTE: Alternatively, you can use the EthernetDHCP library to configure your
//       Ethernet shield.

const char* ip_to_str(const uint8_t*);
void nameFound(const char* name, const byte ipAddr[4]);

void setup()
{
  Ethernet.begin(mac, ip);
  
  // Initialize the Bonjour/MDNS library. You can now reach or ping this
  // Arduino via the host name "arduino.local", provided that your operating
  // system is Bonjour-enabled (such as MacOS X).
  // Always call this before any other method!
  EthernetBonjour.begin("arduino");

  // We specify the function that the Bonjour library will call when it
  // resolves a host name. In this case, we will call the function named
  // "nameFound".
  EthernetBonjour.setNameResolvedCallback(nameFound);

  Serial.begin(9600);
  Serial.println("Enter a Bonjour host name via the Arduino Serial Monitor to "
                 "have it resolved.");
  Serial.println("Do not postfix the name with \".local\"");
}

void loop()
{ 
  char hostName[512];
  int length = 0;
  
  // read in a host name from the Arduino IDE's serial monitor.
  while (Serial.available()) {
    hostName[length] = Serial.read();
    length = (length+1) % 512;
    delay(5);
  }
  hostName[length] = '\0';
  
  // You can use the "isResolvingName()" function to find out whether the
  // Bonjour library is currently resolving a host name.
  // If so, we skip this input, since we want our previous request to continue.
  if (!EthernetBonjour.isResolvingName()) {
    if (length > 0) {    
      byte ipAddr[4];

      Serial.print("Resolving '");
      Serial.print(hostName);
      Serial.println("' via Multicast DNS (Bonjour)...");

      // Now we tell the Bonjour library to resolve the host name. We give it a
      // timeout of 5 seconds (e.g. 5000 milliseconds) to find an answer. The
      // library will automatically resend the query every second until it
      // either receives an answer or your timeout is reached - In either case,
      // the callback function you specified in setup() will be called.

      EthernetBonjour.resolveName(hostName, 5000);
    }  
  }

  // This actually runs the Bonjour module. YOU HAVE TO CALL THIS PERIODICALLY,
  // OR NOTHING WILL WORK! Preferably, call it once per loop().
  EthernetBonjour.run();
}

// This function is called when a name is resolved via MDNS/Bonjour. We set
// this up in the setup() function above. The name you give to this callback
// function does not matter at all, but it must take exactly these arguments
// (a const char*, which is the hostName you wanted resolved, and a const
// byte[4], which contains the IP address of the host on success, or NULL if
// the name resolution timed out).
void nameFound(const char* name, const byte ipAddr[4])
{
  if (NULL != ipAddr) {
    Serial.print("The IP address for '");
    Serial.print(name);
    Serial.print("' is ");
    Serial.println(ip_to_str(ipAddr));
  } else {
    Serial.print("Resolving '");
    Serial.print(name);
    Serial.println("' timed out.");
  }
}

// This is just a little utility function to format an IP address as a string.
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}
