# WUC
Wireless Ultrasonic Cleaner - WiFi addon
---
Add wifi capability to your ultrasonic cleaner by adding an ESP8266.

Parts required:
- Ultrasonic cleaner (more on that below);
- WeMos D1 mini (or any other board, you'll have to update the pinout);
- that's it.

The ultrasonic cleaner needed is sold under various brand but the main product code is `4820` with a capacity of 2500ml, some names are (and not limited to those): Yokkao, Floureon, Codyson or Harbor Freight. If your device look like that, then you're golden.

![](http://reho.st/self/9742f611124922cdc0eadcb971794be1b0abf8f4.jpg)

The code will allow you to manage your device and extend his capability:
- Turn ON or OFF the heater.
- Preset timing in minutes (1, 2, 3, 4, 5 then 10 minutes steps to 50).
- Preset timing in hours (from 1 to 10 hours).
- Custom timing from 1 minutes to 7 days
- Get the remaining time.
- Report Heater status.
- Emergency stop.
- Autocut off heater when the cycle is done.

![](http://reho.st/self/1f3853ab9b72d726027aae9cff634eacb30264e5.png)
![](http://reho.st/self/e43d74e60541deff48161957b01d9095c0da6b13.png)

---

# How to

Remember to change the `SSID` and `PASSWORD` to yours in the code.

The pinout are as follow:

![](http://reho.st/self/62911da6bf350fb78a8101c98d45f526ca23e486.jpg)

![](http://reho.st/self/bb4eeb4c33e26799c0aeb43c00e98d68c002af7b.jpg)

![](http://reho.st/self/77dc638f197f981a896db77b6be1009d5b0ebc24.jpg)
