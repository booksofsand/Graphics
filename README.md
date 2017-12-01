This repository contains the foundations of a program that displays plain text documents onto a physical sandbox with varying text at the varying sand depths.

The sandboxwindow class reads in plain text documents and distributes the characters into a 3D grid array. Based on the current depth map, the sandboxwindow determines which character to display at each location in the 2D display. e.g. If the depth level d at row x, col y decrements by one, the sandboxwindow will update its display to show the character intended for row x, col y, depth d-1.

The kinecthandler class is intended to connect the Kinect camera to the sandboxwindow. It should initialize the Kinect with SARndbox Kinect repository code, then receive the Kinect frame buffers containing the depth map data, convert the values to depth levels for the sandboxwindow, and pass updated depth levels to the sandboxwindow to update the text display. This class needs more work to work properly with the Kinect - retrieving a usable depth map is not working yet.

There are a few sample plain text files for input in the texts subdirectory. A sandboxwindow may be initialized with more than one text file as input: with multiple text files, it will only populate a single layer per text file; with a single text file as input, it will continue to populate layers with characters while there remain characters in the text file.
