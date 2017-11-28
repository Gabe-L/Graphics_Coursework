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
	glutSetCursor(GLUT_CURSOR_NONE);

	// Initialise variables
	cam1.update(0.0f);

	//Loading models
	/*bed.load("models/bedpls.obj", "gfx/bed_tex.png");
	desk.load("models/desk.obj", "gfx/desk.png");
	lamp.load("models/lamp.obj", "gfx/lamp.png");
	door.load("models/door.obj", "gfx/door.png");
	doorFrame.load("models/doorFrame.obj", "gfx/doorFrame.png");
	chair.load("models/chair.obj", "gfx/chair.png");*/


	//generating models
	skyLightBorder.generateQuad(10, 20, "gfx/metal_surf.png", { 0,1,0 }, 1.3, 1);
	floor.generateQuad(10, 20, "gfx/metal_surf.png", { 0,1,0 }, 1.3, 1);
	wall.generateQuad(10, 20, "gfx/concrete.png", { 0,0,1 }, 1.3, 0.7);
	wall2.generateQuad(10, 20, "gfx/concrete.png", { -1,0,0 }, 1, 0.7);

	//loading textures
	skybox = SOIL_load_OGL_texture(
		"gfx/skybox.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

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

	if (input->isKeyDown('u')) {
		lightPos.z += (1 * dt);
	}
	if (input->isKeyDown('j')) {
		lightPos.z -= (1 * dt);
	}
	if (input->isKeyDown('h')) {
		lightPos.x -= (1 * dt);
	}
	if (input->isKeyDown('k')) {
		lightPos.x += (1 * dt);
	}
	if (input->isKeyDown('o')) {
		lightPos.y += (1 * dt);
	}
	if (input->isKeyDown('l')) {
		lightPos.y -= (1 * dt);
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
	GLfloat Light_Position[] = { 10, 10, 10, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT0);

	GLfloat Spot_Ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat Spot_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat spot_Direction[] = { 1, 0, 0 };
	GLfloat Spot_Position[] = { position.x, position.y, position.z, 1.0f};
	torchDir = lookAt;
	torchDir.normalise();
	torchDir.operator+=(lookAt);
	
	glPushMatrix();
		glRotatef(cam1.tempYaw, 0, 1, 0);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_Direction);
	glPopMatrix();

	glLightfv(GL_LIGHT1, GL_AMBIENT, Spot_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Spot_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Spot_Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0f);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 50.0);
	glEnable(GL_LIGHT1);

	///***************************
	glPushMatrix();
		glTranslatef(lightPos.x, lightPos.y, lightPos.z);
		glutWireSphere(0.1, 10, 10);
	glPopMatrix();
	// Render geometry/scene here -------------------------------------
	
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, skybox);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(-90, 1, 0, 0);
		glBegin(GL_TRIANGLES);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		// front face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 1.f / 4.f); //top left
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 1.f / 4.f);  //bottom left
		glVertex3f(0.5f, 0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 0.0f); //top right
		glVertex3f(-0.5f, -0.5f, 0.5f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 1.f / 4.f);  //bottom left
		glVertex3f(0.5f, 0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 0.0f); //bottom right
		glVertex3f(0.5f, -0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 0.0f); //top right
		glVertex3f(-0.5f, -0.5f, 0.5f);

		//back face

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 2.f / 4.f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 3.f / 4.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 2.f / 4.f);
		glVertex3f(-0.5f, -0.5f, -0.5f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 3.f / 4.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 3.f / 4.f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 2.f / 4.f);
		glVertex3f(-0.5f, -0.5f, -0.5f);

		// left face

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 1.f / 4.f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 2.f / 4.f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0, 1.f / 4.f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 2.f / 4.f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0, 2.f / 4.f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0, 1.f / 4.f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		// right face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 1.f / 4.f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(3.f / 4.f, 1.f / 4.f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(3.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(3.f / 4.f, 1.f / 4.f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		// bottom face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(3.f / 4.f, 1.f / 4.f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(3.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1, 1.f / 4.f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(3.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1, 2.f / 4.f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1, 1.f / 4.f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		// top face
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 1.f / 4.f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 1.f / 4.f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(2.f / 4.f, 2.f / 4.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 2.f / 4.f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.f / 4.f, 1.f / 4.f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glEnd();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Walls and Floor/Ceiling
	glPushMatrix();
		glTranslatef(0, 1, 1);
		glRotatef(90, 1, 0, 0);
		floor.render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(90, 1, 0, 0);
		skyLightBorder.render();
	glPopMatrix();

	wall.render();

	glPushMatrix();
		glTranslatef(12, 0, 11);
		glRotatef(90, 0, 1, 0);
		wall2.render();
		glPushMatrix();
		glScalef(0.75, 0.75, 0.75);
			glTranslatef(8, 3.7, 1);
			door.render();
			doorFrame.render();
		glPopMatrix();
	glPopMatrix();

	//Room objects
	glPushMatrix();
		glScalef(1.3, 1.3, 1.3);
		glTranslatef(1, -0.1, 3);
		glRotatef(90, 0, 1, 0);
		bed.render();
	glPopMatrix();

	glPushMatrix();
		glScalef(2, 2, 2);
		glTranslatef(4, 0, 4);
		glRotatef(90, 0, 1, 0);
		desk.render();
		glPushMatrix();
			glTranslatef(-1.15, 1.15, 0.6);
			glRotatef(120, 0, 1, 0);
			lamp.render();
		glPopMatrix();
		glScalef(0.2, 0.2, 0.2);
		glTranslatef(0, 1, 0);
		glRotatef(-90, 0, 1, 0);
		chair.render();
	glPopMatrix();
	


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
