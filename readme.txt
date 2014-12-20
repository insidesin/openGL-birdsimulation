

INB381 Assignment 2:












Student Name:		Jackson Powell
Student ID:			08600571
Date:				27/10/2014


Statement of Completeness:

Student Name:		Jackson Powell
Student ID:		08600571
Date:			27/10/2014
I completed all tasks in the assignment; this includes all tasks along with their entire features to the best of my ability and knowledge. I did not come across any problems that I couldn’t work around.
Description of Program code:
To run the application, simply open it in visual studio 2010 (what I used) and run I didn’t include the .exe debug file in case there were security issues that arose.
New Feature:
There was no single significant feature being introduced in this assignment, but many smaller improvements. I managed to include some significantly more complex objects in the scene (the fences surrounding the simulation-zone). I also attempted to introduce texture mapping but didn’t feel I had time I decided against it. The bird does not require a movement key to be pressed as I felt this was unintuitive since the bird realistically shouldn’t stop mid-air and so the bird instead works on a constant speed (unless input/movement becomes restricted through simulation events).
==> My program can be run in Visual Studio 2010, C++ environment (or just using code hopefully).
==> Shouldn’t require any libraries besides freeGLUT and glew, and if it does they are included already.
Shaders:
The shaders are the same used in our tutorials and learnt from lecturer’s code. Although those shaders were not making use of read objects (.obj files) and calculating the normal of said objects, and instead were only compliant with Spheres.

Controls:
* ‘q’ keyboard button exits the program. 
* A and D keys control the bird flight by giving the bird a new angle to aim towards (using constant motion, it will then fly in that direction). 
* Z key drops the bird in motion but with no real fall spiral. (straight down)
* X key drops the bird in a spiral formation, if it hits a wall it will not move forward but it will continue to rotate until it reaches a direction away from the wall. Upon reaching the floor the bird will crash for a  few seconds and then return to flight afterwards.
* C and v (left and right) are used to turn the environment in a sense that you are looking at it from a different angle, these keys move the camera around horizontally in a circle, always looking at the centre of the environment.


Screenshots (progress pictures too):

Finished:



Different projections/camera angles:


