
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include "GLM.h"
#include <common/SingleMeshLoader.h>
#include <common/EulerCamera.h>
#include <common/Utilities.h>
#include <common/Shader.h>
#include <common/text.h>

using namespace std;
GLuint noTextureShaderID, bearShaderID, blackBearShaderID, variableShaderID, coneShaderID;
Shader shaderFactory;

const int width = 800, height = 800;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/
#define MONKEYHEAD_MESH "../Meshes/monkeyhead.obj"
#define WALL_MESH "../Meshes/wall.obj"
#define BEAR_MESH "../Meshes/bear.obj"
#define BEAR_TEXTURE "../Textures/bear.tga"

SingleMesh monkeyhead_mesh;
SingleMesh bear_mesh;
SingleMesh wall;

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

int textIDs[5] = { -1 };

/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/

#define NOTEXTURE_VERT "../Shaders/noTextureVertexShader.txt"
#define NOTEXTURE_FRAG "../Shaders/noTextureFragmentShader.txt"
#define BEAR_FRAG "../Shaders/bearFragmentShader.txt"
#define BLACKBEAR_FRAG "../Shaders/blackBearFragmentShader.txt"
#define VARIABLE_FRAG "../Shaders/variableFragmentShader.txt"
#define CONE_FRAG "../Shaders/coneLightFragmentShader.txt"

/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/
float fontSize = 25.0f;
glm::vec3 startingPos = { 0.0f, 0.0f, -10.0f };
float rotateY = 50.0f, rotateLight = 0.0f;

EulerCamera cam(startingPos, width, height);
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

GLuint VBO, containerVAO;

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	cam.setSensitivity(3.0f);
	//*************************//
	//*****Compile Shaders*****//
	//*************************//
	blackBearShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, BLACKBEAR_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	bearShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, BEAR_FRAG);
	variableShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, VARIABLE_FRAG);
	coneShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, CONE_FRAG);
	//*********************//
	//*****Init Meshes*****//
	//*********************//
	monkeyhead_mesh.init(MONKEYHEAD_MESH, NULL, NULL);
	bear_mesh.init(BEAR_MESH, BEAR_TEXTURE, NULL);
	wall.init(WALL_MESH, NULL, NULL);
}

void display() {

	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Create a black background

	// object properties
	glm::mat4 model;
	glm::vec3 light = glm::vec3(1.8*cos(rotateLight), 1.8*sin(rotateLight) + 1.0f, -5.0f);//light source location

	model = glm::rotate(glm::mat4x4(), rotateY, glm::vec3(0.0, 1.0, 0.0));
	drawObject(noTextureShaderID, cam, monkeyhead_mesh, model, false, light);
	
	model = glm::translate(glm::mat4x4(), glm::vec3(-4.0f, -4.0f, 0.0f));
	model = glm::rotate(model, rotateY, glm::vec3(0.0, 1.0, 0.0));
	drawObject(variableShaderID, cam, monkeyhead_mesh, model, false, light);

	model = glm::translate(glm::mat4x4(), glm::vec3(4.0f, 3.0f, 0.0f));
	model = glm::rotate(model, rotateY, glm::vec3(0.0, 1.0, 0.0));
	drawObject(bearShaderID, cam, bear_mesh, model, false, light);

	model = glm::translate(glm::mat4x4(), glm::vec3(-4.0f, 3.0f, 0.0f));
	model = glm::rotate(model, rotateY, glm::vec3(0.0, 1.0, 0.0));
	drawObject(blackBearShaderID, cam, bear_mesh, model, false, light);

	model = glm::translate(glm::mat4x4(), glm::vec3(4.0f, -4.0f, 0.0f));
	model = glm::rotate(model, rotateY, glm::vec3(0.0, 1.0, 0.0));
	drawObject(coneShaderID, cam, bear_mesh, model, true, light);

	model = glm::translate(glm::mat4x4(), glm::vec3(6.0f, -8.0f, 1.8f));
	model = glm::rotate(model, 90.0f, glm::vec3(0.0, 1.0, 0.0));
	drawObject(coneShaderID, cam, wall, model, false, light);

	draw_texts();
	glutSwapBuffers();
}

void updateScene() {
	static DWORD  last_frame;	//time when last frame was drawn
	static DWORD last_timer = 0;	//time when timer was updated
	DWORD  curr_time = timeGetTime();//for frame Rate Stuff.

	float  delta = (curr_time - last_frame) * 0.001f;
	if (delta >= 0.03f) 
	{
		last_frame = curr_time;
		glutPostRedisplay();
		rotateY = rotateY + 0.5f;
		rotateLight = rotateLight + 0.1f;
		if (rotateY >= 360.0f)
			rotateY = 0.0f;
		if (rotateLight >= 360.0f)
			rotateLight = 0.0f;
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) 
{
	if (key == (char)27)	//Pressing Escape Ends the game
	{
		exit(0);
	}
}

void keypressUp(unsigned char key, int x, int y)
{

}

void specialKeypress(int key, int x, int y)
{

}

void specialKeypressUp(int key, int x, int y)
{

}

void (mouse)(int x, int y)
{
	
}

#pragma endregion INPUT FUNCTIONS

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GameApp");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);


	// Tell glut where the display function is
	glutWarpPointer(width / 2, height / 2);
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	// Input Function Initialisers //
	glutKeyboardFunc(keypress);
	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(specialKeypress);
	glutSpecialUpFunc(specialKeypressUp);
	glutKeyboardUpFunc(keypressUp);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	init();
	textIDs[0] = add_text(
		"Averaged Channels",
		-0.9f, 0.4f, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	textIDs[1] = add_text(
		"Black Matte Color",
		0.3f, 0.4f, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	textIDs[2] = add_text(
		"Blinn Phong Shading",
		-0.3f, -0.2f, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	textIDs[3] = add_text(
		"Color based on gl_FragCoord",
		0.07f, -0.85f, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	textIDs[4] = add_text(
		"Model Subjected to a Spotlight",
		-1.0f, -0.85f, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);

	glutMainLoop();
	return 0;
}