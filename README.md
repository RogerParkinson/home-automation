# home-automation
Home automation project

Starting small. We have an simple 433Hz transmitter controlled by an attiny85 (call it tx) with a sensor (actually an on/off switch) that transmits its status to a 433Hz receiver also controlled by an attiny85 (call it rx).

rx is connected to a beaglebone black (BBB) via I2C and it simply relays messages from the tx to the BBB.

The message format is 4 bytes: length, target, sender, function, where target is normally 1, ie the home station, the rx/BBB combination and the sender is the id of the tx. This way we can have multiple tx's sending to one (or more) home stations. The message may contain a string in the following bytes but the initial byte always contains the overall length (but not including the length byte).

Further out we can have the home stations sending to peripheral boxes as well.

The functions, for now are just two: event (my switch was thrown) or low battery. The tx's all run of 3xAA batteries and this gives us a central way to monitor them.


