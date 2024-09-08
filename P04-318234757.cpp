//Práctica 4: Modelado Jerárquico.

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
Sphere sp = Sphere(1.0, 20, 20);

void CrearCubo() {
	unsigned int cubo_indices[] = {
		0, 1, 2, 2, 3, 0,	// front
		1, 5, 6, 6, 2, 1,	// right
		7, 6, 5, 5, 4, 7,	// back
		4, 0, 3, 3, 7, 4,	// left
		4, 5, 1, 1, 0, 4,	// bottom
		3, 2, 6, 6, 7, 3 };	// top
	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,	0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,	0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,		-0.5f,  0.5f, -0.5f };
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}
void CrearCilindro(int res, float R){
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	for (n = 0; n <= (res); n++){
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);}
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);//Se generan los indices de los vértices
	Mesh *cilindro = new Mesh();//se genera el mesh del cilindro
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);}
void CrearCono(int res,float R){
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;
	vertices.push_back(0.0);vertices.push_back(0.5);vertices.push_back(0.0);
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);
	for (i = 0; i < res+2; i++) indices.push_back(i);
	Mesh *cono = new Mesh();//se genera el mesh del cono
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);}
void CrearPiramideCuadrangular(){
	vector<unsigned int> piramidecuadrangular_indices={0,3,4,	3,2,4,	2,1,4,	1,0,4,	0,1,2,	0,2,4};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,	0.5f,-0.5f,-0.5f,	-0.5f,-0.5f,-0.5f,	-0.5f,-0.5f,0.5f,	0.0f,0.5f,0.0f,};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);}
void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);}

int main(){
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	
	CrearCubo();
	CrearPiramideCuadrangular();
	CrearCilindro(50, 1.0f);
	CrearCono(25, 2.0f);
	CreateShaders();
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection=0;		GLuint uniformModel=0;
	GLuint uniformView=0;			GLuint uniformColor=0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	sp.init();sp.load();

	glm::mat4 model(1.0);		//Origen de las jererquias.
	glm::mat4 modelaux(1.0);	//Jerarquia con la base de la grua y primera llanta.
	glm::mat4 modelaux2(1.0);	//Jerarquia con la segunda llanta.
	glm::mat4 modelaux3(1.0);	//Jerarquia con la tercer llanta.
	glm::mat4 modelaux4(1.0);	//Jerarquia con la cuarta llanta.
	glm::mat4 modelaux5(1.0);	//Jerarquia con los brazos.
	glm::mat4 modelaux6(1.0);
	
	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f);

	while (!mainWindow.getShouldClose()) {

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

		// ACTIVIDAD 1. Terminar la Grúa.	---		---		---		---		---		---		---		---		---

		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, -4.0f));
		modelaux = model;  //Jerarquia con la base de la grua
		modelaux5 = model; //Jerarquia con los brazos.

		//Cuerpo (prisma rectangular).
		model = glm::translate(model, glm::vec3(0.4f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux;

		//Articulación 1
		model = glm::translate(model, glm::vec3(0.4f, -0.47f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = glm::translate(model, glm::vec3(0.0f, -0.45f, 0.0f));
		modelaux = model;	//Jerarquia con la base de la grua y primera llanta.
		modelaux2 = model;	//Jerarquia con la segunda llanta.
		modelaux3 = model;	//Jerarquia con la tercer llanta.
		modelaux4 = model;	//Jerarquia con la cuarta llanta.

		//Base (pirámide cuadrangular).
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -0.1f));
		model = glm::scale(model, glm::vec3(6.9f, 2.6f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMeshGeometry();

		model = modelaux;

		//Articulación Llanta 01.
		model = glm::translate(model, glm::vec3(2.0f, -2.3f, 2.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		modelaux = model;

		//Llanta 01.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux2;

		//Articulación Llanta 02.
		model = glm::translate(model, glm::vec3(-2.0f, -2.3f, 2.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		modelaux2 = model;

		//Llanta 02.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux3;

		//Articulación Llanta 03.
		model = glm::translate(model, glm::vec3(-2.0f, -2.3f, -2.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		modelaux3 = model;

		//Llanta 03.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux4;

		//Articulación Llanta 04
		model = glm::translate(model, glm::vec3(2.0f, -2.3f, -2.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		modelaux4 = model;

		//Llanta 04.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//Instrucciones de articulaciones y brazos desarrollado en el ejercicio.

		model = modelaux5;

		//Articulación 1.
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		//Brazo N.1.
		model = glm::translate(model, glm::vec3(-2.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		model = modelaux5;

		//Articulación 2.
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux5;

		//Brazo N.2.
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		model = modelaux5;

		//Articulación 3.
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux5;

		//Brazo N.3.
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		model = modelaux5;

		//Articulación 4.
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(-1.0f, 1.0f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux5;

		//Cabina.
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();*/

		// ACTIVIDAD 2. Crear un animal robot 3d	---		---		---		---		---		---		---		---

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -20.0f));
		modelaux = model;   //Jerarquia con la primera pierna.
		modelaux2 = model;  //Jerarquia con la segunda pierna.
		modelaux3 = model;	//Jerarquia con la tercera pierna.
		modelaux4 = model;	//Jerarquia con la cuarta pierna.
		modelaux5 = model;	//Jerarquia con la primer oreja.
		modelaux6 = model;	//Jerarquia con la segunda oreja.

		//Cuerpo.
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		modelaux = model;
		modelaux2 = model;  //Jerarquia con la segunda pierna.
		modelaux3 = model;	//Jerarquia con la tercera pierna.
		modelaux4 = model;	//Jerarquia con la cuarta pierna.
		model = glm::scale(model, glm::vec3(4.0f, 6.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.55f, 0.55f, 0.55f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux;

		//Cabeza.
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
		modelaux5 = model;	//Jerarquia con la primer oreja.
		modelaux6 = model;	//Jerarquia con la segunda oreja.
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//Visor.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//Tapa superior.
		model = glm::translate(model, glm::vec3(0.0f, 1.6f, -3.3f));
		model = glm::scale(model, glm::vec3(1.25f, 1.5f, 3.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;

		//Articulación 1 pierna 1.
		model = glm::translate(model, glm::vec3(2.8f, -1.0f, 2.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux;

		//Pierna 1, primer parte.
		model = glm::translate(model, glm::vec3(1.8f,2.5f,1.8f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux;

		//Articulación 2 pierna 1.
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux;

		//Pierna 1, segunda parte.
		model = glm::translate(model, glm::vec3(2.1f, -1.8f, 2.1f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux;

		//Articulación 3 pierna 1.
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 1.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux;

		//Pie 1.
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 3.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();
		
		model = modelaux2;

		//Articulación 1 pierna 2.
		model = glm::translate(model, glm::vec3(-2.8f, -1.0f, 2.8f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux2;

		//Pierna 2, primer parte.
		model = glm::translate(model, glm::vec3(1.8f, 2.5f, 1.8f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux2;

		//Articulación 2 pierna 2.
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux2;

		//Pierna 2, segunda parte.
		model = glm::translate(model, glm::vec3(2.1f, -1.8f, 2.1f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux2;

		//Articulación 3 pierna 2.
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, -1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux2;

		//Pie 2.
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 3.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		model = modelaux3;

		//Articulación 1 pierna 3.
		model = glm::translate(model, glm::vec3(2.8f, -1.0f, -2.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(1.0f, 0.0f, 1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux3;

		//Pierna 3, primer parte.
		model = glm::translate(model, glm::vec3(1.8f, 2.5f, 1.8f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux3;

		//Articulación 2 pierna 3.
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux3;

		//Pierna 3, segunda parte.
		model = glm::translate(model, glm::vec3(2.1f, -1.8f, 2.1f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux3;

		//Articulación 3 pierna 3.
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 1.0f, -1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux3;

		//Pie 3.
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.8f, 3.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		model = modelaux4;

		//Articulación 1 pierna 4.
		model = glm::translate(model, glm::vec3(-2.8f, -1.0f, -2.8f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux4;

		//Pierna 4, primer parte.
		model = glm::translate(model, glm::vec3(1.8f, 2.5f, 1.8f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux4;

		//Articulación 2 pierna 4.
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux4;

		//Pierna 4, segunda parte.
		model = glm::translate(model, glm::vec3(2.1f, -1.8f, 2.1f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.5f, 6.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		model = modelaux4;

		//Articulación 3 pierna 4.
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(0.0f, 1.0f, -1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux4;

		//Pie 4.
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.8f, 3.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		model = modelaux5;

		//Articulacion Oreja 1
		model = glm::translate(model, glm::vec3(2.2f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion13()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux5;

		//Oreja 1
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, -0.8f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.2f,2.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.8f, 0.8f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		model = modelaux6;

		//Articulacion Oreja 2
		model = glm::translate(model, glm::vec3(-2.2f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion14()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux5;

		//Oreja 2
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, -0.8f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux5 = model;
		model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.8f, 0.8f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		