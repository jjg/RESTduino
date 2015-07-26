#RESTduino

RESTduino is a simple Arduino sketch that provides a REST-like interface to the Arduino via the Ethernet Shield.  The idea is to allow developers familiar with interacting with REST services a comfortable way to control physical devices using an Arduino (without having to write any Arduino code).

You can see a crude demo video on YouTube here: http://www.youtube.com/watch?v=X-s2se-34-g

Of course some flexibility is traded for this convenience; only basic operations are currently supported:

* Digital pin I/O (HIGH, LOW and PWM)
* Analog pin input

Later versions of the sketch may provide additional functionality (servo control, etc.) however if you need more than just basic pin control you're probably better off learning the basics of programming the Arduino and offloading some of the processing to the board itself.

##Getting Started

To use RESTduino you'll need some hardware, minimally:

* An Arduino
* An Arduino network interface

RESTduino was originally designed to work with the Arduino Uno and the Arduino Ethernet Shield, but since that time a number of Arduino-compatible boards with built-in network hardware have appeard.  It should be possible to make any Arduino-Uno compatible network work with RESTduino, but the boards listed below have been tested and confirmed to work:

* WildFire (comes with RESTduino pre-installed) http://shop.wickeddevice.com/product/wildfire/

*Note: if you have confirmed that RESTduino runs on additional boards, please issue a pull request to update this list, or c
reate an [Issue](https://github.com/jjg/RESTduino/issues) with the details so I can add it, thanks!*

If you've never worked with Arduino before (or have no interest in learning how to program one) I highly recommend starting with a board that comes with RESTduino preinstalled.

###Installing RESTduino###

*Note: if you purchased a board with RESTduino pre-installed you can jump to the Testing section below.*

To install RESTduino on your hardware you'll need:

* Git: http://www.git-scm.com/
* The Arduino development tools: https://www.arduino.cc/en/Main/Software
* A computer that can run the above
* Arduino hardware (Arduino, network interface, etc.)
* USB cable
* An LED (for testing)

If you haven't already, install and test the Arduino software by running the "Blink" sample sketch on your hardware (it's much easier to debug this sketch than RESTduino).  Once you have blink working you're ready for the next step.

Clone the RESTduino repository to get a copy of the code:

     git clone https://github.com/jjg/RESTduino.git

Add the custom Bonjour library to your Arduino software:

1. In the Arduino software, select Sketch -> Import Library -> Add Library from the menu
2. Inside the RESTduino directory open the libraries directory and select the EthernetBonjour directory

*note: this is a modified version of Georg Kaindl's Bonjour/Zeroconf library which can be found here: http://gkaindl.com/software/arduino-ethernet/bonjour*

Build RESTduino:

1.  Open the RESTduino.ino file inside the RESTduino directory
2.  Select Sketch -> Verify/Compile

Near the bottom of the Arduino software window you should see the message "Done compiling".  If not, run through the steps above and try again, or if you're at a loss post an [Issue here](https://github.com/jjg/RESTduino/issues) with the message you did receive and we'll try to sort it out.

Once the code compiles you're ready to install it on your hardware.  Connect the board with the USB cable and make sure the correct board and serial port are selected in the Arduino software (these will be the same settings you used to install the Blink sketch earlier).

Now select File -> Upload from the Arduino software menu to install RESTduino on your board.  When complete you'll see another message near the bottom of the Arduino software window telling you the upload is complete.

Now you're ready to test RESTduino!

###Testing RESTduino###

Connect your board to your network.  If the board uses Ethernet, this is straightforward, but you'll want to connect the board to the same network switch as your computer to keep things simple during testing.  If your board uses WiFi, you may need to do some board-specific configuration to connect the board to your WiFi network.

Once the board is connected to the network and powered-up we can try talking to it.  By default, RESTduino is configured to use DHCP to configure it's network address and Bonjour/Zeroconf to advertise it's name to the network.  To test this, try running the following command from your computer:

     ping restduino.local

You should get a response back that shows how long it took to reach your board.  If not there may be something preventing your computer from finding the RESTduino board on your network.  Try restarting the board and performing the ping test again, and if that doesn't work move on to the manual network configuration section to try doing it the hard way.

Once you're able to ping the board you can try some of the more interesting things below.  

##Useage

Once the hardware is setup and we know it's connected to the network we can use RESTduino to interact with the physical world via regular HTTP requests.  Currently RESTduino uses the GET verb for all operations (which isn't very RESTful, but it works :).

###Setting pins

To set the value of a pin, make a GET request indicating which pin to set and what value to set it to.  For example:

     curl http://restduino.local/D9/HIGH

Will set digital pin 9 to HIGH.  If you connect an LED between pin 9 and a ground pin on the board and issue the command above, it should turn on.  To turn the LED off, issue the same request with a different value:

    curl http://restduino.local/D9/LOW

In addition to HIGH and LOW, some of the digital pins can be set to values between 0 and 255.  Pin 9 supports this so if we make a request like this:

    curl http://restduino.local/D9/128

The LED will light up, but dimmer than when we set it to HIGH.  This feature is called PWM, and pins that support it are indicated with a "~" symbol on the board.  Pins that don't support PWM will still accept integer values, but they will simply go HIGH when given one.

###Reading pins

Reading pins works just like setting them but you leave off the value.  So for example, to *read* the value of pin 9:

    curl http://restduino.local/D9

will return a JSON-formatted result like this:

    {"D9":"LOW"}

This isn't very interesting with an LED attached to pin 9, but if you attach a switch instead the value returned will reflect the position of the switch (HIGH when the switch is on, LOW when the switch is off).  

The digital pins are limited to reading HIGH or LOW values, however the Arduino provides a number of analog pins that can be used to read a range between 0 and 1023.  This is useful for any kind of sensor that produces a range of values, or for things like potentiometers (aka, knobs) that can be used for variable input.

Reading an analog pin is just like reading a digital one but with a different name:

    curl http://restduino.local/A0

which returns:

    {"A0":"432"}

Analog pins can't be set to a value (they are input-only)



To turn on the LED attached to pin #9 (currently case sensitive!):

http://192.168.1.177/9/HIGH

This will set the pin to the HIGH state and the LED should light.  Next try this:

http://192.168.1.177/9/100

This will use PWM to illuminate the LED at around 50% brightness (valid PWM values are 0-255).

Now if we connect a switch to pin #9 we can read the digital (on/off) value using this URL:

http://192.168.1.177/9

This returns a tiny chunk of JSON containing the pin requested and its current value:

{"9":"LOW"}

Analog reads are similar; reading the value of Analog pin #1 looks like this:

http://192.168.1.177/a1

...and return the same JSON formatted result as above:

{"a1":"432"}


##Manual Configuration
For testing you'll want some hardware to connect to the Arduino (a green LED is enough to get started).  Connect the LED between pins 9 and ground (GND).

Load up the sketch (RESTduino.pde) and modify the following lines to match your setup:

    byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

This line sets the MAC address of your ethernet board; if your board has one written on it, you should use that instead.

By default RESTduino will use DHCP to configure its IP address, and uses Bonjour/Zeroconf to make itself visible to other clients by name (default name is 'restduino.local').

If you want to use a static address, set the STATICIP flag to 'true' and configure the address by modifying the line below to match your desired address:

    byte ip[] = {192,168,1,177};

Now you should be ready to upload the code to your Arduino.  Once the upload is complete you can open the "Serial Monitor" to get some debug info from the sketch.
