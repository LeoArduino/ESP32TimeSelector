# ESP32TimeSelector

This small project makes use of the work of Random Nerd Tutorials.
https://randomnerdtutorials.com

In the RNT ESP32 series Web Servers tutorials Grid cards are introduced with buttons and meters.
This project adds a Grid card with a Time Selector.
The Grid card shows a switch to select OFF -- TIMER -- ON  Which drives an output pin at the ESP32.
When the TIMER is selected the set startTime and StopTime are used to drive the output.
Also a day of the week selector is included to set the date(s) at which the set time window will be active.
It uses Websockets, to enable more connected browsers at the same time, which are synchronized with each other.

![image](https://user-images.githubusercontent.com/118529118/215122176-b586de8a-5fa2-4bae-854e-edac648d7d3d.png)
