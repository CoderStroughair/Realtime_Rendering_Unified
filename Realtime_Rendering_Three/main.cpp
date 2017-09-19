
//Some Windows Headers (For Time, IO, etc.)
#include <time.h>
#include <common/Shader.h>
#include <common/Utilities.h>
#include <common/SingleMeshLoader.h>
#include <common/EulerCamera.h>
#include <common/Defines.h>
#include <GLM.h>
#include <common/text.h>
#include <common/Framebuffer.h>

using namespace std;

const float width = 800, height = 800;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/

SingleMesh monkeyhead_object, torch_object, wall_object, cube, bear_object, signReflect_object, signRefract_object, signNormal_object;
SingleMesh gem_object, sphere_object;

//Cube Vertices for the Skybox
GLfloat vertices[] = {
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,

	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,

	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,

	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,

	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,

	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f
}; 

/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint noTextureShaderID, textureShaderID, cubeMapTextureID, refractiveShaderID, cubeMapShaderID;
GLuint testID, normalisedShaderID, reflectiveShaderID, multiTextureShaderID, mirrorShaderID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/
GLfloat playerHeight = 0.0f;
GLfloat lastx = width / 2.0f;
GLfloat lasty = height / 2.0f;
float fontSize = 25.0f;
glm::vec3 startingPos = { 0.0f, playerHeight, -20.0f };
glm::vec3 startingFront = { 0.0f, playerHeight, 1.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;

EulerCamera cam(startingPos, width, height);
EulerCamera mirrorCam(glm::vec3(0.0, 0.0, 0.0), width, height);

//yaw set to -90 because yaw of 0.0 points to right or something weird happens in euler angles
GLfloat viewDistance = 5.0f;
GLfloat cos45 = 0.53;
GLfloat sin45 = 0.85;
GLfloat  one = 0.99999999999;
GLfloat  zero = 0.00000000001;
glm::vec3 views[6] = { 
	glm::vec3(1.0f, 0.0, 0.0), glm::vec3(-1.0f, 0.0, 0.0),
	glm::vec3(zero, -one, 0.0), glm::vec3(zero, one, 0.0),
	glm::vec3(0.0, 0.0, 1.0f), glm::vec3(0.0, 0.0, -1.0f)
};
glm::vec3 viewsX[6] = {
	glm::vec3(one, 0.0, 0.0), glm::vec3(-one, 0.0, 0.0),
	glm::vec3(zero, 0.71f, -0.71f), glm::vec3(zero, -0.71f, -0.71f),
	glm::vec3(zero, -0.71f, 0.71f), glm::vec3(zero, 0.71f, -0.71f)
};
glm::vec3 viewsY[6] = {
	glm::vec3(0.71f, 0.0, -0.71f), glm::vec3(-0.71f, 0.0, 0.71f),
	glm::vec3(cos45*0.1, one, cos45*-0.1), glm::vec3(cos45*0.1, -one, cos45*-0.1),
	glm::vec3(0.71f, 0.0, 0.71f), glm::vec3(-0.71f, 0.0, -0.71f)
};
glm::vec3 viewsZ[6] = {
	glm::vec3(-0.71f, 0.71f, 0.0), glm::vec3(0.71f, -0.71f, 0.0),
	glm::vec3(0.71f, 0.71f, 0.0), glm::vec3(-0.71f, -0.71f, 0.0),
	glm::vec3(0.0, zero, -one), glm::vec3(0.0, zero, one)
};

/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/
const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

Framebuffer fb[4][6];
int textID = -1;
void drawLoop(EulerCamera &cam, GLuint framebuffer);
void drawReflectiveObject(EulerCamera &cam, SingleMesh &mesh, glm::mat4 model);
void createCubemap(GLuint& cubemap, int i);

GLuint cubemap[4];
const int numGems = 300;
glm::vec3 gemLocation[numGems];
glm::vec3 gemColour[numGems];
bool simple = true, mesh = true;
/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	cam.setSensitivity(2.0f);
	mirrorCam.setSensitivity(2.0f);
	mirrorCam.setFront(glm::vec3(0.0, 0.0, -1.0), 90, 0);
	//*************************//
	//*****Compile Shaders*****//
	//*************************//
	refractiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, FRESNEL_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory.CompileShader(SKY_VERT, SKY_FRAG);
	textureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	normalisedShaderID = shaderFactory.CompileShader(NORMAL_VERT, NORMAL_FRAG);
	reflectiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, REFLECTIVE_FRAG);
	multiTextureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	mirrorShaderID = shaderFactory.CompileShader(MIRROR_VERT, MIRROR_FRAG);

	testID = shaderFactory.CompileShader(TEST_VERT, TEST_FRAG);
	//*********************//
	//*****Init Objects*****//
	//*********************//
	monkeyhead_object.init(MONKEYHEAD_MESH, NULL, NULL);
	cube.initCubeMap(vertices, 36, "");
	torch_object.init(TORCH_MESH, NULL, NULL);
	torch_object.mode = GL_QUADS;
	bear_object.init(BEAR_MESH, BEAR_TEXTURE, NULL);
	wall_object.init(WALL_MESH, BRICK_TEXTURE, BRICK_NORMAL);
	signReflect_object.init(SIGN_MESH, REFLECT_TEXTURE, NULL);
	signRefract_object.init(SIGN_MESH, REFRACT_TEXTURE, NULL);
	signNormal_object.init(SIGN_MESH, NORMAL_TEXTURE, NULL);
	sphere_object.init(SPHERE_MESH, NULL, NULL);
	gem_object.init(GEM_MESH, NULL, NULL);


	for (int h = 0; h < 4; h++)
	{
		for (int i = 0; i < 6; i++)
		{
			fb[h][i].init(width, height);
		}
		createCubemap(cubemap[h], h);
	}
	// disable
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void display() 
{
	for (int i = 0; i < 6; i++)
	{
		mirrorCam.setFront(mirrorCam.getPosition() + glm::normalize(views[i]), 90, 0);
		mirrorCam.setUp(glm::vec3(0.0, -1.0, 0.0));
		drawLoop(mirrorCam, fb[0][i].framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap[0], 0);

	}
	/*for (int i = 0; i < 6; i++)
	{
		vec3 position = mirrorCam.getPosition();
		view = look_at(position, position + normalise(viewsX[i]), vec3(0.0, -0.5, -0.5));
		if (i == 2)
			view = look_at(position, position + normalise(viewsX[4]), vec3(0.0, -0.5, -0.5));
		else if (i == 3)
			view = look_at(position, position + normalise(viewsX[i]), viewsX[5]);
		drawLoop(mirrorCam, fb[1][i].framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap[1], 0);
	}
	for (int i = 0; i < 6; i++)
	{
		vec3 position = mirrorCam.getPosition();
		view = look_at(position, position + normalise(viewsY[i]), vec3(0.0, -1, 0.0));
		if (i == 2)
			view = look_at(position, position + normalise(viewsY[i]), viewsY[5]);
		else if (i == 3)
			view = look_at(position, position + normalise(viewsY[i]), viewsY[5]);
		drawLoop(mirrorCam, fb[2][i].framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap[2], 0);
	}
	for (int i = 0; i < 6; i++)
	{
		vec3 position = mirrorCam.getPosition();
		view = look_at(position, position + normalise(viewsZ[i]), vec3(-0.5, -0.5, 0.0));
		if (i == 2)
			view = look_at(position, position + normalise(viewsZ[i]), viewsZ[3]);
		else if (i == 3)
			view = look_at(position, position + normalise(viewsZ[i]), viewsZ[5]);
		drawLoop(mirrorCam, fb[3][i].framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap[3], 0);
	}*/


	drawLoop(cam, 0);



	glm::mat4 model = glm::rotate(glm::mat4(), 45.0f, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::translate(model, mirrorCam.getPosition());

	LightStruct lightContainer;
	lightContainer.Ls = glm::vec3(1.0f, 1.0f, 1.0f);	//Specular Reflected Light
	lightContainer.Ld = glm::vec3(0.5f, 0.5f, 0.5f);	//Diffuse Surface Reflectance
	lightContainer.La = glm::vec3(1.0f, 1.0f, 1.0f);	//Ambient Reflected Light
	lightContainer.lightLocation = glm::vec3(5 * sin(rotateLight/2.0f), 10, -5.0f*cos(rotateLight/2.0f));//light source location
	// object colour
	lightContainer.Ks = glm::vec3(0.1f, 0.1f, 0.1f); // specular reflectance
	lightContainer.Kd = WHITE;
	lightContainer.Ka = glm::vec3(0.05f, 0.05f, 0.05f); // ambient reflectance
	lightContainer.specular_exponent = 0.5f; //specular exponent - size of the specular elements

	SingleMesh temp;
	if (mesh)
		temp = sphere_object;
	if(!mesh)
		temp = monkeyhead_object;

	model = glm::rotate(glm::mat4(), 20 * rotateLight, glm::vec3(0.0, 1.0, 0.0));
	drawReflectiveObject(cam, temp, model);

	draw_texts();
	glutSwapBuffers();
}

void updateScene() {
	static clock_t lastFrame = clock();
	clock_t currFrame = clock();
	float delta = (currFrame - lastFrame) / (float)CLOCKS_PER_SEC;
	if (delta >= 0.03f) 
	{
		lastFrame = currFrame;
		glutPostRedisplay();
		rotateY = rotateY + 0.5f;
		rotateLight = rotateLight + 0.01f;
		if (rotateY >= 360.0f)
			rotateY = 0.0f;
		if (rotateLight >= 360.0f)
			rotateLight = 0.0f;
		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);
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
	if (key == ' ')
	{
		simple = !simple;
		if (simple)
			update_text(textID, "Simplified Version - This only shows a single cubemap");
		else
			update_text(textID, "Complex Version - This shows the use of all four cubemaps");
	}
	if (key == 'm')
	{
		mesh = !mesh;
	}
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
	textID = add_text(
		"Simplified Version - This only shows a single cubemap",
		-0.9, 0.8, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < sizeof(gemLocation) / sizeof(*gemLocation); i++)
	{
		float x = rand() % 50 - 25;
		float z = rand() % 50 - 23;
		gemLocation[i] = glm::vec3(x, -6.5f, z);
		int colourNumber = rand() % 4;
		if (colourNumber == 0)
			gemColour[i] = RED;
		else if (colourNumber == 1)
			gemColour[i] = PURPLE;
		else if (colourNumber == 2)
			gemColour[i] = GREEN;
		else
			gemColour[i] = YELLOW;
	}
	glutMainLoop();
	return 0;
}

void drawLoop(EulerCamera &cam, GLuint framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);					//Create a background


	glm::mat4 model = glm::mat4();
	LightStruct lightContainer;
	lightContainer.Ls = glm::vec3(1.0f, 1.0f, 1.0f);	//Specular Reflected Light
	lightContainer.Ld = glm::vec3(0.99f, 0.99f, 0.99f);	//Diffuse Surface Reflectance
	lightContainer.La = glm::vec3(1.0f, 1.0f, 1.0f);	//Ambient Reflected Light
	lightContainer.lightLocation = glm::vec3(5 * sin(rotateLight), 10, -5.0f*cos(rotateLight));//light source location
	// object colour
	lightContainer.Ks = glm::vec3(0.1f, 0.1f, 0.1f); // specular reflectance
	lightContainer.Kd = WHITE;
	lightContainer.Ka = glm::vec3(0.05f, 0.05f, 0.05f); // ambient reflectance
	lightContainer.specular_exponent = 0.5f; //specular exponent - size of the specular elements

	drawCubeMap(cubeMapShaderID, cam, cube);

	model = glm::translate(model, lightContainer.lightLocation);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	drawObject(noTextureShaderID, cam, cube, model, false, lightContainer);

	lightContainer.Kd = BROWN;
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -6.3f, 0.0f));
	drawObject(noTextureShaderID, cam, torch_object, model, false, lightContainer);

	lightContainer.Kd = GREY;
	model = glm::translate(glm::mat4(), glm::vec3(-7.5, -6.5f, -7.5));
	model = glm::rotate(model, 90.0f, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 root = model;
	lightContainer.specular_exponent = 50.0f;
	lightContainer.Ks = glm::vec3(1.0, 1.0, 1.0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			model = glm::translate(glm::mat4(), glm::vec3(16.0f*i, 0.0, 0.0));
			model = glm::translate(model, glm::vec3(0.0, 0.0, 32.0f*j));
			model = model * root;
			drawObject(normalisedShaderID, cam, wall_object, model, false, lightContainer);
		}
	}


	lightContainer.specular_exponent = 0.3f; //specular exponent - size of the specular elements
	lightContainer.Ks = glm::vec3(0.8f, 0.8f, 0.8f);
	lightContainer.Ka = glm::vec3(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < sizeof(gemLocation) / sizeof(*gemLocation); i++)
	{
		lightContainer.Ka = gemColour[i]/1.5f;
		lightContainer.Kd = gemColour[i];
		model = glm::translate(glm::mat4(), gemLocation[i]);
		model = glm::rotate(model, 300 * rotateLight, glm::vec3(0.0, 1.0, 0.0));
		drawObject(noTextureShaderID, cam, gem_object, model, false, lightContainer);
	}
}

void drawReflectiveObject(EulerCamera &cam, SingleMesh &mesh, glm::mat4 model)
{
	glUseProgram(reflectiveShaderID);
	glBindVertexArray(mesh.VAO[0]);
	glUniform4fv(glGetUniformLocation(reflectiveShaderID, "camPos"), 1, &cam.getPosition()[0]);
	glUniformMatrix4fv(glGetUniformLocation(reflectiveShaderID, "view"), 1, GL_FALSE, &cam.getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(reflectiveShaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(reflectiveShaderID, "model"), 1, GL_FALSE, &model[0][0]);
	glUniform1i(glGetUniformLocation(reflectiveShaderID, "simple"), simple);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
	glUniform1ui(glGetUniformLocation(reflectiveShaderID, "cube_texture"), 0);
	//glUniform1i(0, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
	glUniform1ui(glGetUniformLocation(reflectiveShaderID, "cube_textureX"), 1);
	//glUniform1i(1, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[2]);
	glUniform1i(glGetUniformLocation(reflectiveShaderID, "cube_textureY"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[3]);
	glUniform1i(glGetUniformLocation(reflectiveShaderID, "cube_textureZ"), 3);


	glDrawArrays(mesh.mode, 0, mesh.mesh_vertex_count);
}

void createCubemap(GLuint& cubemap, int i)
{
	// create the cubemap
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set textures
	for (int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}