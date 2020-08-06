# ESP32 BLE Tire Pressure Management System

Arduino code (ESP32) to read the tire pressures, temperatures, battery levels and alarms from the BLE ZEEPIN TPMS Sensors, TP630, etc.

## The TPMS BLE "manufacturer data" format

The devices cannot be connected or paired to and the devices do not receive any incoming BLE data. All data is broadcast as part of the "Manufacturer data" portion of the BLE advertisement.
Manufacturer data looks like this:

```
000180EACA108A78E36D0000E60A00005B00
```

And now let's analyze in depth the received data:<br><br>
_bytes 0 and 1_<br>
`0001`		Manufacturer<br><br>
_byte 2_<br>
`80`		Sensor Number (`80`:1, `81`:2, `82`:3, `83`:4, ...)<br><br>
_bytes 3 and 4_<br>
`EACA`		Address Prefix<br><br>
_bytes 5, 6 and 7_<br>
`108A78`	Sensor Address<br><br>
_bytes 8, 9, 10 and 11_<br>
`E36D0000`	Tire pressure (in kPA)<br><br>
_bytes 12, 13, 14 and 15_<br>
`E60A0000`	Tire Temperature (in Celsius)<br><br>
_byte 16_<br>
`5B`		Battery Percentage<br><br>
_byte 17_<br>
`00`		Alarm Flag (`00`: Ok, `01`: No pressure)<br><br>

## Extract Tire pressure and temperature

Bytes 8,9,10 and 11 are a representation of the air pressure in kPA; Bytes 12,13,14 and 15 represent the temperature in Celsius.
To get the values we need to do a little-endian conversion.

```
long result = byte0|byte1<<8|byte2<<16|byte3<<24;
```

The pressure in kPA is obtained by dividing by 1000 the value obtained from the conversion:<br>
```
kPA=result/1000.0
```
<br>
The pressure in bar is obtained by dividing by 100000 the value obtained from the conversion:<br>
```
bar=result/100000.0
```
<br>
The temperature in Celsius is obtained by dividing by 100 the value obtained from the conversion:<br>
```
temp=result/100.0
```
<br>
