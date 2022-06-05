# Robot-Arm-Remote-Control-UdeG-Space
> This is the Mars Rover Robot Arm Control testing code created by UdeG Space members (2022)

## matlab branch
There are two ways to control the motors with the joystick:
1. The `ArduinoRemoteControl.m` file **detect the joystick inputs and controls the Arduino outputs**.
2. The `Transmitter_Joystick.m` file **detect the joystick inputs and send the serial data** to the `Receiver_Motors.ino` file that **controls the Arduino outputs**.
