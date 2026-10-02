# AI-Assisted Object Sorting Machine (Advanced Version)

This project is a more advanced Arduino-based object sorting machine using a camera and computer vision to identify objects and sort them into different bins.

## Overview
The machine combines:
- Arduino Uno or Mega for actuator control and embedded logic
- Camera + OpenCV for vision processing
- Serial communication between Python and Arduino
- Conveyor belt and sorting gate
- Servo-based bin selection
- Optional IR object detection for controlling conveyor flow

The system is designed for a real prototype and can be upgraded to use YOLO, TensorFlow, or a more complex ML model.

## Hardware Components
- Arduino Mega or Uno
- 12V DC conveyor motor
- L298N motor driver or MOSFET driver
- Camera (USB webcam or Raspberry Pi Camera)
- 2-channel or 3-channel servo motor
- IR proximity sensor
- 3 sorting bins
- 12V power supply
- Breadboard and jumper wires
- Optional LCD display or buzzer

## Typical Sorting Flow
1. Object enters the conveyor area.
2. Camera detects the object and calculates the dominant color / shape.
3. Python script classifies the object into RED, GREEN, BLUE, or OTHER.
4. Python sends the classification to Arduino over serial.
5. Arduino moves the sorting gate to the correct bin.
6. Conveyor continues while the sorting motor or gate actuates.

## Repository Structure
- `arduino/object_sorter_machine/object_sorter_machine.ino` - Arduino firmware
- `python/object_sorter.py` - vision classification and serial communication
- `requirements.txt` - Python dependencies

## Advanced Features
- HSV color detection with contour filtering
- Largest-object scoring logic
- Serial command protocol for sorting and status feedback
- Conveyor automation controlled by IR sensor
- Servo-based gate positioning for multi-bin sorting
- Expandable logic for color, size, or shape classification

## Setup Instructions

### 1. Install Python dependencies
```bash
pip install -r requirements.txt
```

### 2. Upload the Arduino sketch
Open the Arduino IDE and upload:
```text
arduino/object_sorter_machine/object_sorter_machine.ino
```

### 3. Run the vision script
```bash
python python/object_sorter.py
```

### 4. Edit the serial port if needed
In `python/object_sorter.py`:
```python
SERIAL_PORT = "COM3"  # Windows
# SERIAL_PORT = "/dev/ttyUSB0"  # Linux / Raspberry Pi
```

## Communication Protocol
The Python script sends label commands to Arduino in this format:
```text
SORT:RED
SORT:GREEN
SORT:BLUE
STOP
STATUS
```

The Arduino interprets those commands and moves the gate to the corresponding bin.

## Example Sorting Logic
- Red object -> bin A
- Green object -> bin B
- Blue object -> bin C
- Unknown object -> default bin / alert

## Future Upgrades
- Replace color detection with YOLO or TensorFlow
- Add object size and shape detection
- Add stepper motor for precise feed control
- Add an LCD status panel
- Add a metal / RFID sensor for industrial sorting

## License
MIT
