#include "include/Bird.h"

vector<Object*> objects;

//Constructor for the object, just initialises most variables.
Bird::Bird(double locationX, double locationY, double locationZ, double birdFlyingBounds[]) {
	
	falling = false;
	staticDrop = false;

	birdStartLocationX = locationX;
	birdStartLocationY = locationY;
	birdStartLocationZ = locationZ;

	//Set the flying bounds (note: +1 to -1)
	flyingBounds[0] = birdFlyingBounds[0];
	flyingBounds[1] = birdFlyingBounds[1];
	flyingBounds[2] = birdFlyingBounds[2];

	//Add objects to form a bird object collection.
	objects.push_back(new Object("wing.obj", "WingLeft"));
	objects.push_back(new Object("wing.obj", "WingRight"));
	objects.push_back(new Object("sphere.obj", "Head"));
	objects.push_back(new Object("sphere.obj", "Body"));

	//Setup objects for rendering.
	for(int i = 0; i < objects.size(); i++) {
		objects[i]->setupData(0);
		objects[i]->setSpeed(0.02);
	}

	setupBirdSkeleton();
}

//Destructor.
Bird::~Bird() {
	
	//Remove the allocation of memory.
	for(int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	delete flyingBounds;

}

//Basically sets the objects in the object tree so they 
//are spread out in the shape of a bird's skeleton.
void Bird::setupBirdSkeleton() {
	//Body
	objects[3]->setTranslation( birdStartLocationX, 
								birdStartLocationY, 
								birdStartLocationZ );
	//Left Wing
	objects[0]->setTranslation( birdStartLocationX + objects[0]->getTranslation()[0] + 0.3, 
								birdStartLocationY + objects[0]->getTranslation()[1] + 0.1, 
								birdStartLocationZ + objects[0]->getTranslation()[2] );
	//Right Wing
	objects[1]->setTranslation( birdStartLocationX + objects[1]->getTranslation()[0] - 0.3, 
								birdStartLocationY + objects[1]->getTranslation()[1] + 0.1, 
								birdStartLocationZ + objects[1]->getTranslation()[2] );
	//Head
	objects[2]->setTranslation( birdStartLocationX + objects[2]->getTranslation()[0], 
								birdStartLocationY + objects[2]->getTranslation()[1] + 0.3, 
								birdStartLocationZ + objects[2]->getTranslation()[2] + 0.3);

	//Set wing flap speeds in Z axis rotation
	objects[0]->setRotationSpeed(0, 0, -4);
	objects[1]->setRotationSpeed(0, 0, 4);
}

//Allow steering of the bird only when in air
void Bird::steerBird(double angle) {
	for(int i = 0; i < objects.size(); i++) {
		if(objects[i]->isCrashed || falling)
			break;
		objects[i]->setRotation(objects[i]->getRotation()[0], 
								objects[i]->getRotation()[1] + angle, 
								objects[i]->getRotation()[2] );
	}
}

//Update all openGL visuals of the bird object (structure)
void Bird::updateObjectDisplays(MatrixStack projectionStack) {
	birdCentre[0] = objects[3]->getTranslation()[0];
	birdCentre[1] = objects[3]->getTranslation()[1];
	birdCentre[2] = objects[3]->getTranslation()[2];

	//Check if we've hit the ground
	if(birdCentre[1] <= -flyingBounds[1]) {
		for(int i = 0; i < objects.size(); i++) {
			objects[i]->resetFall();
			if(!staticDrop)
				objects[i]->initiateCrash();
		}
		staticDrop = false;
		falling = false;
	}

	//Go through objects and display.
	for(int i = 0; i < objects.size(); i++) {
		objects[i]->updateDisplay(projectionStack, falling, birdCentre[1] >= flyingBounds[1], 
			staticDrop, withinBounds(), birdCentre);
	}
}

void Bird::fall(bool drop) {
	staticDrop = drop;
	falling = true;
	for(int i = 0; i < objects.size() && !drop; i++) {
		objects[i]->setTranslationSpeed(objects[i]->getTranslationSpeed()[0], 
										0.01, 
										objects[i]->getTranslationSpeed()[2]);
		objects[i]->setSpeed(0.05);
	}
}

bool Bird::isFalling() {
	return falling;
}

bool Bird::withinBounds() {
	//Let's get the location that we think we're going to be in due to steering.
	double nextXLocation = 
		objects[3]->getTranslation()[0] + 
		sin(M_PI * objects[3]->getRotation()[1] / 180) * objects[3]->getSpeed();
	double nextZLocation = 
		objects[3]->getTranslation()[2] + 
		cos(M_PI * objects[3]->getRotation()[1] / 180) * objects[3]->getSpeed();

	//Is it within our bounds?
	if(flyingBounds[0] < nextXLocation || -flyingBounds[0] > nextXLocation ||
	   flyingBounds[2] < nextZLocation || -flyingBounds[2] > nextZLocation)
		return false;

	return true;
}