#ifndef BIRD_H
#define BIRD

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include "include/object.h"
#include <math.h>
#include <vector>

using namespace std;

class Bird
{

	public:
		
		Bird(double locationX, double locationY, double locationZ, double flyingBounds[]);
		~Bird();

		void updateObjectDisplays(MatrixStack projectionStack);
		void fall(bool drop);
		void steerBird(double angle);

		bool isFalling();

	private:

		void setupBirdSkeleton();
		void wingMovement();
		bool withinBounds();
		
		bool falling;
		bool staticDrop;
		double flyingBounds[3];
		
		double birdCentre[3];
		double birdStartLocationX;
		double birdStartLocationY;
		double birdStartLocationZ;

};

#endif