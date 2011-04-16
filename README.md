RESTduino
=========

(note: RESTduino is very beta, expect surprises for awhile yet)

RESTduino is a simple sketch to provide a REST-like interface to the Arduino via the Ethernet Shield.  The idea is to allow developers familiar with interacting with REST services a way to control physical devices using the Arduino without having to write any Arduino code.

Of course some flexibility is traded for this convinience, only basic operations are currently supported:

* Digital pin I/O
* Analog pin input

Later versions of the sketch may provide additional functionality (PWM, servo control, etc.) however if you need more than just basic pin control you're probably better off learning the basics of programming the Arduino and offloading some of your processing to the board itself.

Getting Started
---------------

First you'll need an Arduino, the Wiznet-based Ethernet shield and the Arduino development tools; here's some links to get you started:

* Arduino Uno (adafruit): http://www.adafruit.com/index.php?main_page=product_info&cPath=17&products_id=50
* Ethernet Shield (adafruit): http://www.adafruit.com/index.php?main_page=product_info&cPath=17_21&products_id=201
* Arduino development tools: http://www.arduino.cc/en/Main/Software

For testing you'll want some hardware to connect to the Arduino (a green LED is enough to get started).  Connect the LED between pins 9 and ground (GND).

Load up the sketch (RESTduino.pde) and modify the following lines to match your setup:

This line sets the MAC address of your ethernet board; if your board has one written on it, you should use that instead:
byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

The next line you'll need to modify is this one which sets the IP address; set it to something valid for your network:
byte ip[] = {192,168,1,177};

Now you should be ready to upload the code to your Arduino.  Once the upload is complete you can open the "Serial Monitor" to get some debug info from the sketch.

Now you're ready to start talking REST to your Arduino!

To turn on the LED attached to pin #9:

http://192.168.1.177/9/HIGH

This will set the pin to the HIGH state and will again test the state of the pin and return the state as JSON (useful for troubleshooting):

{"9":"1"}

To read the value of digital pin #9:

http://192.168.1.177/9

This returns a tiny chunk of JSON containing the pin requested and its current value:

{"9":"0"}

(note that the return values are integers; I'm still torn on what to "standardize" on in regard to digital values)

Analog reads are simular; reading the value of Analog pin #1 looks like this:

http://192.168.1.177/a1

...and return the same JSON formatted result as above:

{"a1":"432"}

(I'm not sure if the analog reads are working correctly yet, my test equiptment is incomplete at the moment)
