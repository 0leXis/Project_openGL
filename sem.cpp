//----------------------------------------------------------------------------------------
/**
 * \file    skeleton.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Jaroslav Sloup, Petr Felkel
 * \date    2022/03/03
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

 /**
  * \brief	\mainpage Documentation of the skeleton for the Computer graphics course on FEE and FIT CTU CZ
  *
  * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
  * It serves as a starter boiler-plate code for a semester project - a simple interactive application.
  * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
  *
  *
  * Comment your code using the [doxygen](https://www.doxygen.nl/index.html) documenting system style.
  * Create "doxygen" directory, make it current by "cd doxygen", prepare a configuration file with "doxygen -g" and edit the details.
  *
  * Start by renaming of this file from skeleton.cpp to <your_name>.cpp and the project to <your_name>.vcxproj
  *
  * In <your_name>.vcxproj:
  *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
  *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
  *
  * Start coding and documenting. Generate the documentation by the command "doxygen" in the "doxygen" directory.
  *
  */


  // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer


#include <iostream>
#include <unordered_map>
#include "pgr.h"
#include "object.h"
#include "triangle.h"
#include "singlemesh.h"
#include "camera.h"


constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

ObjectList objects;
Camera camera;
std::unordered_map<char, bool> keyPressedState;
std::unordered_map<int, bool> keyPressedSpecialState;
int previousMouseX = 0;
int previousMouseY = 0;

// shared shader programs
ShaderProgram commonShaderProgram;


// -----------------------  OpenGL stuff ---------------------------------

/**
 * \brief Load and compile shader programs. Get attribute locations.
 */
void loadShaderPrograms()
{
	std::string vertexShaderSrc =
		"#version 140\n"
		"in vec3 position;\n"
		"uniform mat4 PVM;\n"
		"void main() {\n"
		"  gl_Position = PVM * vec4(position, 1.0f);\n"
		"}\n"
		;

	std::string fragmentShaderSrc =
		"#version 140\n"
		"out vec4 fragmentColor;"
		"void main() {\n"
		"  fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n"
		;

	GLuint shaders[] = {
	  pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
	  pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),
	  0
	};

	commonShaderProgram.program = pgr::createProgram(shaders);
	commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "position");

	// other attributes and uniforms
	commonShaderProgram.locations.PVMmatrix = glGetUniformLocation(commonShaderProgram.program, "PVM");

	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	// ...

	commonShaderProgram.initialized = true;
}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {

	pgr::deleteProgramAndShaders(commonShaderProgram.program);
}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	glm::mat4x4 viewMatrix = camera.getView();
	glm::mat4x4 projectionMatrix = camera.getProjection();

	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix);
	}
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the window contents (scene objects)
	drawScene();

	glutSwapBuffers();
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	// TODO: Take new window size and update the application state,
	// window and projection.

	// glViewport(...);
};

// -----------------------  Keyboard ---------------------------------

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {
	switch (keyPressed) {
	case 27:
		glutLeaveMainLoop();
		exit(EXIT_SUCCESS);
		break;
	case 'w':
	case 'W':
		keyPressedState['w'] = true;
		break;
	case 's':
	case 'S':
		keyPressedState['s'] = true;
		break;
	case 'a':
	case 'A':
		keyPressedState['a'] = true;
		break;
	case 'd':
	case 'D':
		keyPressedState['d'] = true;
		break;
	}
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
	switch (keyReleased) {
	case 'w':
	case 'W':
		keyPressedState['w'] = false;
		break;
	case 's':
	case 'S':
		keyPressedState['s'] = false;
		break;
	case 'a':
	case 'A':
		keyPressedState['a'] = false;
		break;
	case 'd':
	case 'D':
		keyPressedState['d'] = false;
		break;
	}
}

//
/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
	case GLUT_KEY_UP:
		keyPressedSpecialState[GLUT_KEY_UP] = true;
		break;
	case GLUT_KEY_DOWN:
		keyPressedSpecialState[GLUT_KEY_DOWN] = true;
		break;
	case GLUT_KEY_LEFT:
		keyPressedSpecialState[GLUT_KEY_LEFT] = true;
		break;
	case GLUT_KEY_RIGHT:
		keyPressedSpecialState[GLUT_KEY_RIGHT] = true;
		break;
	}
}

void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
	switch (specKeyReleased) {
	case GLUT_KEY_UP:
		keyPressedSpecialState[GLUT_KEY_UP] = false;
		break;
	case GLUT_KEY_DOWN:
		keyPressedSpecialState[GLUT_KEY_DOWN] = false;
		break;
	case GLUT_KEY_LEFT:
		keyPressedSpecialState[GLUT_KEY_LEFT] = false;
		break;
	case GLUT_KEY_RIGHT:
		keyPressedSpecialState[GLUT_KEY_RIGHT] = false;
		break;
	}
} // key released

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {
}

/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {
	int deltaX = previousMouseX - mouseX;
	int deltaY = previousMouseY - mouseY;

	camera.addYawPitch(deltaX * camera.getMouseSensitivity(), deltaY * camera.getMouseSensitivity());

	int halfWidth = WINDOW_WIDTH / 2;
	int halfHeight = WINDOW_HEIGHT / 2;

	glutWarpPointer(halfWidth, halfHeight);

	previousMouseX = halfWidth;
	previousMouseY = halfHeight;
}

// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 */
void timerCb(int)
{
#ifndef SKELETON // @task_1_0
	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);

	float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds
	glm::vec3 movementVector(0.0f);

	if (keyPressedState['w'])
	{ 
		movementVector += camera.getForward();
	}
	else
	if (keyPressedState['s'])
	{
		movementVector -= camera.getForward();
	}

	if (keyPressedState['a'])
	{
		movementVector -= camera.getRight();
	}
	else
	if (keyPressedState['d'])
	{
		movementVector += camera.getRight();
	}

	movementVector *= elapsedTime * camera.getSpeed();
	camera.setPosition(camera.getPosition() + movementVector);

	float rotationX = 0;
	float rotationY = 0;

	if (keyPressedSpecialState[GLUT_KEY_UP])
	{ 
		rotationX += 1 * elapsedTime * camera.getKeySensitivity();
	}
	else
	if (keyPressedSpecialState[GLUT_KEY_DOWN])
	{
		rotationX -= 1 * elapsedTime * camera.getKeySensitivity();
	}

	if (keyPressedSpecialState[GLUT_KEY_LEFT])
	{
		rotationY += 1 * elapsedTime * camera.getKeySensitivity();
	}
	else
	if (keyPressedSpecialState[GLUT_KEY_RIGHT])
	{
		rotationY -= 1 * elapsedTime * camera.getKeySensitivity();
	}

	camera.addYawPitch(rotationY, rotationX);

	// update the application state
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->update(elapsedTime, &sceneRootMatrix);
	}
#endif // task_1_0

	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}


// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...
	loadShaderPrograms();

	objects.push_back(new Triangle(&commonShaderProgram));
	// objects.push_back(new SingleMesh(&commonShaderProgram));

	// init your Application
	// - setup the initial application state
	glutSetCursor(GLUT_CURSOR_NONE);
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
	cleanupShaderPrograms();
}


/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {

	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow(WINDOW_TITLE);

		// callbacks - use only those you need
		glutDisplayFunc(displayCb);
		glutReshapeFunc(reshapeCb);
		glutKeyboardFunc(keyboardCb);
		glutKeyboardUpFunc(keyboardUpCb);
		glutSpecialFunc(specialKeyboardCb);     // key pressed
		glutSpecialUpFunc(specialKeyboardUpCb); // key released
		glutPassiveMotionFunc(passiveMouseMotionCb);
		// glutMouseFunc(mouseCb);
		// glutMotionFunc(mouseMotionCb);
#ifndef SKELETON // @task_1_0
		glutTimerFunc(33, timerCb, 0);
#else
		// glutTimerFunc(33, timerCb, 0);
#endif // task_1_0

	}
	// end for each window 

	// initialize pgr-framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	// code after glutLeaveMainLoop()
	// cleanup

	return EXIT_SUCCESS;
}
