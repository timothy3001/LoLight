# Haloght
Building a smart RGB(W) light was never as easy! (Ok, maybe some day in the future when this is actually finished...)

## The idea
Last year I purchased a ceiling light which is similar to this one: https://goo.gl/DBBDUp 
I love the design and distribution of light, but...

At the beginning, it was way too bright so I am using a dimmer now, which is sometimes still too bright though for my tastes. And even though the color of the light is ok, I would still want to be able to change it and make it more comfortable, use a wider variety of colors.

## SK6812 RGBW
I recently got myself some SK6812 RGBW in the "warm white" version and I like them a lot. Compared to WS2812 or other RGB LEDs, these give a much nicer white light.

## The project
The obvious conclusion for a maker is to pimp that lamp using some SK6812 RGBW LEDs and a microcontroller! I already made a similar project for an ESP8266 but I had lots of trouble with having a "nice and goodlooking" page to control the device. Concluded: I had lots of crashes while opening the control page, slow load times etc... 

This is why I am going to use an ESP32 for this and hopefully I will not run into similar issues.

After all this project is not supposed to be a quick and dirty solution, I want it to be a more sophisticated solution that others can benefit from as well.

## Goals
1. First and foremost it should be easy to build. In particular this means that no one should need to modify the actual code to get it running. There should be no WiFi configuration in code, no LED setup in code (e.g. amount and kind of LEDs used) and so on. In short: Flash the ESP32, solder the data pin of the LEDs to the ESP32 and you are good to go. The remaining configuration will be done via web interface.
2. A smart device that can do old school tricks: What is the usual way to control a "smart" device? Correct, by using your smartphone. That is pretty cool, but also most of the time just annoying. I want to be able to turn the light on and off via light switch without rewiring my appartment. ~~Thus, the device should remember its last state when it gets turned off and on again.~~ There will be a default color that is set whenever you turn the lights off and back on again. When you use the On / Off button on the webpage, the default color will also be used.
3. Cool AND useful LED effects only: We all love cool LED effects but having a ceiling lamp that acts as strobe light is most of the time not that useful.
4. Additional "smart" features: Even though this might be somehow contrary to 3 I would like to have some additional features in the lamp. For example a subtle LED effect which blends in every full hour to show the time.

## ESP8266 FastLED Webserver
There is already a similar project existing for ESP8266 which you can find here: https://github.com/jasoncoon/esp8266-fastled-webserver 
You should check it out. It just doesn't entirely fulfill my needs though.

## PlatformIO
I do not like the Arduino IDE very much. PlatformIO is great and offers many features. You need to install Microsoft Visual Studio Code and PlatformIO as extension. After that, you should be able to build and run the project.

## Name
Since my lamp kinda looks like a halo I thought the composition between "Halo" and "Light" would make a good name. And I could not find something else which is named "Haloght" online.
