# Magnetic Accelerator
Code for a little coil based magnetic launcher I designed using the MSP430 MCU! Features include a Hall-effect sensor for automatic magnet detection, allowing for detection of the magnet approaches the coil and a 2-stage acceleration sequence which reverses the polarity of the coil and allows for a higher final velocity. Uses an RC car motor driver to push 1 A of current through a coil of unknown strength, as I just found the coil lying around and have yet to fully characterize its strength. Despite the single-coil design (I could only find the one), it launches several coin sized neodynium magnets with enough speed to cause them to occasionally chip on contact.

The source code can be found within `main.c`.

The contraption can be seen here:
![alt text](https://github.com/jai-parhar/Magnetic-Accelerator/blob/main/mag_accelerator.jpg)
