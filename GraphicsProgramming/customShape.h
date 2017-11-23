#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "Vector3.h"
#include "SOIL.h"

class customShape
{

public:

	void generateDisc(float size, int segments, char* textureFilename);
	void generateQuad(float size, int segments, char* textureFilename);
	//void spawnSphere(float size, int segments);
	//void spawnCylinder(float size, int segments);
	
	void render();
	

protected:
	void loadTexture(char*);

	GLuint texture;

	std::vector<float> vertex, normals, texCoords;

};