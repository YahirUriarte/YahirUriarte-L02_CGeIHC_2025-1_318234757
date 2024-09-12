//Práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"

//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z

using std::vector;

const float toRadians = 3.14159265f/180.0;
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20);

void Piramide(){
	unsigned int indices_piramide_triangular[] = {0,1,2,  1,3,2,  3,0,2,  1,0,3};
	GLfloat vertices_piramide_triangular[] = {
		-0.9f,0.0f,0.0f,	0.9f,0.0f,0.0f,		0.0f,0.0f,1.56f,	0.0f,1.47f,0.52f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);}

void T01(){//Triangulo 1.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		0.25f,-0.01f,1.09f,		-0.25f,-0.01f,1.09f,		0.0f,-0.01f,1.52f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T02(){//Triangulo 2.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		-0.05f,-0.01f, 0.56f,   -0.55f,-0.01f,0.56f,   -0.3f,-0.01f,0.99f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T03(){//Triangulo 3.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		-0.25f,-0.01f,1.04f,	0.25f,-0.01f,1.04f,   0.0f,-0.01f,0.61f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T04(){//Triangulo 4.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		0.3f,-0.01f,0.99f,		0.05f,-0.01f,0.56f,		0.55f,-0.01f,0.56f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T05(){//Triangulo 5.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		-0.6f,-0.01f, 0.46f,	-0.85f,-0.01f,0.03f,	-0.35f,-0.01f,0.03f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}

void T06(){//Triangulo 6.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		-0.55f,-0.01f,0.51f,	-0.05f,-0.01f,0.51f,	-0.3f,-0.01f,0.08f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T07(){//Triangulo 7.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		0.0f,-0.01f, 0.46f,   0.25f,-0.01f,0.03f,  -0.25f ,-0.01f, 0.03f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T08(){//Triangulo 8.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		0.55f,-0.01f,0.51f,		0.05f,-0.01f,0.51f,		0.3f,-0.01f,0.08f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}
void T09(){//Triangulo 9.
	unsigned int indices_piramide_triangular[] = { 0,1,2 };
	GLfloat vertices_piramide_triangular[] = {
		0.6f,-0.01f,0.46f,		0.85f,-0.01f,0.03f,		0.35f,-0.01f,0.03f};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 9, 3);
	meshList.push_back(obj1);}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);}

int main(){

	mainWindow = Window(800, 800);
	mainWindow.Initialise();

	Piramide();
	T01();T02();T03();T04(); T05(); T06(); T07(); T08(); T09();

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);	
	GLuint uniformProjection = 0;GLuint uniformModel = 0;GLuint uniformView = 0;GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	sp.init();sp.load();
	glm::mat4 model(1.0);
	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f);

	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//Piramide principal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.5f));
		model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//Triangulos de la Primera Cara - Rojos
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh(); meshList[2]->RenderMesh(); meshList[3]->RenderMesh();
		meshList[4]->RenderMesh(); meshList[5]->RenderMesh(); meshList[6]->RenderMesh();
		meshList[7]->RenderMesh(); meshList[8]->RenderMesh(); meshList[9]->RenderMesh();

		//Triangulos de la Segunda Cara - Verde
		glm::mat4 rotatedModel = model;
		rotatedModel = glm::translate(rotatedModel, glm::vec3(0.0f, 0.01f, -0.008f));
		rotatedModel = glm::rotate(rotatedModel, glm::radians(288.785f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rotatedModel));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh(); meshList[2]->RenderMesh(); meshList[3]->RenderMesh();
		meshList[4]->RenderMesh(); meshList[5]->RenderMesh(); meshList[6]->RenderMesh();
		meshList[7]->RenderMesh(); meshList[8]->RenderMesh(); meshList[9]->RenderMesh();

		//Triangulos de la Tercer Cara - Azul
		glm::mat4 rotatedModel1 = model;
		rotatedModel1 = glm::translate(rotatedModel1, glm::vec3(0.0f, 0.01f, -0.008f));
		rotatedModel1 = glm::rotate(rotatedModel1, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotatedModel1 = glm::rotate(rotatedModel1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedModel1 = glm::translate(rotatedModel1, glm::vec3(-0.69f, 0.0f, 0.0f));
		rotatedModel1 = glm::rotate(rotatedModel1, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotatedModel1 = glm::rotate(rotatedModel1, glm::radians(18.785f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotatedModel1 = glm::translate(rotatedModel1, glm::vec3(0.15f, 0.425f, -0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rotatedModel1));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh(); meshList[2]->RenderMesh(); meshList[3]->RenderMesh();
		meshList[4]->RenderMesh(); meshList[5]->RenderMesh(); meshList[6]->RenderMesh();
		meshList[7]->RenderMesh(); meshList[8]->RenderMesh(); meshList[9]->RenderMesh();

		//Triangulos de la Tercer Cara - Amarillo
		glm::mat4 rotatedModel2 = model;
		rotatedModel2 = glm::translate(rotatedModel2, glm::vec3(0.0f, 0.01f, -0.008f));
		rotatedModel2 = glm::rotate(rotatedModel2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotatedModel2 = glm::rotate(rotatedModel2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedModel2 = glm::translate(rotatedModel2, glm::vec3(-0.69f, 0.0f, 0.0f));
		rotatedModel2 = glm::rotate(rotatedModel2, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotatedModel2 = glm::rotate(rotatedModel2, glm::radians(-18.785f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotatedModel2 = glm::translate(rotatedModel2, glm::vec3(0.15f, -0.405f, -0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rotatedModel2));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh(); meshList[2]->RenderMesh(); meshList[3]->RenderMesh();
		meshList[4]->RenderMesh(); meshList[5]->RenderMesh(); meshList[6]->RenderMesh();
		meshList[7]->RenderMesh(); meshList[8]->RenderMesh(); meshList[9]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		