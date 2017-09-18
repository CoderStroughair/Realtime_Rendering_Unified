
//Some Windows Headers (For Time, IO, etc.)
#include <time.h>
#include <common/Shader.h>
#include <common/Utilities.h>
#include <common/SingleMeshLoader.h>
#include <common/EulerCamera.h>
#include <common/Defines.h>
#include <GLM.h>
#include <common/text.h>

using namespace std;

const int width = 800, height = 800;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/

#define CUBE_TEXTURE "desert"

SingleMesh monkeyhead_mesh, torch_mesh, wall_mesh, cube, bear_mesh, sign_meshReflect, sign_meshRefract, sign_meshNormal;

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

GLfloat vertices[] = {
	-40.0f, -40.0f, -40.0f,  0.0f,  0.0f, -1.0f,
	40.0f, -40.0f, -40.0f,  0.0f,  0.0f, -1.0f,
	40.0f,  40.0f, -40.0f,  0.0f,  0.0f, -1.0f,
	40.0f,  40.0f, -40.0f,  0.0f,  0.0f, -1.0f,
	-40.0f,  40.0f, -40.0f,  0.0f,  0.0f, -1.0f,
	-40.0f, -40.0f, -40.0f,  0.0f,  0.0f, -1.0f,

	-40.0f, -40.0f,  40.0f,  0.0f,  0.0f,  1.0f,
	40.0f, -40.0f,  40.0f,  0.0f,  0.0f,  1.0f,
	40.0f,  40.0f,  40.0f,  0.0f,  0.0f,  1.0f,
	40.0f,  40.0f,  40.0f,  0.0f,  0.0f,  1.0f,
	-40.0f,  40.0f,  40.0f,  0.0f,  0.0f,  1.0f,
	-40.0f, -40.0f,  40.0f,  0.0f,  0.0f,  1.0f,

	-40.0f,  40.0f,  40.0f, -1.0f,  0.0f,  0.0f,
	-40.0f,  40.0f, -40.0f, -1.0f,  0.0f,  0.0f,
	-40.0f, -40.0f, -40.0f, -1.0f,  0.0f,  0.0f,
	-40.0f, -40.0f, -40.0f, -1.0f,  0.0f,  0.0f,
	-40.0f, -40.0f,  40.0f, -1.0f,  0.0f,  0.0f,
	-40.0f,  40.0f,  40.0f, -1.0f,  0.0f,  0.0f,

	40.0f,  40.0f,  40.0f,  1.0f,  0.0f,  0.0f,
	40.0f,  40.0f, -40.0f,  1.0f,  0.0f,  0.0f,
	40.0f, -40.0f, -40.0f,  1.0f,  0.0f,  0.0f,
	40.0f, -40.0f, -40.0f,  1.0f,  0.0f,  0.0f,
	40.0f, -40.0f,  40.0f,  1.0f,  0.0f,  0.0f,
	40.0f,  40.0f,  40.0f,  1.0f,  0.0f,  0.0f,

	-40.0f, -40.0f, -40.0f,  0.0f, -1.0f,  0.0f,
	40.0f, -40.0f, -40.0f,  0.0f, -1.0f,  0.0f,
	40.0f, -40.0f,  40.0f,  0.0f, -1.0f,  0.0f,
	40.0f, -40.0f,  40.0f,  0.0f, -1.0f,  0.0f,
	-40.0f, -40.0f,  40.0f,  0.0f, -1.0f,  0.0f,
	-40.0f, -40.0f, -40.0f,  0.0f, -1.0f,  0.0f,

	-40.0f,  40.0f, -40.0f,  0.0f,  1.0f,  0.0f,
	40.0f,  40.0f, -40.0f,  0.0f,  1.0f,  0.0f,
	40.0f,  40.0f,  40.0f,  0.0f,  1.0f,  0.0f,
	40.0f,  40.0f,  40.0f,  0.0f,  1.0f,  0.0f,
	-40.0f,  40.0f,  40.0f,  0.0f,  1.0f,  0.0f,
	-40.0f,  40.0f, -40.0f,  0.0f,  1.0f,  0.0f
};

/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/


#define SKY_VERT "../Shaders/skyboxVertexShader.txt"
#define SKY_FRAG "../Shaders/skyboxFragmentShader.txt"

#define REFLECTIVE_FRAG "../Shaders/reflectiveFragmentShader.txt"
#define FRESNEL_FRAG	"../Shaders/fresnelFragmentShader.txt"

GLuint noTextureShaderID, textureShaderID, cubeMapTextureID, refractiveShaderID, cubeMapShaderID, testID, normalisedShaderID, reflectiveShaderID;
Shader shaderFactory;

/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/
GLfloat lastx = width / 2.0f;
GLfloat lasty = height / 2.0f;
float fontSize = 25.0f;
glm::vec3 startingPos = { 0.0f, 0.0f, -20.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;

EulerCamera cam(startingPos, width, height);
//yaw set to -90 because yaw of 0.0 points to right or something weird happens in euler angles

/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/


void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}

	cam.setSensitivity(1.0f);
	//*************************//
	//*****Compile Shaders*****//
	//*************************//
	refractiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, FRESNEL_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory.CompileShader(SKY_VERT, SKY_FRAG);
	textureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	normalisedShaderID = shaderFactory.CompileShader(NORMAL_VERT, NORMAL_FRAG);
	reflectiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, REFLECTIVE_FRAG);

	testID = shaderFactory.CompileShader(TEST_VERT, TEST_FRAG);
	//*********************//
	//*****Init Meshes*****//
	//*********************//
	monkeyhead_mesh.init(MONKEYHEAD_MESH, NULL, NULL);
	cube.initCubeMap(vertices, 36, CUBE_TEXTURE);
	torch_mesh.init(TORCH_MESH, NULL, NULL);

	bear_mesh.init(BEAR_MESH, BEAR_TEXTURE, NULL);
	wall_mesh.init(WALL_MESH, BRICK_NORMAL, NULL);
	sign_meshReflect.init(SIGN_MESH, REFLECT_TEXTURE, NULL);
	sign_meshRefract.init(SIGN_MESH, REFRACT_TEXTURE, NULL);
	sign_meshNormal.init(SIGN_MESH, NORMAL_TEXTURE, NULL);
}

void display() {

	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);					//Create a black background


	glm::mat4 view, model, proj;

	LightStruct light;

	light.Kd = WHITE;
	model = glm::mat4();
	drawCubeMap(cubeMapShaderID, cam, cube);
	model = glm::scale(glm::mat4(), glm::vec3(0.01f, 0.01f, 0.01f));
	model = glm::translate(model, light.light);
	drawObject(noTextureShaderID, cam, cube, model, false, light);

	light.Kd = BROWN;
	model = glm::scale(glm::mat4(), glm::vec3(2.0, 2.0, 2.0));
	model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(5.0, -6.0, 0.0));
	drawObject(textureShaderID, cam, bear_mesh, model, false, light);

	light.Kd = WHITE;
	model = glm::rotate(glm::mat4(), rotateY, glm::vec3(0.0, 1.0, 0.0));
	drawObject(refractiveShaderID, cam, monkeyhead_mesh, model, false, light);

	model = glm::rotate(glm::mat4(), 180.0f, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(0.0, -3.0f, -2.5f));
	drawObject(textureShaderID, cam, sign_meshRefract, model, false, light);

	light.Kd = BROWN;
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -6.3f, 0.0f));
	drawObject(noTextureShaderID, cam, torch_mesh, model, false, light);

	light.Kd = GREY;
	model = glm::rotate(glm::mat4(), 90.0f, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(7.4f, -6.5f, 0.0f));
	light.light = -light.light;
	light.specular_exponent = 50.0f;
	drawObject(normalisedShaderID, cam, wall_mesh, model, false, light);

	light.Kd = WHITE;
	light.light = -light.light;
	light.specular_exponent = 0.5;

	glm::mat4 rootmodel = glm::translate(glm::mat4(), glm::vec3(-16.0, 0.0, 0.0));

	model = glm::scale(glm::mat4(), glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
	model = rootmodel * model;
	drawObject(reflectiveShaderID, cam, cube, model, false, light);

	model = glm::rotate(glm::mat4(), 180.0f, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(0.0, -3.0f, -2.5f));
	model = rootmodel*model;
	drawObject(textureShaderID, cam, sign_meshReflect, model, false, light);

	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -6.3f, 0.0f));
	model = rootmodel*model;
	drawObject(noTextureShaderID, cam, torch_mesh, model, false, light);

	light.Kd = GREY;
	model = glm::rotate(glm::mat4(), 90.0f, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(7.4f, -6.5f, 0.0f));
	model = rootmodel*model;
	light.light = -light.light;
	light.specular_exponent = 50.0f;
	drawObject(normalisedShaderID, cam, wall_mesh, model, false, light);

	//draw_texts();
	glutSwapBuffers();
}

void updateScene() {
	static clock_t lastFrame = clock();
	clock_t currFrame = clock();
	float delta = (currFrame - lastFrame) / (float)CLOCKS_PER_SEC;
	if (delta >= 0.03f) 
	{
		lastFrame = currFrame;
		rotateY = rotateY + 0.5f;
		rotateLight = rotateLight + 0.1f;
		if (rotateY >= 360.0f)
			rotateY = 0.0f;
		if (rotateLight >= 360.0f)
			rotateLight = 0.0f;
		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);
		glutPostRedisplay();
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) 
{
	if (key == (char)27)	//Pressing Escape Ends the game
	{
		exit(0);
	}
	else if (key == 'w' || key == 'W')
		frontCam = 1;
	else if ((key == 's') || (key == 'S'))
		frontCam = -1;
	if ((key == 'a') || (key == 'A'))
		sideCam = -1;
	else if ((key == 'd') || (key == 'D'))
		sideCam = 1;
}

void keypressUp(unsigned char key, int x, int y)
{
	if ((key == 'w') || (key == 'W'))
		frontCam = 0;
	else if ((key == 's') || (key == 'S'))
		frontCam = 0;
	if ((key == 'a') || (key == 'A'))
		sideCam = 0;
	else if ((key == 'd') || (key == 'D'))
		sideCam = 0;
}

void specialKeypress(int key, int x, int y) 
{
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 4;
		break;
	case (GLUT_KEY_LEFT):
		yawCam = -1;
		break;
	case (GLUT_KEY_RIGHT):
		yawCam = 1;
		break;
	case (GLUT_KEY_UP):
		pitCam = 1;
		break;
	case (GLUT_KEY_DOWN):
		pitCam = -1;
		break;
	}
}

void specialKeypressUp(int key, int x, int y) 
{
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 1;
		break;
	case (GLUT_KEY_LEFT):
		yawCam = 0;
		break;
	case (GLUT_KEY_RIGHT):
		yawCam = 0;
		break;
	case (GLUT_KEY_UP):
		pitCam = 0;
		break;
	case (GLUT_KEY_DOWN):
		pitCam = 0;
		break;
	}
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
	glutMainLoop();
	return 0;
}