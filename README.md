# Thesis_Project

This project contains files regarding my Final year project.

1. Final_code_transmitter_on_VR :
Arduino sketch written for getting and transmitting Accelerometer(ADXL335) and Magnetometer(HMC5833I) wirelessly
over xbee(series 1) network.
These sensors are mounted on smart phone VR Goggles and track head movements of the user in 2D.
The sensor output values are in degrees(10-170) read to give it to pan-tilt mechanism of servo for movements.

2. Final_code_Receiver_on_UGV
Arduino sketch to accept sensor values wirelessly over xbee network. 
The sketch is burnt on Arduino mega mounted on a UGV which controls the pan-tilt mechanism as per the sensor values.
