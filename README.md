# Pressure_Regulator
Code and circuit diagram for a pressure regulator using ITV0031,MPX5500DP &amp; MCP41010 sensor and IC


# Circuit_Diagram
Circuit diagram is shown in the png file

# Code
The following code is written for Arduino UNO. 11 is the MOSI & 13 is the CLK pin being used in this code(51 & 52 respectively for Arduino MEGA). Pins 9 and 10 are used to select the IC which should be 'ON' when the input value is given in the Serial monitor.A0 & A1 are used to measure the pressure difference using MPX5500DP sensor.

# START THE CODE KEEPING THE INPUT VALVE CLOSED AS THE CODE CALCULATES INITIAL PRESSURE TO COMPENSATE FOR THE ERROR

# EXECUTING
Once executed type in serial monitor the pressure you need as following:
For instance:
- 100_50 would ask sensor1(A0) to set pressure to 100 while the other(A1) to 50.
- would directly give zero to both the sensors (no need to enter _ in between).
- would set first sensor to 50 while keeping the other sensor to 0 (no need of _ again if you need zero)
- _50 would set the second sensor to 50 while keeping 1 at zero.
