# Robot-Arm-Remote-Control-UdeG-Space
> This is the Mars Rover Robot Arm Control testing code created by UdeG Space members (2022)


There are three branches in this repository:
1. The `main` branch **contains the ROS node implementation**.
2. The `serialtest` branch **contains testing code for the arduino serial comunication and outputs response**.
3. The `matlab` branch **contains two ways to test the joystick with the arduino**.

## Main branch
The `arm_node.ino` file is the subscriber node that recieves the serial messages from the **joy topic** and controls the arm motors.

The `arm_launch.launch` file is the ROS launch with all the necesary nodes (**joy_node** and **rosserial**).
