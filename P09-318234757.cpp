//Practica 9. Animacion Basica		Uriarte Ortiz Enrique Yahir
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

float movDado;
float movOffsetDado;
float RotacionX, RotacionZ;
bool DadoRotando, DadoPiso;

float movX,movZ, giro;
float movOffsetHeli;
float V_Rot_Helice;
bool C1, C2, C3, C4;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;
Texture Casilla1, Casilla2, Casilla3, Casilla4, Csl2, Csl3;
Texture A1, A9, B1, B4, C7, C9, D1, D9;

Model Dado;

Model Helicoptero;
Model Helice;
Model Helice_T;

Skybox skybox;

Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

DirectionalLight mainLight;

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";
 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,unsigned int vLength, unsigned int normalOffset){
	for (size_t i = 0; i < indiceCount; i += 3){
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticeCount / vLength; i++){
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects(){
	unsigned int indices[] = {0,3,1,	1,3,2,	2,3,0,	0,1,2};
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f};

	unsigned int floorIndices[] = {0,2,1,	1,2,3};
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f};

	unsigned int vegetacionIndices[] = {0,1,2,	0,2,3,	4,5,6,	4,6,7};
	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);}

void Casillas_Esquina() {
	unsigned int Casillas1_indices[] = {
		0,1,2, 0,2,3,	//Up
		4,5,6, 4,6,7,	//Down
		8,9,11, 8,10,11,	//Lateral_01
		12,13,15, 12,14,15,	//Lateral_02
		16,17,18, 17,18,19,	//Lateral_03
		20,21,22, 21,22,23,	//Lateral_04
	};
	GLfloat Casillas1_vertices[] = {
		//Up
		1.0f, 0.2f, 1.0f, 0.00f, 1.0f, 0.0f,0.0f,0.0f,	//A - 0
		1.0f, 0.2f,-1.0f, 0.00f, 0.0f, 0.0f,0.0f,0.0f,	//B - 1
	   -1.0f, 0.2f,-1.0f, 0.79f,0.0f, 0.0f,0.0f,0.0f,	//C - 2
	   -1.0f, 0.2f, 1.0f, 0.79f,1.0f, 0.0f,0.0f,0.0f,	//D - 3

	   //Down
		1.0f, 0.0f, 1.0f, 0.79f,1.0f, 0.0f,0.0f,0.0f,	//E - 4
		1.0f, 0.0f,-1.0f, 0.79f,0.0f, 0.0f,0.0f,0.0f,	//F - 5
	   -1.0f, 0.0f,-1.0f, 0.99f,0.0f, 0.0f,0.0f,0.0f,	//G - 6
	   -1.0f, 0.0f, 1.0f, 0.99f,1.0f, 0.0f,0.0f,0.0f,	//H - 7

	   //Lateral_01
		1.0f, 0.2f, 1.0f, 0.795f,1.0f, 0.0f,0.0f,0.0f,	//A - 8
		1.0f, 0.2f,-1.0f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//B - 9
		1.0f, 0.0f, 1.0f, 0.795f,0.0f, 0.0f,0.0f,0.0f,	//E - 10
		1.0f, 0.0f,-1.0f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//F - 11

	   //Lateral_02
		1.0f, 0.2f,-1.0f, 0.795f,1.0f, 0.0f,0.0f,0.0f,	//B - 12
	   -1.0f, 0.2f,-1.0f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//C - 13
		1.0f, 0.0f,-1.0f, 0.795f,0.0f, 0.0f,0.0f,0.0f,	//F - 14
	   -1.0f, 0.0f,-1.0f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//G - 15

	   //Lateral_03
	    1.0f, 0.2f, 1.0f, 0.795f,1.0f, 0.0f,0.0f,0.0f,	//A - 16
	   -1.0f, 0.2f, 1.0f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//D - 17
		1.0f, 0.0f, 1.0f, 0.795f,0.0f, 0.0f,0.0f,0.0f,	//E - 18
	   -1.0f, 0.0f, 1.0f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//H - 19

	   //Lateral_04
	   -1.0f, 0.2f,-1.0f, 0.795f,1.0f, 0.0f,0.0f,0.0f,	//C - 20
	   -1.0f, 0.2f, 1.0f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//D - 21
	   -1.0f, 0.0f,-1.0f, 0.795f,0.0f, 0.0f,0.0f,0.0f,	//G - 22
	   -1.0f, 0.0f, 1.0f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//H - 23

	};
	Mesh* Casillas1 = new Mesh();
	Casillas1->CreateMesh(Casillas1_vertices, Casillas1_indices, 192, 36);
	meshList.push_back(Casillas1);
}

void Casillas_Inter() {
	unsigned int Casillas2_indices[] = {
		0,1,2, 0,2,3,	//Up
		4,5,6, 4,6,7,	//Down
		8,9,11, 8,10,11,	//Lateral_01
		12,13,15, 12,14,15,	//Lateral_02
		16,17,18, 17,18,19,	//Lateral_03
		20,21,22, 21,22,23,	//Lateral_04
		8,9,11, 8,10,11,	//Lateral_01
		12,13,15, 12,14,15,	//Lateral_02
		16,17,18, 17,18,19,	//Lateral_03
		20,21,22, 21,22,23,	//Lateral_04
	};
	GLfloat Casillas2_vertices[] = {
		//Up
		1.0f, 0.2f, 0.5f, 0.00f,0.0f, 0.0f,0.0f,0.0f,	//A - 0
		1.0f, 0.2f,-0.5f, 0.72f,0.0f, 0.0f,0.0f,0.0f,	//B - 1
	   -1.0f, 0.2f,-0.5f, 0.72f,1.0f, 0.0f,0.0f,0.0f,	//C - 2
	   -1.0f, 0.2f, 0.5f, 0.00f,1.0f, 0.0f,0.0f,0.0f,	//D - 3

	   //Down
		1.0f, 0.0f, 0.5f, 0.75f,1.0f, 0.0f,0.0f,0.0f,	//E - 4
		1.0f, 0.0f,-0.5f, 0.75f,0.0f, 0.0f,0.0f,0.0f,	//F - 5
	   -1.0f, 0.0f,-0.5f, 0.99f,0.0f, 0.0f,0.0f,0.0f,	//G - 6
	   -1.0f, 0.0f, 0.5f, 0.99f,1.0f, 0.0f,0.0f,0.0f,	//H - 7

	   //Lateral_01
		1.0f, 0.2f, 0.5f, 0.75f,1.0f, 0.0f,0.0f,0.0f,	//A - 8
		1.0f, 0.2f,-0.5f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//B - 9
		1.0f, 0.0f, 0.5f, 0.75f,0.0f, 0.0f,0.0f,0.0f,	//E - 10
		1.0f, 0.0f,-0.5f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//F - 11

		//Lateral_02
		 1.0f, 0.2f,-0.5f, 0.75f,1.0f, 0.0f,0.0f,0.0f,	//B - 12
		-1.0f, 0.2f,-0.5f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//C - 13
		 1.0f, 0.0f,-0.5f, 0.75f,0.0f, 0.0f,0.0f,0.0f,	//F - 14
		-1.0f, 0.0f,-0.5f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//G - 15

		//Lateral_03
		 1.0f, 0.2f, 0.5f, 0.75f,1.0f, 0.0f,0.0f,0.0f,	//A - 16
		-1.0f, 0.2f, 0.5f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//D - 17
		 1.0f, 0.0f, 0.5f, 0.75f,0.0f, 0.0f,0.0f,0.0f,	//E - 18
		-1.0f, 0.0f, 0.5f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//H - 19

		//Lateral_04
		-1.0f, 0.2f,-0.5f, 0.75f,1.0f, 0.0f,0.0f,0.0f,	//C - 20
		-1.0f, 0.2f, 0.5f, 0.985f,1.0f, 0.0f,0.0f,0.0f,	//D - 21
		-1.0f, 0.0f,-0.5f, 0.75f,0.0f, 0.0f,0.0f,0.0f,	//G - 22
		-1.0f, 0.0f, 0.5f, 0.985f,0.0f, 0.0f,0.0f,0.0f,	//H - 23
	};
	Mesh* Casillas2 = new Mesh();
	Casillas2->CreateMesh(Casillas2_vertices, Casillas2_indices, 192, 36);
	meshList.push_back(Casillas2);
}

int main(){
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	Casillas_Esquina();
	Casillas_Inter();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	Casilla1 = Texture("Textures/Casilla_1.tga");
	Casilla1.LoadTextureA();
	Casilla2 = Texture("Textures/Casilla2.tga");
	Casilla2.LoadTextureA();
	Casilla3 = Texture("Textures/Casilla3.tga");
	Casilla3.LoadTextureA();
	Csl2 = Texture("Textures/Casilla_2.tga");
	Csl2.LoadTextureA();
	Csl3 = Texture("Textures/Casilla_3.tga");
	Csl3.LoadTextureA();
	Casilla4 = Texture("Textures/Casilla_4.tga");
	Casilla4.LoadTextureA();
	A1 = Texture("Textures/A1.tga");
	A1.LoadTextureA();
	A9 = Texture("Textures/A9.tga");
	A9.LoadTextureA();
	B1 = Texture("Textures/B1.tga");
	B1.LoadTextureA();
	B4 = Texture("Textures/B4.tga");
	B4.LoadTextureA();
	C7 = Texture("Textures/C7.tga");
	C7.LoadTextureA();
	C9 = Texture("Textures/C9.tga");
	C9.LoadTextureA();
	D1 = Texture("Textures/D1.tga");
	D1.LoadTextureA();
	D9 = Texture("Textures/D9.tga");
	D9.LoadTextureA();

	Dado = Model();
	Dado.LoadModel("Models/Dado.obj");

	Helicoptero = Model();
	Helicoptero.LoadModel("Models/Cuerpo_Helicoptero.obj");
	Helice = Model();
	Helice.LoadModel("Models/Helice.obj");
	Helice_T = Model();
	Helice_T.LoadModel("Models/Helice_Trasera.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.3f, 0.3f, 0.0f, 0.0f, -1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movDado = 0.0f;
	RotacionX, RotacionZ = 0.0f;
	movOffsetDado = 0.8f;
	DadoRotando = true;	 //	Dado rotando.
	DadoPiso = false;    // Dado en el suelo.

	movX, movZ = 0.0f;
	movOffsetHeli = 0.5f;
	V_Rot_Helice, giro = 0.0f;
	C1 = true;	C2, C3, C4 = false;
	
	struct Giro {float x, z;};
	Giro Vista_Caras[10] = {
		{ -35.0f,-40.0f},/*Numero 0*/		{ 217.5f, 40.0f},/*Numero 1*/
		{  50.0f, 15.0f},/*Numero 2*/		{ 0.0f, -127.0f},/*Numero 3*/
		{ -50.0f, 15.0f},/*Numero 4*/		{ -50.0f, 195.0},/*Numero 5*/
		{ 0.0f, -307.0f},/*Numero 6*/		{ 230.0f,-15.0f},/*Numero 7*/
		{  37.5f,-40.0f},/*Numero 8*/		{-215.0f, 40.0f},/*Numero 9*/
	};

	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//-- Dado --
		if (mainWindow.gettiro() == true && DadoRotando == true) {
			movDado -= movOffsetDado * deltaTime;
			int secuencia = rand() % 10;
			Giro rotSelect = Vista_Caras[secuencia];
			RotacionX = rotSelect.x; RotacionZ = rotSelect.z;

			if (movDado < -24.0f){ DadoRotando = false; DadoPiso = true;}// Ya no rota.	
		}
		if (mainWindow.getreinicio() == true && DadoPiso == true) { movDado = 0.0f; DadoRotando = true;  DadoPiso = false; }
		// El dado esta ya esta en el piso, por lo tanto vuelve a su posicion original para volver a tirarlo.

		//-- Helicoptero --
		V_Rot_Helice += 8.0f * deltaTime;
		if (C1 == true) {
			if (movZ < 74.0f) {movZ += movOffsetHeli * deltaTime;}
			else {C1 = false; C2 = true; giro += 270.0f;}
		}
		else if (C2 == true) {
			if (movX > -75.0f) {movX -= movOffsetHeli * deltaTime;}
			else { C2 = false; C3 = true; giro += 270.0f;}
		}
		else if (C3 == true) {
			if (movZ > -1.0f) {movZ -= movOffsetHeli * deltaTime;}
			else { C3 = false; C4 = true; giro += 270.0f;}
		}
		else if (C4 == true) {
			if (movX < 0.0f) {movX += movOffsetHeli * deltaTime;}
			else {C4 = false; C1 = true; C2 = false; C3 = false; giro += 270.0f;}
		}
		
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		shaderList[0].SetDirectionalLight(&mainLight);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-27.0f, movDado + 25.0f, 27.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		if (mainWindow.gettiro() == true) {// Aplicar rotaciones aleatorias
			model = glm::rotate(model, glm::radians(RotacionZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(RotacionX), glm::vec3(1.0f, 0.0f, 0.0f));}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado.RenderModel();

		//Tablero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla1.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		A1.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		A9.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		
		//Otro Lado
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Csl2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		B1.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		B4.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Otro Lado
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Csl3.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		C7.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		C9.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Otro Lado
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla4.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		D1.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		D9.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Helicoptero
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movX + 10.f, 10.0f, movZ - 9.0f));
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helicoptero.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
		model = glm::rotate(model, V_Rot_Helice * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice.RenderModel();
		model = modelaux;

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
		model = glm::rotate(model, glm::radians(32.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, V_Rot_Helice * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, 1.9f, -20.2f));
		model = glm::rotate(model, V_Rot_Helice * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_T.RenderModel();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}