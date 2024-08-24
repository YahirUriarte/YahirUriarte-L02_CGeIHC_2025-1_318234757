//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas

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

//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"

const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

static const char* vshader_rojo = "shaders/shader_rojo.vert";
static const char* fshader_rojo = "shaders/shader_rojo.frag";
static const char* vshader_azul = "shaders/shader_azul.vert";
static const char* fshader_azul = "shaders/shader_azul.frag";
static const char* vshader_verde01 = "shaders/shader_verde01.vert";
static const char* fshader_verde01 = "shaders/shader_verde01.frag";
static const char* vshader_verde02 = "shaders/shader_verde02.vert";
static const char* fshader_verde02 = "shaders/shader_verde02.frag";
static const char* vshader_cafe = "shaders/shader_cafe.vert";
static const char* fshader_cafe = "shaders/shader_cafe.frag";

float angulo = 0.0f;

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
	meshList.push_back(casa);
}

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
			-0.15f, -0.5f,  0.6f,  0.15f, -0.5f, 0.6f,
			 0.15f, -0.2f,  0.6f, -0.15f, -0.2f, 0.6f,
		// Uniones traseras
			-0.15f, -0.5f,  0.3f,  0.15f, -0.5f,  0.3f,
			 0.15f, -0.2f,  0.3f, -0.15f, -0.2f,  0.3f };
	Mesh* puerta = new Mesh();
	puerta->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(puerta);
}

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
			-0.4f, 0.1f, 0.6f, -0.1f, 0.1f, 0.6f,
			-0.1f, 0.4f, 0.6f, -0.4f, 0.4f, 0.6f,
		// Uniones traseras
			-0.4f, 0.1f, 0.3f, -0.1f, 0.1f, 0.3f,
			-0.1f, 0.4f, 0.3f, -0.4f, 0.4f, 0.3f };
	Mesh* ventana1 = new Mesh();
	ventana1->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana1);
}

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
			0.4f, 0.1f, 0.6f, 0.1f, 0.1f, 0.6f,
			0.1f, 0.4f, 0.6f, 0.4f, 0.4f, 0.6f,
		// Uniones traseras
			0.4f, 0.1f, 0.3f, 0.1f, 0.1f, 0.3f,
			0.1f, 0.4f, 0.3f, 0.4f, 0.4f, 0.3f };
	Mesh* ventana2 = new Mesh();
	ventana2->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(ventana2);
}

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
	meshList.push_back(techo);
}

void Arbol01() {
	unsigned int indices[] = {
		// Base
		0, 1, 2,	2, 3, 0,
		// Caras
		0, 4, 1,	1, 4, 2,	2, 4, 3,	3, 4, 0 };
	GLfloat vertices[] = {
		// Base
		 1.0f,-0.3f, 0.3f,  // 0
		 0.6f,-0.3f, 0.3f,  // 1
		 0.6f,-0.3f,-0.3f,  // 2
		 1.0f,-0.3f,-0.3f,  // 3
		// Altura
		 0.8f, 0.3f,  0.0f   // 4
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
		-1.0f,-0.3f, 0.3f,  // 0
		-0.6f,-0.3f, 0.3f,  // 1
		-0.6f,-0.3f,-0.3f,  // 2
		-1.0f,-0.3f,-0.3f,  // 3
		 // Altura
		 -0.8f, 0.3f,  0.0f   // 4
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

void CrearLetras(){
	GLfloat vertices_letra_U[] = {//Letra U
		//	  X		Y	  Z		 R	   G	 B
			 0.1f,-0.2f, 0.1f,  0.0f, 0.0f,	1.0f,
			 0.1f, 0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			 0.3f,-0.2f, 0.1f,	0.0f, 0.0f,	1.0f,
			 0.3f,-0.2f, 0.1f,  0.0f, 0.0f,	1.0f,
			 0.3f, 0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			 0.1f, 0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			-0.1f,-0.2f, 0.1f,  0.0f, 0.0f,	1.0f,
			-0.1f, 0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			-0.3f,-0.2f, 0.1f,	0.0f, 0.0f,	1.0f,
			-0.3f,-0.2f, 0.1f,  0.0f, 0.0f,	1.0f,
			-0.3f, 0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			-0.1f, 0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			-0.3f,-0.2f, 0.1f,  0.0f, 0.0f,	1.0f,
			-0.3f,-0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			 0.3f,-0.2f, 0.1f,	0.0f, 0.0f,	1.0f,
			-0.3f,-0.4f, 0.1f,  0.0f, 0.0f,	1.0f,
			 0.3f,-0.4f, 0.1f,	0.0f, 0.0f,	1.0f,
			 0.3f,-0.2f, 0.1f,	0.0f, 0.0f,	1.0f};
	MeshColor* letra_U = new MeshColor();
	letra_U->CreateMeshColor(vertices_letra_U, 108);
	meshColorList.push_back(letra_U);

	GLfloat vertices_letra_O[] = {//Letra O
		//	  X		Y	  Z		 R	   G	 B
			 0.1f,-0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			 0.1f, 0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.3f, 0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.3f,-0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			 0.3f, 0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.1f,-0.2f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.1f,-0.4f, 0.1f,  0.0f, 1.0f,	0.0f,
			-0.1f, 0.2f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.3f,-0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.3f, 0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			-0.3f,-0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.1f, 0.2f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.1f, 0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			 0.1f, 0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.3f, 0.2f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.3f, 0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			-0.3f, 0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.1f, 0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.1f,-0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			-0.1f,-0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.3f,-0.2f, 0.1f,	0.0f, 1.0f,	0.0f,
			 0.3f,-0.2f, 0.1f,  0.0f, 1.0f,	0.0f,
			 0.3f,-0.4f, 0.1f,	0.0f, 1.0f,	0.0f,
			-0.1f,-0.4f, 0.1f,	0.0f, 1.0f,	0.0f};
	MeshColor* letra_O = new MeshColor();
	letra_O->CreateMeshColor(vertices_letra_O, 144);
	meshColorList.push_back(letra_O);

	GLfloat vertices_letra_E[] = {//Letra E
		//	  X		Y	  Z		 R	   G	 B
			-0.3f,-0.4f, 0.1f,  1.0f, 0.0f,	0.0f,
			-0.3f, 0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f, 0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f,-0.4f, 0.1f,  1.0f, 0.0f,	0.0f,
			-0.1f, 0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.3f,-0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f, 0.2f, 0.1f,  1.0f, 0.0f,	0.0f,
			-0.1f, 0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			 0.3f, 0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f, 0.2f, 0.1f,  1.0f, 0.0f,	0.0f,
			 0.3f, 0.2f, 0.1f,	1.0f, 0.0f,	0.0f,
			 0.3f, 0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f,-0.2f, 0.1f,  1.0f, 0.0f,	0.0f,
			-0.1f,-0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			 0.3f,-0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f,-0.2f, 0.1f,  1.0f, 0.0f,	0.0f,
			 0.3f,-0.2f, 0.1f,	1.0f, 0.0f,	0.0f,
			 0.3f,-0.4f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f,-0.1f, 0.1f,  1.0f, 0.0f,	0.0f,
			 0.3f,-0.1f, 0.1f,	1.0f, 0.0f,	0.0f,
			 0.3f, 0.1f, 0.1f,	1.0f, 0.0f,	0.0f,
			-0.1f, 0.1f, 0.1f,  1.0f, 0.0f,	0.0f,
			-0.1f,-0.1f, 0.1f,	1.0f, 0.0f,	0.0f,
			 0.3f, 0.1f, 0.1f,	1.0f, 0.0f,	0.0f};
	MeshColor* letra_E = new MeshColor();
	letra_E->CreateMeshColor(vertices_letra_E, 144);
	meshColorList.push_back(letra_E);

	GLfloat vertices_letra_Y[] = {//Letra Y
		//	  X		Y	  Z		 R	   G	 B
			 0.1f, 0.0f, 0.1f,  0.6f, 0.0f,	0.6f,
			-0.1f, 0.0f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.1f,-0.4f, 0.1f,	0.6f, 0.0f,	0.6f,
			-0.1f,-0.4f, 0.1f,  0.6f, 0.0f,	0.6f,
			-0.1f, 0.0f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.1f,-0.4f, 0.1f,	0.6f, 0.0f,	0.6f,
			-0.1f, 0.0f, 0.1f,  0.6f, 0.0f,	0.6f,
			 0.1f, 0.0f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.0f, 0.1f, 0.1f,	0.6f, 0.0f,	0.6f,

			-0.1f, 0.0f, 0.1f,  0.6f, 0.0f,	0.6f,
			 0.0f, 0.1f, 0.1f,	0.6f, 0.0f,	0.6f,
			-0.4f, 0.3f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.0f, 0.1f, 0.1f,  0.6f, 0.0f,	0.6f,
			-0.3f, 0.4f, 0.1f,	0.6f, 0.0f,	0.6f,
			-0.4f, 0.3f, 0.1f,	0.6f, 0.0f,	0.6f,

			 0.1f, 0.0f, 0.1f,  0.6f, 0.0f,	0.6f,
			 0.0f, 0.1f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.4f, 0.3f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.0f, 0.1f, 0.1f,  0.6f, 0.0f,	0.6f,
			 0.3f, 0.4f, 0.1f,	0.6f, 0.0f,	0.6f,
			 0.4f, 0.3f, 0.1f,	0.6f, 0.0f,	0.6f};
	MeshColor* letra_Y = new MeshColor();
	letra_Y->CreateMeshColor(vertices_letra_Y, 126);
	meshColorList.push_back(letra_Y);
}

void CreateShaders(){
	Shader *shader1 = new Shader(); //Color como parte del VAO: letras
	shader1->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//Color rojo. 
	shader2->CreateFromFiles(vshader_rojo, fshader_rojo);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader();//Color azul. 
	shader3->CreateFromFiles(vshader_azul, fshader_azul);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader();//Color verde claro. 
	shader4->CreateFromFiles(vshader_verde01, fshader_verde01);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader();//Color verde fuerte.
	shader5->CreateFromFiles(vshader_verde02, fshader_verde02);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader();//Color cafe.
	shader6->CreateFromFiles(vshader_cafe, fshader_cafe);
	shaderList.push_back(*shader6);
}

int main(){
	mainWindow = Window(900, 900);
	mainWindow.Initialise();

	Casa();
	Puerta();
	Ventana01();
	Ventana02();
	Techo();
	Arbol01();
	Arbol02();
	Tronco01();
	Tronco02();

	CrearLetras();
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//--Actividad 1 de la Practica 02 --
		//shaderList[0].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();

		//model = glm::mat4(1.0);// Ubicacion de Letra U
		//model = glm::translate(model, glm::vec3(-0.5f, 0.5f, -4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();

		//model = glm::mat4(1.0);// Ubicacion de Letra O
		//model = glm::translate(model, glm::vec3(0.5f, 0.5f, -4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[1]->RenderMeshColor();

		//model = glm::mat4(1.0);// Ubicacion de Letra E
		//model = glm::translate(model, glm::vec3(-0.5f,-0.5f, -4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[2]->RenderMeshColor();

		//model = glm::mat4(1.0);// Ubicacion de Letra Y
		//model = glm::translate(model, glm::vec3(0.5f,-0.5f, -4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[3]->RenderMeshColor();

		//--Actividad 2 de la Practica 02 --
		// Casa
		shaderList[1].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Puerta
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Ventana 1 y 2
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		// Techo
		shaderList[2].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[4]->RenderMesh();

		// Arbol 1 y 2
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[5]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[6]->RenderMesh();

		// Tronco 1 y 2
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[7]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));//Rotar la figura.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[8]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}