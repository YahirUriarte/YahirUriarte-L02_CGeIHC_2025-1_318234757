//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>

//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"

//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z

using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";

static const char* vshader_rojo = "shaders/shader_rojo.vert";
static const char* fshader_rojo = "shaders/shader_rojo.frag";
static const char* vshader_azul = "shaders/shader_azul.vert";
static const char* fshader_azul = "shaders/shader_azul.frag";
static const char* vshader_verde01 = "shaders/shader_verde01.vert";
static const char* fshader_verde01 = "shaders/shader_verde01.frag";
static const char* vshader_verde02 = "shaders/shader_verde02.vert";
static const char* fshader_verde02 = "shaders/shader_verde02.frag";
static const char* vshader_verde03 = "shaders/shader_verde03.vert";
static const char* fshader_verde03 = "shaders/shader_verde03.frag";
static const char* vshader_cafe = "shaders/shader_cafe.vert";
static const char* fshader_cafe = "shaders/shader_cafe.frag";

Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

//Codigo de la practica anterior
void Casa() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			-0.5f, -0.5f,  0.5f,  0.5f, -0.5f, 0.5f,
			 0.5f,  0.5f,  0.5f, -0.5f,  0.5f, 0.5f,
		// Uniones traseras
			-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f };
	Mesh* casa = new Mesh();
	casa->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(casa);}
void Techo() {
	unsigned int indices[] = {
		// Base
		0, 1, 2,	2, 3, 0,
		// Caras
		0, 4, 1,	1, 4, 2,	2, 4, 3,	3, 4, 0 };
	GLfloat vertices[] = {
		// Base
		-0.5f, 0.5f,  0.5f,  // 0
		 0.5f, 0.5f,  0.5f,  // 1
		 0.5f, 0.5f, -0.5f,  // 2
		-0.5f, 0.5f, -0.5f,  // 3
		// Altura
		 0.0f, 1.0f,  0.0f   // 4
	};
	Mesh* techo = new Mesh();
	techo->CreateMesh(vertices, indices, 15, 18);
	meshList.push_back(techo);}
void Puerta() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			-0.15f, -0.5f,  0.55f,  0.15f, -0.5f, 0.55f,
			 0.15f, -0.2f,  0.55f, -0.15f, -0.2f, 0.55f,
		// Uniones traseras
		    -0.15f, -0.5f,  0.3f,  0.15f, -0.5f,  0.3f,
			 0.15f, -0.2f,  0.3f, -0.15f, -0.2f,  0.3f };
	Mesh* puerta = new Mesh();
	puerta->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(puerta);}
void Ventana01() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			-0.4f, 0.1f, 0.55f, -0.1f, 0.1f, 0.55f,
			-0.1f, 0.4f, 0.55f, -0.4f, 0.4f, 0.55f,
		// Uniones traseras
			-0.4f, 0.1f, 0.3f, -0.1f, 0.1f, 0.3f,
			-0.1f, 0.4f, 0.3f, -0.4f, 0.4f, 0.3f };
	Mesh* ventana1 = new Mesh();
	ventana1->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana1);}
void Ventana02() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			0.4f, 0.1f, 0.55f, 0.1f, 0.1f, 0.55f,
			0.1f, 0.4f, 0.55f, 0.4f, 0.4f, 0.55f,
		// Uniones traseras
			0.4f, 0.1f, 0.3f, 0.1f, 0.1f, 0.3f,
			0.1f, 0.4f, 0.3f, 0.4f, 0.4f, 0.3f };
	Mesh* ventana2 = new Mesh();
	ventana2->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana2);}
void Arbol01() {
	unsigned int indices[] = {
		// Base
		0, 1, 2,	2, 3, 0,
		// Caras
		0, 4, 1,	1, 4, 2,	2, 4, 3,	3, 4, 0 };
	GLfloat vertices[] = {
		// Base
		 1.0f,-0.3f, 0.25f,  // 0
		 0.6f,-0.3f, 0.25f,  // 1
		 0.6f,-0.3f,-0.25f,  // 2
		 1.0f,-0.3f,-0.25f,  // 3
		 // Altura
		  0.8f, 0.2f,  0.0f   // 4
	};
	Mesh* arbol01 = new Mesh();
	arbol01->CreateMesh(vertices, indices, 15, 18);
	meshList.push_back(arbol01);
}
void Arbol02() {
	unsigned int indices[] = {
		// Base
		0, 1, 2,	2, 3, 0,
		// Caras
		0, 4, 1,	1, 4, 2,	2, 4, 3,	3, 4, 0 };
	GLfloat vertices[] = {
		// Base
		-1.0f,-0.3f, 0.25f,  // 0
		-0.6f,-0.3f, 0.25f,  // 1
		-0.6f,-0.3f,-0.25f,  // 2
		-1.0f,-0.3f,-0.25f,  // 3
		// Altura
		-0.8f, 0.2f,  0.0f   // 4
	};
	Mesh* arbol02 = new Mesh();
	arbol02->CreateMesh(vertices, indices, 15, 18);
	meshList.push_back(arbol02);
}
void Tronco01() {//Cubo.
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
			  // Cara Izquierda	Cara Derecha
					1, 5, 6,		   4, 0, 3,
					6, 2, 1,		   3, 7, 4,
					// Tapa Superior	Tapa Infetior
						  4, 5, 1,		   3, 2, 6,
						 1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			-0.9f, -0.5f,  0.1f, -0.7f, -0.5f,  0.1f,
			-0.7f, -0.3f,  0.1f, -0.9f, -0.3f,  0.1f,
			// Uniones traseras
				-0.9f, -0.5f, -0.1f, -0.7f, -0.5f, -0.1f,
				-0.7f, -0.3f, -0.1f, -0.9f, -0.3f, -0.1f };
	Mesh* tronco01 = new Mesh();
	tronco01->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(tronco01);
}
void Tronco02() {//Cubo.
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
			  // Cara Izquierda	Cara Derecha
					1, 5, 6,		   4, 0, 3,
					6, 2, 1,		   3, 7, 4,
					// Tapa Superior	Tapa Infetior
						  4, 5, 1,		   3, 2, 6,
						  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			 0.9f, -0.5f,  0.1f,  0.7f, -0.5f,  0.1f,
			 0.7f, -0.3f,  0.1f,  0.9f, -0.3f,  0.1f,
			 // Uniones traseras
				  0.9f, -0.5f, -0.1f,  0.7f, -0.5f, -0.1f,
				  0.7f, -0.3f, -0.1f,  0.9f, -0.3f, -0.1f };
	Mesh* tronco02 = new Mesh();
	tronco02->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(tronco02);
}

//Codigo nuevo para los elementos agregados
void Ventana_Lateral01() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			0.55f, 0.1f, 0.4f, 0.25f, 0.1f, 0.4f,
			0.25f, 0.4f, 0.4f, 0.55f, 0.4f, 0.4f,
		// Uniones traseras
			0.55f, 0.1f, 0.1f, 0.25f, 0.1f, 0.1f,
			0.25f, 0.4f, 0.1f, 0.55f, 0.4f, 0.1f };
	Mesh* ventana_l01 = new Mesh();
	ventana_l01->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana_l01);}

void Ventana_Lateral02() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			0.55f, 0.1f, -0.4f, 0.25f, 0.1f, -0.4f,
			0.25f, 0.4f, -0.4f, 0.55f, 0.4f, -0.4f,
		// Uniones traseras
			0.55f, 0.1f, -0.1f, 0.25f, 0.1f, -0.1f,
			0.25f, 0.4f, -0.1f, 0.55f, 0.4f, -0.1f };
	Mesh* ventana_l02 = new Mesh();
	ventana_l02->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana_l02);}

void Ventana_Lateral03() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			-0.55f, 0.1f, 0.4f, -0.25f, 0.1f, 0.4f,
			-0.25f, 0.4f, 0.4f, -0.55f, 0.4f, 0.4f,
		// Uniones traseras
			-0.55f, 0.1f, 0.1f, -0.25f, 0.1f, 0.1f,
			-0.25f, 0.4f, 0.1f, -0.55f, 0.4f, 0.1f };
	Mesh* ventana_l03 = new Mesh();
	ventana_l03->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana_l03);}

void Ventana_Lateral04() {
	unsigned int cubo_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat cubo_vertices[] = {
		// Uniones frontales
			-0.55f, 0.1f, -0.4f, -0.25f, 0.1f, -0.4f,
			-0.25f, 0.4f, -0.4f, -0.55f, 0.4f, -0.4f,
		// Uniones traseras
			-0.55f, 0.1f, -0.1f, -0.25f, 0.1f, -0.1f,
			-0.25f, 0.4f, -0.1f, -0.55f, 0.4f, -0.1f };
	Mesh* ventana_l04 = new Mesh();
	ventana_l04->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana_l04);}

void CrearCilindro(int res, float R, float altura, float posX, float posY, float posZ) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, y, z = -altura / 2.0f + posZ;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	for (n = 0; n <= res; n++) {
		if (n != res) { x = R * cos(n * dt) + posX;
						y = R * sin(n * dt) + posY;}
		else{ x = R * cos(0) + posX;
			  y = R * sin(0) + posY;}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: vertices.push_back(z); break;
			case 3: vertices.push_back(x); break;
			case 4: vertices.push_back(y); break;
			case 5: vertices.push_back(z + altura); break;}
		}
	}

	for (n = 0; n <= res; n++) {
		x = R * cos(n * dt) + posX;
		y = R * sin(n * dt) + posY;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);}

	for (n = 0; n <= res; n++) {
		x = R * cos(n * dt) + posX;
		y = R * sin(n * dt) + posY;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z + altura);}

	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);}

void Terreno() {
	unsigned int terreno_indices[] = {
		//  Cara Frontal	Cara Trasera
			  0, 1, 2,		   7, 6, 5,
			  2, 3, 0,		   5, 4, 7,
		// Cara Izquierda	Cara Derecha
			  1, 5, 6,		   4, 0, 3,
			  6, 2, 1,		   3, 7, 4,
		// Tapa Superior	Tapa Infetior
			  4, 5, 1,		   3, 2, 6,
			  1, 0, 4,		   6, 7, 3 };
	GLfloat terreno_vertices[] = {
		// Uniones frontales
			-2.0f, -0.5f,  2.0f,  2.0f, -0.5f, 2.0f,
			 2.0f, -0.55f,  2.0f, -2.0f, -0.55f, 2.0f,
		// Uniones traseras
			-2.0f, -0.5f, -2.0f,  2.0f, -0.5f, -2.0f,
			 2.0f, -0.55f, -2.0f, -2.0f, -0.55f, -2.0f };
	Mesh* terreno = new Mesh();
	terreno->CreateMesh(terreno_vertices, terreno_indices, 24, 36);
	meshList.push_back(terreno);}





void CreateShaders(){
	Shader* shader1 = new Shader();//Color rojo. 
	shader1->CreateFromFiles(vshader_rojo, fshader_rojo);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//Color azul. 
	shader2->CreateFromFiles(vshader_azul, fshader_azul);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader();//Color verde claro. 
	shader3->CreateFromFiles(vshader_verde01, fshader_verde01);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader();//Color verde fuerte.
	shader4->CreateFromFiles(vshader_verde02, fshader_verde02);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader();//Color cafe.
	shader5->CreateFromFiles(vshader_cafe, fshader_cafe);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader();//Color verde fuerte de terreno.
	shader6->CreateFromFiles(vshader_verde03, fshader_verde03);
	shaderList.push_back(*shader6);
}

int main(){
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	Casa();
	Puerta();
	Ventana01();
	Ventana02();
	Ventana_Lateral01();
	Ventana_Lateral02();
	Ventana_Lateral03();
	Ventana_Lateral04();
	Techo();
	Arbol01();
	Arbol02();
	Tronco01();
	Tronco02();
	CrearCilindro(70, 0.2f,0.1f,0.0f,0.0f,-0.5f);
	Terreno();

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	sp.init();
	sp.load();

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Cubo Rojo.
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//Cubos de puerta y ventanas.
		shaderList[2].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);// Puerta
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();
		model = glm::mat4(1.0);// Ventana 01
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();
		model = glm::mat4(1.0);// Ventana 02
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0);// Ventana 01L
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		model = glm::mat4(1.0);// Ventana 02L
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);// Ventana 03L
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[6]->RenderMesh();
		model = glm::mat4(1.0);// Ventana 04L
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[7]->RenderMesh();

		//Techo.
		shaderList[1].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[8]->RenderMesh();

		//Arboles.
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);//Arbol 01
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[9]->RenderMesh();
		model = glm::mat4(1.0);//Arbol 02
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[10]->RenderMesh();

		//Troncos.
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);//Tronco 01
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[11]->RenderMesh();
		model = glm::mat4(1.0);//Tronco 02
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[12]->RenderMesh();

		//Ventana trasera
		shaderList[1].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[13]->RenderMeshGeometry();

		//Terreno
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[14]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		