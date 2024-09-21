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
Model Carro_Cuerpo;
Model Carro_Llanta01;
Model Carro_Llanta02;
Model Carro_Llanta03;
Model Carro_Llanta04;
Model Carro_Cofre;
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

	Carro_Cuerpo = Model();
	Carro_Cuerpo.LoadModel("Models/Carro_Cuerpo.obj");
	Carro_Cofre = Model();
	Carro_Cofre.LoadModel("Models/Carro_Cofre.obj");
	Carro_Llanta01 = Model();
	Carro_Llanta01.LoadModel("Models/Carro_Llanta01.obj");
	Carro_Llanta02 = Model();
	Carro_Llanta02.LoadModel("Models/Carro_Llanta02.obj");
	Carro_Llanta03 = Model();
	Carro_Llanta03.LoadModel("Models/Carro_Llanta03.obj");
	Carro_Llanta04 = Model();
	Carro_Llanta04.LoadModel("Models/Carro_Llanta04.obj");

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
	glm::mat4 Cofre(1.0);		//Jerarquia con Cofre.
	glm::mat4 Llanta01(1.0);	//Jerarquia con llanta 1.
	glm::mat4 Llanta02(1.0);	//Jerarquia con llanta 2.
	glm::mat4 Llanta03(1.0);	//Jerarquia con llanta 3.
	glm::mat4 Llanta04(1.0);	//Jerarquia con llanta 4.

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

		//ACTIVIDAD 1.- Importar su modelo de coche propio dentro del escenario a una escala adecuada.

		model = glm::mat4(1.0);

		//Carro Base.
		color = glm::vec3(0.5f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion3(),-0.25f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		Cofre = model;
		Llanta01 = model;
		Llanta02 = model;
		Llanta03 = model;
		Llanta04 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cuerpo.RenderModel();
		
		//ACTIVIDAD 2.- Importar sus 4 llantas.

		//Articulación - Llanta01. Derecha.
		model = Llanta01;
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(30.0f, -20.0f, 70.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta01 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llanta01.RenderModel();

		//Articulación - Llanta02. Izquierda.
		model = Llanta02;
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(-30.0f, -20.0f, 70.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta02 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llanta02.RenderModel();

		//Articulación - Llanta03. Izquierda.
		model = Llanta03;
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(-30.0f, -21.0f, -55.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta03 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llanta03.RenderModel();

		//Articulación - Llanta04. Derecha.
		model = Llanta04;
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(30.0f, -21.0f, -55.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta04 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llanta04.RenderModel();

		//ACTIVIDAD 3.- Importar el cofre del coche.

		//Articulación - Cofre de carro.
		model = Cofre;
		color = glm::vec3(0.3f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 11.0f, 50.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		Cofre = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cofre.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();}
	return 0;}