#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLM.h>
#include "EulerCamera.h"
#include "SingleMeshLoader.h"

/*----------------------------------------------------------------------------
								  STRUCTS
----------------------------------------------------------------------------*/

struct LightStruct {

	// light properties
	glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.0f);	//Specular Reflected Light
	glm::vec3 Ld = glm::vec3(0.5f, 0.5f, 0.5f);	//Diffuse Surface Reflectance
	glm::vec3 La = glm::vec3(1.0f, 1.0f, 1.0f);	//Ambient Reflected Light
	glm::vec3 light = glm::vec3(0, 10, -5.0f);//light source location
	glm::vec3 coneDirection = light + glm::vec3(0.0f, -1.0f, 0.0f);
	float coneAngle = 40.0f;
	// object colour
	glm::vec3 Ks = glm::vec3(0.001f, 0.001f, 0.001f); // specular reflectance
	glm::vec3 Kd = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Ka = glm::vec3(1.0f, 1.0f, 1.0f); // ambient reflectance
	float specular_exponent = 0.5f; //specular 'power'?

};

/*----------------------------------------------------------------------------
								DEFINITIONS
----------------------------------------------------------------------------*/
GLuint createQuad(int location);
void drawObject(GLuint shaderID, EulerCamera cam, SingleMesh mesh, glm::mat4 model, bool cone, glm::vec3 light);
void drawObject(GLuint shaderID, EulerCamera cam, SingleMesh mesh, glm::mat4 model, bool cone, LightStruct light);

void drawCubeMap(GLuint shaderID, EulerCamera cam, SingleMesh mesh);
void drawCubeMap(GLuint shaderID, EulerCamera cam, SingleMesh mesh, LightStruct light);
/*----------------------------------------------------------------------------
								IMPLEMENTATIONS
----------------------------------------------------------------------------*/

GLuint createQuad(int location) {
	GLuint vao = 0, vbo = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float* verts;
	switch (location)
	{
	case 0:	//Bottom left of screen
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-1.0f, -0.5f,	0.0f, 1.0f,	//Top Left
			-0.5f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			-0.5f, -0.5f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 1:	//Bottom Right of screen
		verts = new float[16]
		{
			//positions		//textures
			0.5f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			0.5f, -0.5f,	0.0f, 1.0f,	//Top Left
			1.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			1.0f, -0.5f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 2:
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, 0.5f,	0.0f, 0.0f,	//Bottom Left
			-1.0f, 1.0f,	0.0f, 1.0f,	//Top Left
			-0.5f, 0.5f,	1.0f, 0.0f,	//Bottom Right
			-0.5f, 1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 3:
	default:
		verts = new float[16]
		{
			//positions		//textures
			0.5f, 0.5f,		0.0f, 0.0f,	//Bottom Left
			0.5f, 1.0f,		0.0f, 1.0f,	//Top Left
			1.0f, 0.5f,		1.0f, 0.0f,	//Bottom Right
			1.0f, 1.0f,		1.0f, 1.0f	//Top Right
		};
		break;
	case 4:	//Left of Screen
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-1.0f,  1.0f,	0.0f, 1.0f,	//Top Left
			 0.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			 0.0f,  1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 5:	//Right of Screen
		verts = new float[16]
		{
			//positions		//textures
			0.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			0.0f,  1.0f,	0.0f, 1.0f,	//Top Left
			1.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			1.0f,  1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 6:	//Bottom of Screen
		verts = new float[16]
		{
			//positions		//textures
			-0.5f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-0.5f, -0.5f,	0.0f, 1.0f,	//Top Left
			 0.5f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			 0.5f, -0.5f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 7:	//Entire Screen
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-1.0f,  1.0f,	0.0f, 1.0f,	//Top Left
			 1.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			 1.0f,  1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	}


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	GLintptr stride = 4 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, NULL);

	glEnableVertexAttribArray(1);
	GLintptr offset = 2 * sizeof(float);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)offset);
	return vao;
}

void drawObject(GLuint shaderID, EulerCamera cam, SingleMesh mesh, glm::mat4 model, bool cone, glm::vec3 light)
{
	glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.0f);	//Specular Reflected Light
	glm::vec3 Ld = glm::vec3(0.8f, 0.8f, 0.8f);	//Diffuse Surface Reflectance
	glm::vec3 La = glm::vec3(0.1f, 0.1f, 0.1f);	//Ambient Reflected Light


	glUseProgram(shaderID);
	glBindVertexArray(mesh.VAO[0]);
	//Light Setup
	glUniform3fv(glGetUniformLocation(shaderID, "Ls"), 1, &Ls[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Ld"), 1, &Ld[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "La"), 1, &La[0]);
	if (cone)
	{
		glm::vec3 coneDirection = light + glm::vec3(0.0f, -1.0f, 0.0f);
		float coneAngle = 40.0f;
		glUniform3fv(glGetUniformLocation(shaderID, "coneDirection"), 1, &coneDirection[0]);
		glUniform1f(glGetUniformLocation(shaderID, "coneAngle"), coneAngle);
	}
	//Matrix Setup
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &cam.getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);


	if (mesh.tex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.tex);
		glUniform1i(glGetUniformLocation(shaderID, "texture_primary"), 0);
	}

	if (mesh.norm)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh.norm);
		glUniform1i(glGetUniformLocation(shaderID, "normal_map"), 1);
	}
	glDrawArrays(GL_TRIANGLES, 0, mesh.mesh_vertex_count);
}

void drawObject(GLuint shaderID, EulerCamera cam, SingleMesh mesh, glm::mat4 model, bool cone, LightStruct light)
{
	glUseProgram(shaderID);
	glBindVertexArray(mesh.VAO[0]);
	//Light Setup
	glUniform3fv(glGetUniformLocation(shaderID, "Ls"), 1, &light.Ls[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Ld"), 1, &light.Ld[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "La"), 1, &light.La[0]);
	if (cone)
	{
		glUniform3fv(glGetUniformLocation(shaderID, "coneDirection"), 1, &light.coneDirection[0]);
		glUniform1f(glGetUniformLocation(shaderID, "coneAngle"), light.coneAngle);
	}
	//Matrix Setup
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &cam.getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);


	if (mesh.tex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.tex);
		glUniform1i(glGetUniformLocation(shaderID, "texture_primary"), 0);
	}

	if (mesh.norm)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh.norm);
		glUniform1i(glGetUniformLocation(shaderID, "normal_map"), 1);
	}
	glDrawArrays(GL_TRIANGLES, 0, mesh.mesh_vertex_count);
}

void drawCubeMap(GLuint shaderID, EulerCamera cam, SingleMesh mesh)
{
	glDepthMask(GL_FALSE);
	glUseProgram(shaderID);
	glBindVertexArray(mesh.VAO[0]);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderID, "cube_texture"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mesh.tex);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glm::mat4 view = cam.getView();
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &view[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, mesh.mesh_vertex_count);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void drawCubeMap(GLuint shaderID, EulerCamera cam, SingleMesh mesh, LightStruct light)
{
	glDepthMask(GL_FALSE);
	glUseProgram(shaderID);
	glBindVertexArray(mesh.VAO[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Ld"), 1, &light.Ld[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "La"), 1, &light.La[0]);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderID, "cube_texture"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mesh.tex);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glm::mat4 view = cam.getView();
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &view[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, mesh.mesh_vertex_count);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}
#endif