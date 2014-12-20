#include "include/object.h"

//Constructor for the object, just initialises most variables.
Object::Object(char* fileName, string objectName) {
	name = objectName;

	numVertices = 0;
	numIndices  = 0;
	numNormals  = 0;

	isCrashed = false;
	crashTravel = 150;
	
	//Setup translation and roation
	for(int i = 0; i < 3; i++) {
		currentTranslation[i] = 0.0;
		currentTranslationSpeed[i] = 0.0;
		currentRotation[i] = 0;
		currentRotationSpeed[i] = 0;
	}
	objectForwardSpeed = 0.0;

	modelViewStack = new MatrixStack(5);
	
	readFile(fileName);
}

//Destructor.
Object::~Object() {
	
	delete modelViewStack;
	delete vertexPositions;
	delete vertexNormals;
	delete vertexColours;
	delete vertexIndices;

}

void Object::setupData(int objectId) {

	id = objectId;

	// Load shaders and use the resulting shader program
	program = InitShader( "shaders/vertexShader.glsl", "shaders/pixelShader.glsl" );
	glUseProgram( program );

	modelView = glGetUniformLocation( program, "ModelView" );
	projection = glGetUniformLocation( program, "Projection" );

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Create and initialize two buffer objects
	glGenBuffers(2, buffers);

	//one buffer for the vertices and colours
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0]);
	glBufferData( GL_ARRAY_BUFFER, numVertexPositionBytes() + numVertexNormalBytes(), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, numVertexPositionBytes(), vertexPositions );
	glBufferSubData( GL_ARRAY_BUFFER, numVertexPositionBytes(), numVertexNormalBytes(), vertexNormals);

	//one buffer for the indices
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, numVertexIndexBytes(), vertexIndices, GL_STATIC_DRAW );

	//Allow the program to communicate with the shader programs by passing and receiving refrences.
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_DOUBLE, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_DOUBLE, GL_FALSE, 0, BUFFER_OFFSET(numVertexPositionBytes()) );

    // Initialize shader lighting parameters
    GLfloat light_position[] = { 1.0, 2.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { 0.2, 0.2, 0.6, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0};

	GLfloat material_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat material_diffuse[] = { 0.0, 0.0, 0.5, 1.0 };
	GLfloat material_specular[] = { 0.5, 0.5, 1.0, 1.0 };
    float material_shininess = 20.0;

    GLfloat ambient_product[4], diffuse_product[4], specular_product[4];

    multiply(ambient_product, light_ambient, material_ambient);
    multiply(diffuse_product, light_diffuse, material_diffuse);
    multiply(specular_product , light_specular, material_specular);

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position );
    glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

//Simple method for multiplication of material parameters
void Object::multiply(GLfloat *res, GLfloat *a, GLfloat *b){
  for (unsigned char i=0 ; i<4; i++) res[i] = a[i]*b[i];
}

void Object::updateDisplay(MatrixStack projectionStack, bool falling, bool reachedTop, 
	bool staticDrop, bool inBounds, double objectCentre[]) {

	//Load modelView Identity
	modelViewStack->loadIdentity();

	if(!reachedTop && !falling && !isCrashed)
		currentTranslation[1] += currentTranslationSpeed[1];
	if(isCrashed) {
		crashTravel++;
		if(objectForwardSpeed > 0.001)
			objectForwardSpeed -= 0.001;
		if(crashTravel > 150) {
			isCrashed = false;
			objectForwardSpeed = 0.02;
		}
	}
		
	//Are we dropping [straight] down? Initiate the drop.
	if(staticDrop)
		objectFall(false);
	if(falling) {
		if(inBounds && !staticDrop)
			objectFall(true);
		else if(!inBounds) 
			objectFall(false);
	}
	
	//OBJECT MOVEMENT AND ROTATION
	if(objectCentre != NULL)
		modelViewStack->translated(objectCentre[0], objectCentre[1], objectCentre[2]);

	//Rotate the object based on it's rotation speed (and current rotation) AROUND AXI
	if(!isCrashed) {
		currentRotation[0] += currentRotationSpeed[0];
		currentRotation[1] += currentRotationSpeed[1];
		currentRotation[2] += currentRotationSpeed[2];
	}

	//Rotate on all axi if we have changes from our rotation speed.
	modelViewStack->rotated(currentRotation[0], 1, 0, 0);
	modelViewStack->rotated(currentRotation[1], 0, 1, 0);
	modelViewStack->rotated(currentRotation[2], 0, 0, 1);

	if(objectCentre != NULL)
		modelViewStack->translated(-objectCentre[0], -objectCentre[1], -objectCentre[2]);

	if(inBounds && !falling) {
		//Steering motors
		if(name != "Ground") {
			currentTranslation[0] += sin(M_PI * currentRotation[1] / 180) * objectForwardSpeed;
			currentTranslation[2] += cos(M_PI * currentRotation[1] / 180) * objectForwardSpeed;
		}
	}

	//Translate the object in space.
	modelViewStack->translated(currentTranslation[0], currentTranslation[1], currentTranslation[2]);

		
	//Wing movements
	if(name == "WingLeft" || name == "WingRight")
		updateWings(projectionStack);
	
	glUniformMatrix4fv(modelView, 1, GL_FALSE, modelViewStack->getMatrixf());
	glUniformMatrix4fv(projection, 1, GL_FALSE, projectionStack.getMatrixf());
	
	//Bind vao
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	//Indexing into vertices we need to use glDrawElements
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

void Object::objectFall(bool spin) {
	//Accelerated drop based on rotation rate.
	if(currentRotationSpeed[1] < 15)
		currentRotationSpeed[1] += 0.04;
	currentTranslation[1] -= currentRotationSpeed[1] * 0.005;
	//If we're spinning, we can force movement forward, as we spin
	//the spinning will increase in angle and hence decrease the spin radius.
	if(spin) {
		currentTranslation[0] += sin(M_PI * currentRotation[1] / 180) * objectForwardSpeed;
		currentTranslation[2] += cos(M_PI * currentRotation[1] / 180) * objectForwardSpeed;
	}
}

void Object::resetFall() {
	currentTranslation[1] += 0.1;
	currentTranslationSpeed[1] = 0.01;
	objectForwardSpeed = 0.02;
	currentRotationSpeed[1] = 0.0;
}

void Object::initiateCrash() {
	objectForwardSpeed = 0.05;
	crashTravel = 0;
	isCrashed = true;
}

void Object::updateWings(MatrixStack projectionStack) {
	//WING FLAPPING
	//Translate the object in space.
	modelViewStack->translated(name == "WingRight" ? 0.3 : -0.3, 0, 0);

	currentRotation[2] += currentRotationSpeed[2];
	//Rotate the object based on it's rotation speed (and current rotation) AROUND Z AXIS
	modelViewStack->rotated(currentRotation[2], 0, 0, 1);
		
	modelViewStack->translated(name == "WingRight" ? -0.3 : 0.3, 0, 0);
}

//Calculate the normal of object triangles using 3 vertices.
GLdouble* Object::calcNormal(GLdouble* p1, GLdouble* p2, GLdouble* p3) {
	GLdouble V1[3] = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
	GLdouble V2[3] = { p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2] };
	GLdouble* normal = new double[3];
    normal[0] =  (V1[1] * V2[2]) - (V1[2] * V2[1]);
    normal[1] =  (V1[2] * V2[0]) - (V1[0] * V2[2]);
    normal[2] =  (V1[0] * V2[1]) - (V1[1] * V2[0]);

	GLdouble length = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);

	normal[0] /= length;
	normal[1] /= length;
	normal[2] /= length;

	//cout << normal[0] << " " << normal[1] << " " << normal[2] << "\n";
    // Dont forget to normalize if needed
    return normal;
}

//Reads the file in and grabs vertex information, face information etc.
void Object::readFile(char* fileName) {
	ifstream in(fileName);

	//Use vectors for now so we know how to assign global arrays space in memory later.
	vector<GLdouble> tempPositions;
	vector<GLdouble> tempNormals;
	vector<GLuint> tempIndices;

	//Keep track of what line we're on.
	int currentLine = 0;

	string lineStr;
    while(getline(in, lineStr)) {

        istringstream lineSS( lineStr );
        string lineType;
        lineSS >> lineType;

        //Fetch a line with vertices
        if(lineType == "v") {
            GLdouble x = 0.0, y = 0.0, z = 0.0, w = 1.0;
            lineSS >> x >> y >> z;

			//Debug text.
            //cout << currentLine + 1 << " " << x << " " << y << " " << z << " " << w << endl;

			tempPositions.push_back(x);
			tempPositions.push_back(y);
			tempPositions.push_back(z);
			tempPositions.push_back(w);

			numVertices += 1;
        }

		//Fetch a line with vertex indices
        if(lineType == "f") {
            GLuint x = 0, y = 0, z = 0;
            lineSS >> x >> y >> z;

			//Debug text.
            //cout << currentLine + 1 << " " << x << " " << y << " " << z << endl;
			
			tempIndices.push_back(x);
			tempIndices.push_back(y);
			tempIndices.push_back(z);

			numIndices += 3;
        }

		currentLine++;
    }

	//Setup normal array for insertion
	vertexNormals = new GLdouble[numIndices*4];
	for(int i = 0; i < numIndices*4; i++)
		vertexNormals[i] = 0.0;
	
	//Calculate normals
	for(int i = 0; i < numIndices; i += 3) {
		int x = tempIndices[i+0];
		int y = tempIndices[i+1];
		int z = tempIndices[i+2];

		GLdouble p1[3] = {tempPositions[(x-1)*4], tempPositions[(x-1)*4+1], tempPositions[(x-1)*4+2]};
		GLdouble p2[3] = {tempPositions[(y-1)*4], tempPositions[(y-1)*4+1], tempPositions[(y-1)*4+2]};
		GLdouble p3[3] = {tempPositions[(z-1)*4], tempPositions[(z-1)*4+1], tempPositions[(z-1)*4+2]};
		double* pointNormals = new double[3];
		pointNormals = calcNormal(p1, p2, p3);

		vertexNormals[(x-1)*4+0] += pointNormals[0];
        vertexNormals[(x-1)*4+1] += pointNormals[1];
        vertexNormals[(x-1)*4+2] += pointNormals[2];
 
        vertexNormals[(y-1)*4+0] += pointNormals[0];
        vertexNormals[(y-1)*4+1] += pointNormals[1];
        vertexNormals[(y-1)*4+2] += pointNormals[2];
 
        vertexNormals[(z-1)*4+0] += pointNormals[0];
        vertexNormals[(z-1)*4+1] += pointNormals[1];
        vertexNormals[(z-1)*4+2] += pointNormals[2];
	}

	//cout << numVertices / 3 << "  ";
	//cout << numIndices << "  ";

	//Store contents of vectors in global array.
	vertexPositions = new GLdouble[numVertices*4];
	for(int i = 0; i < numVertices*4; i++) {
		vertexPositions[i] = tempPositions.at(i);
		//cout << tempPositions.at(i) << " ";
	}
	vertexIndices = new GLuint[numIndices];
	for(int i = 0; i < numIndices; i++) {
		vertexIndices[i] = tempIndices.at(i) - 1;
		//cout << tempIndices.at(i) << " ";
	}
	vertexColours = new GLdouble[numVertices*4];
	int modifier = 0;
	for(int i = 0; i < numVertices*4; i++) {
		modifier += 1;
		vertexColours[i] = (modifier % 10) * 0.1;
		//cout << vertexColours[i] << " ";
	}
}

//ACCESSORS AND SETTERS ARE BELOW
void Object::setTranslation(double x, double y, double z) { 
	currentTranslation[0] = x;	
	currentTranslation[1] = y;	
	currentTranslation[2] = z;	
}
void Object::setTranslationSpeed(double x, double y, double z) { 
	currentTranslationSpeed[0] = x;	
	currentTranslationSpeed[1] = y;	
	currentTranslationSpeed[2] = z;	
}

void Object::setRotation(double x, double y, double z) { 
	currentRotation[0] = x;	
	currentRotation[1] = y;	
	currentRotation[2] = z;	
}
void Object::setRotationSpeed(double x, double y, double z) { 
	currentRotationSpeed[0] = x;	
	currentRotationSpeed[1] = y;	
	currentRotationSpeed[2] = z;	
}

void Object::setSpeed(double speed) { 
	objectForwardSpeed = speed;
}

double* Object::getTranslation()	 { return currentTranslation;	}
double* Object::getRotation()		 { return currentRotation;		}
double Object::getSpeed()			 { return objectForwardSpeed;	}

double* Object::getTranslationSpeed()	{ return currentTranslationSpeed;	}
double* Object::getRotationSpeed()		{ return currentRotationSpeed;		}

GLuint Object::numVertexPositionBytes() { return numVertices*4*sizeof(GLdouble);	}
GLuint Object::numVertexColourBytes()	{ return numVertices*4*sizeof(GLdouble);	}
GLuint Object::numVertexNormalBytes()	{ return numIndices*4*sizeof(GLdouble);	}
GLuint Object::numVertexIndexBytes()	{ return numIndices*sizeof(GLuint);			}