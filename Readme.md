Arduino DCF77 Library
===========================

Version  0.9.9

(c) 2012 Thijs Elenbaas - https://github.com/thijse/Arduino-Libraries/downloads  
(c) 2014 Thomas Mohaupt - https://github.com/ThoMo/DCF77  

*This work is licensed under the under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version. To view a copy of this license, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA*

(This library is forked out of the [arduino-libraries](https://github.com/thijse/Arduino-Libraries) of Thijs Elenbaas.)


The DCF77 library adds the ability to read and decode the atomic time broadcasted by the DCF77 radiostation. It has been designed to work in conjunction with the Arduino Time library and allows a sketch to get the precise CET time and date as a standard C time_t.
The DCF77 Library download. Example sketches have been added to

1.  illustrate and debug the incoming signal
2.  use the library, using the setSyncProvider callback and converting to different time zones. In order to exploit all features in the library, Both the Time and TimeZone library are included.

Additional documentation and full explanations of the samples can be found in these blog posts on DCF Hardware, DCF Signal and ultimately DCF Library.

Functional Overview
-------------------

    DCF77(DCF77Pin, DCFinterrupt, OnRisingFlank);   // Initialize library
    setSplitTime(shortPulseLength, longPulsLength); // set time to distinct between short and long pulses
    time_t getTime();                               // Returns the current time in CET
    time_t getUTCTime();                            // Returns the current time in UTC
    Start();                                        // Start listening to DCF77 signal
    Stop();                                         // Stop listening to DCF77 signal
    isPulse();                                      // Returns current pulse signal level

Using the Library
-----------------

To use the library, first download the DCF77 library [here](https://github.com/thijse/Arduino-Libraries/downloads) and install it in the Arduino Library folder. See these [instructions for installing a third-party library](http://arduino.cc/en/Guide/Libraries) or follow this [step-by-step how-to article on installing Arduino libraries](http://thijs.elenbaas.net/2012/07/installing-an-arduino-library).

The DCFF77 directory contains the DCFF77 library and some example sketches

- DCFSignal

  This is the most basic example: it shows the raw signal coming from the DCF decoder. It will show Pulse-to-Pulse times of approximately 1000 ms and pulse widths of approx 100ms and 200ms.

- DCFPulseLength

  This example illustrates the pulse-to-pulse time and pulse lengths coming from the DCF decoder. While the DCF specification says that pulses should be either 100 or 200 ms, you will probably see other pulse lengths. For optimal distinction between long and short pulses use the output of this sketch as parameter for `setTimeSplit`, e.g. `setTimeSplit(100,200)`.

- DCFBinaryStream

  This example shows the binary stream generated by the pulse train coming from the DCF decoder and the resulting CET time.

- DCFCheck

  This example shows the usage of isPulse() and how the split time can be set (measured with DCFSignal).

- InternalClockSync

  This is the probably the most important example: It shows how to fetch a DCF77 time and synchronize the internal Arduino clock. 

- SyncProvider

  This sketch shows how to fetch a DCF77 time and synchronize the internal clock using the `setSyncProvider` function. Note that the loop code does not require any logic to maintain time sync. The Time library will monitor DC77 and sync the time as necessary. 

- TimeZones

  This example shows how to convert the DCF77 time to a different timezone. It uses the UTC time to ensure that no ambiguities can exist. For timezone conversion it employs the TimeZone library.

Example sketch
--------------

The sketch below implements the most DCF77 basic functionality. It reads the signal from pin 2, and 2-3 minutes it will update the internal clock. More information on this example can be found here: 

http://thijs.elenbaas.net/2012/04/arduino-dcf77-radio-clock-receiver-library/

    #include "DCF77.h"
    #include "Time.h"

    #define DCF_PIN 2                // Connection pin to DCF 77 device
    #define DCF_INTERRUPT 0          // Interrupt number associated with pin

    time_t time;
    DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

    void setup() {
      Serial.begin(9600);
      // DCF.setSplitTime(80, 180);  // set the split time according to pulse length of 80ms resp. 180ms
      DCF.Start();
      Serial.println("Waiting for DCF77 time ... ");
      Serial.println("It will take at least 2 minutes before a first time update.");
    }

    void loop() {
      delay(1000);
      time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
      if (DCFtime!=0)
      {
        Serial.println("Time is updated");
        setTime(DCFtime);
      }
      digitalClockDisplay();
    }

    void digitalClockDisplay(){
      // digital clock display of the time
      Serial.print(hour());
      printDigits(minute());
      printDigits(second());
      Serial.print(" ");
      Serial.print(day());
      Serial.print(" ");
      Serial.print(month());
      Serial.print(" ");
      Serial.print(year());
      Serial.println();
    }

    void printDigits(int digits){
      // utility function for digital clock display: prints preceding colon and leading 0
      Serial.print(":");
      if(digits < 10)
      Serial.print('0');
      Serial.print(digits);
    }

Change log
----------

* v0.9.9
  add `setTimeSplit` function to deal with different pulse width
  
