#ifndef OBJECT_H
#define OBJECT

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include "include/MatrixStack.h"
#include "include/InitShader.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Object
{

	public:
		
		int id;
		string name;

		Object(char* fileName, string objectName);
		~Object();

		bool isCrashed;

		void updateDisplay(MatrixStack projectionStack, bool falling, bool reachedTop, 
		bool staticDrop, bool inBounds, double objectCentre[]);
		void setupData(int objectId);

		MatrixStack* modelViewStack;

		void setTranslation(double x, double y, double z);
		void setTranslationSpeed(double x, double y, double z);
		void setRotation(double x, double y, double z);
		void setRotationSpeed(double x, double y, double z);
		void setSpeed(double speed);
		void resetFall();
		void initiateCrash();
		
		double* getTranslation();
		double* getTranslationSpeed();
		double* getRotation();
		double* getRotationSpeed();
		double getSpeed();

	private:
	
		int numVertices;
		int numIndices;
		int numNormals;

		double crashTravel;
		double objectForwardSpeed;

		double currentTranslation[3];
		double currentTranslationSpeed[3];
		double currentRotation[3];
		double currentRotationSpeed[3];
		
		GLdouble* calcNormal(GLdouble* p1, GLdouble* p2, GLdouble* p3);
		void multiply(GLfloat *res, GLfloat *a, GLfloat *b);
		void objectFall(bool spin);
		void updateWings(MatrixStack projectionStack);
		void readFile(char* fileName);

		GLdouble* vertexPositions;
		GLdouble* vertexNormals;
		GLdouble* vertexColours;
		GLuint* vertexIndices;
		
		GLuint  modelView;  // model-view matrix uniform shader variable location
		GLuint  projection; // projection matrix uniform shader variable location
		
		GLuint numVertexPositionBytes();
		GLuint numVertexNormalBytes();
		GLuint numVertexColourBytes();
		GLuint numVertexIndexBytes();

		//----------------------------------------------------------------------------
		GLuint vao;
		GLuint program;
		GLuint buffers[2];


};

#endif