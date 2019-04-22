# Embedded System

This project is a Arduino based code, that uses a HostShield to controll a 3D printer configured by Marlin.

### To controll the printer you have to run the code on the Arduino, and connect the HostShield to the printer.
### Then, all you need to do is open the serial monitor on the arduino IDLE, and write the GCode with the command you want.

# Here are some examples:
```
Move the printer in the X axis: G0 10X
```

```
Move the printer in the Y axis: G0 15Y 
```

```
Move the printer in the X and Y axis: G0 10X 35Y
```
