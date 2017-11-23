#include "Scene.h"

Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
		
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Other OpenGL / render setting should be applied here.
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	// Initialise variables
	cam1.update(0.0f);

	//Loading models
	bed.load("models/bedpls.obj", "gfx/bed_tex.png");
	lamp.load("models/lamp.obj", "gfx/lamp.png");
	desk.load("models/desk.obj", "gfx/desk.png");
	door.load("models/door.obj", "gfx/door.png");
	doorFrame.load("models/doorFrame.obj", "gfx/doorFrame.png");
	chair.load("models/chair.obj", "gfx/chair.png");

	testDisc.generateQuad(1, 20, "gfx/concrete.png");

}

void Scene::update(float dt)
{
	// Handle user input
	
	// update scene related variables.

	//Camera Input
	if (input->isKeyDown('w')) {
		cam1.moveForward(dt);
		camUpdate = true;
	}

	if (input->isKeyDown('s')) {
		cam1.moveForward(-dt);
		camUpdate = true;
	}

	if (input->isKeyDown('r')) {
		cam1.moveUp(dt);
		camUpdate = true;
	}

	if (input->isKeyDown('f')) {
		cam1.moveUp(-dt);
		camUpdate = true;
	}

	if (input->isKeyDown('d')) {
		cam1.moveRight(dt);
		camUpdate = true;
	}

	if (input->isKeyDown('a')) {
		cam1.moveRight(-dt);
		camUpdate = true;
	}

	mouseDiffX = input->getMouseX() - (width / 2);
	mouseDiffY = input->getMouseY() - (height / 2);

	if (mouseDiffX != 0 || mouseDiffY != 0) {
		cam1.movePYR(-mouseDiffY, mouseDiffX, 0, dt);

		camUpdate = true;

		glutWarpPointer(width / 2, height / 2);
	}

	if (input->isKeyDown('q')) {
		cam1.movePYR(0, 0, 1, dt);
		camUpdate = true;
	}

	if (input->isKeyDown('e')) {
		cam1.movePYR(0, 0, -1, dt);
		camUpdate = true;
	}

	if (camUpdate) {
		cam1.update(dt);
		camUpdate = false;
	}

	position = cam1.getPosition();
	lookAt = cam1.getLookAt();
	up = cam1.getUp();

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
	
	///***************************
	///TEMP LIGHTING SETTINGS
	GLfloat Light_Ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat Light_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light_Position[] = { 3.0f, 3.0f, 10.0f, 0.0f };


	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT0);

	///***************************

	// Render geometry/scene here -------------------------------------
	
	//bed.render();
	//lamp.render();
	//desk.render();
	//door.render();
	//doorFrame.render();
	//chair.render();

	testDisc.render();

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	
}

// Calculates FPS
void Scene::calculateFPS()
{

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
