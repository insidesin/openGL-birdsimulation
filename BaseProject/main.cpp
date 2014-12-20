#include "include/main.h"

double camRotateValue = 0.0;
double maxSpeed = 0.05;

Bird* bird;
Object* object;
vector<Object*> fences;

MatrixStack projectionStack(5);

//----------------------------------------------------------------------------

// OpenGL initialization
void init() {

	//Enable DEPTH on the buffer.
	glEnable(GL_DEPTH_TEST);

	//Grey Background
	glClearColor(0.2, 0.2, 0.2, 1.0);

	//Our bird
	double bounds[3] = { 2.0, 2.0, 2.0 };
	bird = new Bird(0, 0, 0, bounds);

	//The ground
	object = new Object("plane.obj", "Ground");
	object->setTranslation(0, -2, 0);
	object->setupData(1);

	//Develop fences
	for(int i = 0; i < 4; i++) {
		fences.push_back(new Object("fence.obj", "Fence" + i));
		fences.at(i)->setupData(2);
	}
	//Setup fences around edge.
	fences.at(0)->setRotation(0, 90, 0);
	fences.at(1)->setRotation(0, 90, 0);
	fences.at(0)->setTranslation(-2.5, -2, 0);
	fences.at(1)->setTranslation(2.5, -2, 0);
	fences.at(2)->setTranslation(-2.5, -2, 0);
	fences.at(3)->setTranslation(2.5, -2, 0);
}

//----------------------------------------------------------------------------
void display( void ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//Load the camera projection
	projectionStack.loadIdentity();
	projectionStack.perspective(75, 1, 0.1, 25);
	projectionStack.lookAt(sin(camRotateValue*2) * 5, 1, cos(camRotateValue*2) * 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//Display the bird (and skeleton)
	bird->updateObjectDisplays(projectionStack);

	//Display the plane (ground)
	object->updateDisplay(projectionStack, false, true, false, true, NULL);
	
	//Display all fences
	for(int i = 0; i < fences.size(); i++) {
		fences.at(i)->updateDisplay(projectionStack, false, true, false, true, NULL);
	}

	glutSwapBuffers();

}

//----------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y ) {
	switch( key ) {

	//Quit
	case 033: // Escape Key
	case 'q': case 'Q':
	exit( EXIT_SUCCESS );
	break;
	
	//Drop the bird
	case 'c':
	camRotateValue += 0.1;
	break;
	case 'v':
	camRotateValue -= 0.1;
	break;

	//Drop the bird
	case 'x':
	if(!bird->isFalling())
		bird->fall(false);
	break;
	case 'z':
	if(!bird->isFalling()) {
		bird->fall(true);
	}
	break;

	//Control the bird
	case 'd':
	if(!bird->isFalling())
		bird->steerBird(-5);
	break;
	case 'a':
	if(!bird->isFalling())
		bird->steerBird(5);
	break;

	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
void reshape( int width, int height ) {

	glViewport( 0, 0, width, height );

}

//----------------------------------------------------------------------------
int main( int argc, char **argv ) {

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 512, 512 );
	glutCreateWindow( "Flappy Bird 2.0" );

	glewInit();

	init();

	glutIdleFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );

	glutMainLoop();

	//delete bird;

	return 0;
}
