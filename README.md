RESTduino
=========

(note: RESTduino is very beta, expect surprises for awhile yet)

RESTduino is a simple sketch to provide a REST-like interface to the Arduino via the Ethernet Shield.  The idea is to allow developers familiar with interacting with REST services a way to control physical devices using the Arduino without having to write any Arduino code.

Of course some flexibility is traded for this convinience, only basic operations are currently supported:

* Digital pin I/O
* Analog pin input

Later versions of the sketch may provide additional functionality (PWM, servo control, etc.) however if you need more than just basic pin control you're probably better off learning the basics of programming the Arduino and offloading some of your processing to the board itself.

Examples
--------

To read the value of digital pin #9:

http://192.168.1.177/9

This returns a tiny chunk of JSON containing the pin requested and its current value:

{"9":"0"}

To set the value of digital pin #9:

http://192.168.1.177/9/HIGH

This will set the pin to the HIGH state and will again test the state of the pin and return the state as JSON (useful for troubleshooting):

{"9":"1"}

(note that the return values are integers; I'm still torn on what to "standardize" on in regard to digital values)

Analog reads are simular; reading the value of Analog pin #1 looks like this:

http://192.168.1.177/a1

...and return the same JSON formatted result as above:

{"a1":"432"}

(I'm not sure if the analog reads are working correctly yet, my test equiptment is incomplete at the moment)
