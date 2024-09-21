//Ejercicio Práctica 5: Optimización y Carga de Modelos		Uriarte Ortiz Enrique Yahir.
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Goddard_Cuerpo;
Model Goddard_Cabeza;
Model Goddard_MandInf;
Model Goddard_Pata01;
Model Goddard_Pata02;
Model Goddard_Pata03;
Model Goddard_Pata04;
Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
static const char* vShader = "shaders/shader_m.vert";
static const char* fShader = "shaders/shader_m.frag";

void CreateObjects(){
	unsigned int indices[] = {0,3,1,   1,3,2,   2,3,0,   0,1,2};
	GLfloat vertices[] = {//  x      y      z		 u	   v		 nx	   ny    nz
							-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
							 0.0f, -1.0f,  1.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
							 1.0f, -1.0f, -0.6f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
							 0.0f,  1.0f,  0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f};
	unsigned int floorIndices[] = {0,2,1,   1,2,3};
	GLfloat floorVertices[] = {
							-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
							 10.0f, 0.0f, -10.0f,	10.0f,  0.0f,	0.0f, -1.0f, 0.0f,
							-10.0f, 0.0f,  10.0f,	 0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
							 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, -1.0f, 0.0f};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);}

int main(){
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	CreateObjects();
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	Goddard_Cuerpo = Model();
	Goddard_Cuerpo.LoadModel("Models/goddard_cuerpo.obj");
	Goddard_Cabeza = Model();
	Goddard_Cabeza.LoadModel("Models/goddard_cabeza.obj");
	Goddard_MandInf = Model();
	Goddard_MandInf.LoadModel("Models/goddard_mandibula-inferior.obj");
	Goddard_Pata01 = Model();
	Goddard_Pata01.LoadModel("Models/goddard_pata01.obj");
	Goddard_Pata02 = Model();
	Goddard_Pata02.LoadModel("Models/goddard_pata02.obj");
	Goddard_Pata03 = Model();
	Goddard_Pata03.LoadModel("Models/goddard_pata03.obj");
	Goddard_Pata04 = Model();
	Goddard_Pata04.LoadModel("Models/goddard_pata04.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);		//Origen de las jererquias.
	glm::mat4 mandibula(1.0);	//Jerarquia con mandibula.
	glm::mat4 pata_01(1.0);		//Jerarquia con pata 1.
	glm::mat4 pata_02(1.0);		//Jerarquia con pata 2.
	glm::mat4 pata_03(1.0);		//Jerarquia con pata 3.
	glm::mat4 pata_04(1.0);		//Jerarquia con pata 4.

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	while (!mainWindow.getShouldClose()){

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		color = glm::vec3(1.0f, 0.4f, 0.1f); //Dibujo de Piso.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//ACTIVIDAD 1.- Importar por separado y agregar jerarquía: Cuerpo, Cabeza, Mandibula inferior y cada Pata por separado.

		model = glm::mat4(1.0);

		//Goddard Base.
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		mandibula = model;
		pata_01 = model;
		pata_02 = model;
		pata_03 = model;
		pata_04 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Cuerpo.RenderModel();
		
		//Goddard Cabeza.
		model = mandibula;
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		model = glm::translate(model, glm::vec3(1.6f, 0.7f, 0.0f));
		mandibula = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Cabeza.RenderModel();

		//Articulación - Goddard Mandibula.
		model = mandibula;
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		model = glm::translate(model, glm::vec3(1.52f, 0.25f, 0.052f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, -1.0f));
		mandibula = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_MandInf.RenderModel();

		//Articulación - Pata01. Izquierda.
		model = pata_01;
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		model = glm::translate(model, glm::vec3(1.5f, -0.2f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, -1.0f));
		pata_01 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata01.RenderModel();

		//Articulación - Pata02. Derecha.
		model = pata_02;
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		model = glm::translate(model, glm::vec3(1.5f, -0.2f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, -1.0f));
		pata_02 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata02.RenderModel();

		//Articulación - Pata03. Izquierda.
		model = pata_03;
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.9f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
		pata_03 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata03.RenderModel();

		//Articulación - Pata04. Derecha.
		model = pata_04;
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.9f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, -1.0f));
		pata_04 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata04.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
