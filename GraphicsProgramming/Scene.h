// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include "Vector3.h"
#include "Camera.h"
#include "Model.h"
#include "customShape.h"


class Scene{

public:
	Scene(Input *in);
	// Main render function
	void render();
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

	//Camera Variables
	Vector3 position, lookAt, up;
	Camera cam1;
	bool camUpdate;
	float mouseDiffX, mouseDiffY;

	//Loaded models
	Model bed;
	Model lamp;
	Model desk;
	Model door;
	Model doorFrame;
	Model chair;

	//Custom models
	customShape skyLightBorder;
	customShape floor;
	customShape wall, wall2;

	//textures
	GLuint skybox;

	///TEMP VARs
	Vector3 lightPos = { 0.0f,0.0f,0.0f };
	Vector3 torchDir;

protected:
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	// draw primitive functions
	

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];

};

#endif