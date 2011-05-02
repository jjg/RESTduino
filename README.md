RESTduino
=========

RESTduino is a simple sketch to provide a REST-like interface to the Arduino via the Ethernet Shield.  The idea is to allow developers familiar with interacting with REST services with a way to control physical devices using the Arduino (without having to write any Arduino code).

You can see a crude demo video on YouTube here: http://www.youtube.com/watch?v=X-s2se-34-g

Of course some flexibility is traded for this convenience; only basic operations are currently supported:

* Digital pin I/O (HIGH, LOW and PWM)
* Analog pin input

Later versions of the sketch may provide additional functionality (servo control, etc.) however if you need more than just basic pin control you're probably better off learning the basics of programming the Arduino and offloading some of your processing to the board itself.

Getting Started
---------------

First you'll need an Arduino, a Wiznet-based Ethernet shield and the Arduino development tools; here's some links to get you started:

* Arduino Uno (adafruit): http://www.adafruit.com/index.php?main_page=product_info&cPath=17&products_id=50
* Ethernet Shield (adafruit): http://www.adafruit.com/index.php?main_page=product_info&cPath=17_21&products_id=201
* Arduino development tools: http://www.arduino.cc/en/Main/Software

For testing you'll want some hardware to connect to the Arduino (a green LED is enough to get started).  Connect the LED between pins 9 and ground (GND).

Load up the sketch (RESTduino.pde) and modify the following lines to match your setup:

byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

This line sets the MAC address of your ethernet board; if your board has one written on it, you should use that instead.

byte ip[] = {192,168,1,177};

The next line you'll need to modify is this one which sets the IP address; set it to something valid for your network.

Now you should be ready to upload the code to your Arduino.  Once the upload is complete you can open the "Serial Monitor" to get some debug info from the sketch.

Now you're ready to start talking REST to your Arduino!

To turn on the LED attached to pin #9 (currently case sensitive!):

http://192.168.1.177/9/HIGH

This will set the pin to the HIGH state and the LED should light.  Next try this:

http://192.168.1.177/100

This will use PWM to illuminate the LED at around 50% brightness (valid PWM values are 0-255).

Now if we connect a switch to pin #9 we can read the digital (on/off) value using this URL:

http://192.168.1.177/9

This returns a tiny chunk of JSON containing the pin requested and its current value:

{"9":"LOW"}

Analog reads are similar; reading the value of Analog pin #1 looks like this:

http://192.168.1.177/a1

...and return the same JSON formatted result as above:

{"a1":"432"}

Javascript/jQuery Demo
----------------------
A simple example of how to interface with RESTduino via jQuery is included as DemoApp.html.  

This page displays a slider control (via jQuery UI) which when adjusted will set the PWM value of Pin #9 to the value selected by the slider.

If you look at line 19 you can see where the REST URL (you'll need to adjust this for the IP address of your device) is constructed based on the selected value of the slider and on line 22 an AJAX request is executed passing the URL constructed above to the Arduino.
