# ESP32 BLE Tire Pressure Management System

Arduino code (ESP32) to read the tire pressures, temperatures, battery levels and alarms from the BLE ZEEPIN TPMS Sensors, TP630, etc.

## The TPMS BLE "manufacturer data" format

The devices cannot be connected or paired to and the devices do not receive any incoming BLE data. All data is broadcast as part of the "Manufacturer data" portion of the BLE advertisement.
Manufacturer data looks like this:

```
000180EACA108A78E36D0000E60A00005B00
```

And now let's analyze in depth the received data:

`0001`		Manufacturer<br>
`80`		Sensor Number (80:1, 81:2, 82:3, 83:4, ...)<br>
`EACA`		Address Prefix<br>
`108A78`	Sensor Address<br>
`E36D0000`	Tire pressure (in kPA)<br>
`E60A0000`	Tire Temperature (in Celsius)<br>
`5B`		Battery Percentage<br>
`00`		Alarm Flag (00: Ok, 01: No pressure)<br>
