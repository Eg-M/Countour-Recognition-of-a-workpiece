# Workpiece Positioning System for Coordinate Machines

## Project Description
This program assists operators of coordinate machines by determining the optimal positioning (coordinates and angle of rotation) of a workpiece. It streamlines the setup process for tools like lasers or milling heads on coordinate machines.

## Key Features
- Captures and processes live video feed from a standard camera.
- Implements Hough Transform for line detection.
- Calculates workpiece's location and rotation angle.
- Provides real-time visual feedback via GUI.
- Saves the captured image and workpiece data for record-keeping.

## Technologies Used
- C++ with OpenCV library for image processing.
- Standard Template Library (STL) for handling data structures.

## How to Run
- Ensure proper camera connection and OpenCV setup.
- Compile and run the program in an environment supporting C++ and OpenCV.
- Interact with the program through the graphical interface for live updates.

## Output
- The program outputs the coordinates and angle of the workpiece.
- Saves the workpiece data to `data.txt` and the processed image to `capture.jpg`.

## Potential Enhancements
- Integration with automated coordinate machine systems for direct control.
- Implement more advanced image processing algorithms for higher precision.
