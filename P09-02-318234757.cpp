//Práctica 9 parte 2: Animación Avanzada	Uriarte Ortiz Enrique Yahir.
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>+m
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

//Variables de Actividad 01.
float movLetrero, movOffset, rotLetrero;
float toffsetnumerov, toffsetnumerocambiau;
float ultimaActivacion;
bool avanza2;

Model Arco;
Model Letrero;

Texture PalabraTexture;

//Variables de Actividad 02.
float movD, movDOffset, giro;
bool avanza, ida;

float r, espiralX, espiralY;		// Espiral de Arquimedes.
float rosaX, rosaY;					// Roseta.
float lemniscataX, lemniscataY;		// Lemniscata de Bernoulli.
float cardioideX, cardioideY;		// Cardioide.
float hipocicloideX, hipocicloideY; // Hipocicloide.

Model Tiamat_Cuerpo;
Model Tiamat_Ala01; Model Tiamat_Ala02;
Model Tiamat_Cabeza01; Model Tiamat_Cabeza02;
Model Tiamat_Cabeza03; Model Tiamat_Cabeza04;
Model Tiamat_Cabeza05;

//Actividad Extra - Efecto de Alientos
float toffsetAlientou = 0.0f;
float toffsetAlientov = 0.0f;
float toffsetAlientocambiau = 0.0;

Texture gasTexture;
Texture fuegoTexture;
Texture rayoTexture;
Texture gasVTexture;
Texture hieloTexture;
Texture arenaTexture;
Texture NothingTexture;


float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Texture pisoTexture;
Skybox skybox;

Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

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
	unsigned int indices[] = {0,3,1, 1,3,2, 2,3,0, 0,1,2};
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f};

	unsigned int floorIndices[] = {0,2,1, 1,2,3};
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f};

	unsigned int vegetacionIndices[] = {0,1,2, 0,2,3, 4,5,6, 4,6,7};
	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,};
	
	unsigned int flechaIndices[] = {0,1,2, 0,2,3};
	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f};

	unsigned int scoreIndices[] = {0,1,2, 0,2,3};
	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,	 0.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		 0.5f, 0.0f, 0.5f,	 1.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		 0.5f, 0.0f,-0.5f,	 1.0f, 1.0f,	0.0f,-1.0f, 0.0f,
		-0.5f, 0.0f,-0.5f,	 0.0f, 1.0f,	0.0f,-1.0f, 0.0f};

	

	

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	unsigned int numeroIndices[] = { 0,1,2, 0,2,3 };
	GLfloat numeroVertices[] = {
		-5.45f, 0.0f, 2.5f,	 0.0f, 0.0f,	0.0f,-1.0f, 0.0f,
		 5.45f, 0.0f, 2.5f,	 0.25f, 0.0f,	0.0f,-1.0f, 0.0f,
		 5.45f, 0.0f,-2.5f,	 0.25f, 1.0f,	0.0f,-1.0f, 0.0f,
		-5.45f, 0.0f,-2.5f,	 0.0f, 1.0f,	0.0f,-1.0f, 0.0f };

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	unsigned int elementosIndices[] = { 0,1,2, 0,2,3, };
	GLfloat elementosVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 1.0f, };
	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(elementosVertices, elementosIndices, 32, 6);
	meshList.push_back(obj8);
}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);}

int main() {
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	PalabraTexture = Texture("Textures/TNProy.png");
	PalabraTexture.LoadTextureA();

	Arco = Model();
	Arco.LoadModel("Models/Arco.obj");
	Letrero = Model();
	Letrero.LoadModel("Models/Letrero.obj");

	Tiamat_Cuerpo = Model();
	Tiamat_Cuerpo.LoadModel("Models/Tiamat_Cuerpo.obj");
	Tiamat_Ala01 = Model();
	Tiamat_Ala01.LoadModel("Models/Tiamat_Ala01.obj");
	Tiamat_Ala02 = Model();
	Tiamat_Ala02.LoadModel("Models/Tiamat_Ala02.obj");
	Tiamat_Cabeza01 = Model();
	Tiamat_Cabeza01.LoadModel("Models/Tiamat_Cabeza01.obj");
	Tiamat_Cabeza02 = Model();
	Tiamat_Cabeza02.LoadModel("Models/Tiamat_Cabeza02.obj");
	Tiamat_Cabeza03 = Model();
	Tiamat_Cabeza03.LoadModel("Models/Tiamat_Cabeza03.obj");
	Tiamat_Cabeza04 = Model();
	Tiamat_Cabeza04.LoadModel("Models/Tiamat_Cabeza04.obj");
	Tiamat_Cabeza05 = Model();
	Tiamat_Cabeza05.LoadModel("Models/Tiamat_Cabeza05.obj");

	gasTexture = Texture("Textures/gas.png");
	gasTexture.LoadTextureA();
	fuegoTexture = Texture("Textures/Fuego.png");
	fuegoTexture.LoadTextureA();
	rayoTexture = Texture("Textures/Rayo.png");
	rayoTexture.LoadTextureA();
	gasVTexture = Texture("Textures/GasV.png");
	gasVTexture.LoadTextureA();
	hieloTexture = Texture("Textures/Hielo.png");
	hieloTexture.LoadTextureA();
	arenaTexture = Texture("Textures/Arena.png");
	arenaTexture.LoadTextureA();
	NothingTexture = Texture("Textures/Nothing.tga");
	NothingTexture.LoadTextureA();

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

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Variables de Actividad 01.
	movLetrero = 0.0; movOffset = 0.4f; rotLetrero = 0.0f;
	toffsetnumerov = 0.0f;  toffsetnumerocambiau = 0.0f;
	ultimaActivacion = 0.0f;
	avanza2 = true; 
	
	//Variables de Actividad 02.
	movD = 0.0f; movDOffset = 0.3f; giro = 0.0f;
	avanza = true; ida = true;
	
	r = 0.0f; espiralX = 0.0f; espiralY = 0.0f; // Espiral de Arquimides.
	rosaX = rosaY = 0.0f;						// Roseta.
	lemniscataX = lemniscataY = 0.0f;			// Lemniscata de Bernoulli.
	cardioideX = cardioideY = 0.0f;				// Cardioide.
	hipocicloideX = hipocicloideY = 0.0f;		// Hipocicloide.

	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		rotLetrero += 3.5f * deltaTime;

		// Movimiento del letrero.
		if ((now - ultimaActivacion) >= 2.0f && avanza2 == true) {
			if (movLetrero > -41.0f) {movLetrero -= movOffset * deltaTime; rotLetrero = 0;}
			else {avanza2 = false; ultimaActivacion = now; rotLetrero = 0;} }
		else if ((now - ultimaActivacion) >= 2.0f && avanza2 == false) {
			if (movLetrero < 0.0f) {movLetrero += movOffset * deltaTime; rotLetrero = 0;}
			else { avanza2 = true; ultimaActivacion = now; rotLetrero = 0; } }

		// Movimiento del dragon (ida y vuelta).
		if (avanza == true) {
			if (ida == true) {
				if (movD > -100.0f) {movD -= movDOffset * deltaTime;}
				else { ida = false; giro += 180.0f; } }
			else {
				if (movD < 100.0f) {movD += movDOffset * deltaTime;}
				else { ida = true; giro += 180.0f;} } }

		angulovaria += 0.5f * deltaTime;

		// Funcion Espiral de Arquimedes.
		r = 2.0 + 0.01 * angulovaria;
		espiralX = r * cos(angulovaria * 0.5f);
		espiralY = r * sin(angulovaria * 0.5f);

		// Funcion Roseta (Curva de Rosa), 3 pétalos.
		float k = 3.0f; // Numero de pétalos
		float a = 30.0f; // Escala
		rosaX = a * cos(k * angulovaria * 0.05f) * cos(angulovaria * 0.05f);
		rosaY = a * cos(k * angulovaria * 0.05f) * sin(angulovaria * 0.05f);

		// Funcion Lemniscata de Bernoulli.
		float a_lemn = 30.0f; // Escala
		lemniscataX = (a_lemn * cos(angulovaria * 0.05f)) / (1 + sin(angulovaria * 0.05f) 
					  * sin(angulovaria * 0.05f));
		lemniscataY = (a_lemn * sin(angulovaria * 0.05f) * cos(angulovaria * 0.05f)) / (1 + sin(angulovaria * 0.05f) 
					  * sin(angulovaria * 0.05f));

		// Funcion Cardioide.
		float a_card = 15.0f; // Escala
		cardioideX = a_card * (2 * cos(angulovaria * 0.1f) - cos(2 * angulovaria * 0.1f));
		cardioideY = a_card * (2 * sin(angulovaria * 0.1f) - sin(2 * angulovaria * 0.1f));

		// Funcion Hipocicloide.
		float R = 20.0f; // radio del círculo fijo
		float r = 5.0f;  // radio del círculo móvil
		hipocicloideX = (R - r) * cos(angulovaria * 0.1f) + r * cos((R - r) * angulovaria * 0.1f / r);
		hipocicloideY = (R - r) * sin(angulovaria * 0.1f) - r * sin((R - r) * angulovaria * 0.1f / r);

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 JerC1(1.0); glm::mat4 JerC2(1.0); glm::mat4 JerC3(1.0);
		glm::mat4 JerC4(1.0); glm::mat4 JerC5(1.0);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Arco.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco.RenderModel();

		//Letrero
		model = glm::translate(model, glm::vec3(0.0f, movLetrero + 48.6f, 5.0f));
		if (rotLetrero <= 360.0f) { model = glm::rotate(model, rotLetrero * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));}
		else {model = glm::rotate(model, 360.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero.RenderModel();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		toffsetnumerocambiau -= 0.0025 * deltaTime; 
		if (toffsetnumerocambiau > 1.0) toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, movLetrero/3.4 + 12.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		if (rotLetrero <= 360.0f) { model = glm::rotate(model, rotLetrero * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); }
		else { model = glm::rotate(model, 360.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); }
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PalabraTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		model = glm::mat4(1.0);//Cuerpo de Tiamat
		model = glm::translate(model, glm::vec3(movD + 0.0f, 18.0f + (3 * sin(glm::radians(angulovaria * 5))), 30.0));
		//model = glm::translate(model, glm::vec3(0.0f, 18.0f, 30.0));
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 0.33f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Cuerpo.RenderModel();

		model = modelaux;//Ala 01 de Tiamat
		model = glm::translate(model, glm::vec3(0.65f, 0.65f, 0.22f));
		model = glm::rotate(model, 0.55f - sin(glm::radians(angulovaria * 3.5f)), glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 0.55f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Ala01.RenderModel();

		model = modelaux;//Ala 02 de Tiamat 
		model = glm::translate(model, glm::vec3(0.65f, 0.65f, -0.22f));
		model = glm::rotate(model, -0.55f + sin(glm::radians(angulovaria * 3.5f)), glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 0.55f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Ala02.RenderModel();

		toffsetAlientocambiau -= 0.01 * deltaTime;
		if (toffsetAlientocambiau > 1.0) toffsetAlientocambiau = 0.0;
		toffsetAlientov = 0.0;
		toffset = glm::vec2(toffsetAlientocambiau, toffsetAlientov);

		model = modelaux;//Cabeza 01 de Tiamat
		model = glm::translate(model, glm::vec3(-1.18f, 0.325f, 0.025f));
		model = glm::rotate(model, glm::radians(espiralX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(espiralY), glm::vec3(0.0f, 0.0f, 1.0f));
		JerC1 = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Cabeza01.RenderModel();

		model = modelaux;//Cabeza 02 de Tiamat
		model = glm::translate(model, glm::vec3(-0.765f, 0.075f, 0.6f));
		model = glm::rotate(model, glm::radians(rosaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rosaY), glm::vec3(0.0f, 0.0f, 1.0f));
		JerC2 = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Cabeza02.RenderModel();

		model = modelaux;//Cabeza 03 de Tiamat
		model = glm::translate(model, glm::vec3(-0.765f, 0.0f,-0.68f));
		model = glm::rotate(model, glm::radians(lemniscataX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(lemniscataY), glm::vec3(0.0f, 1.0f, 0.0f));
		JerC3 = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Cabeza03.RenderModel();
		
		model = modelaux;//Cabeza 04 de Tiamat
		model = glm::translate(model, glm::vec3(-0.6f, 1.15f,-0.55f));
		model = glm::rotate(model, glm::radians(cardioideX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(cardioideY), glm::vec3(0.0f, 1.0f, 0.0f));
		JerC4 = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Cabeza04.RenderModel();

		model = modelaux;//Cabeza 05 de Tiamat
		model = glm::translate(model, glm::vec3(-0.7f, 1.12f, 0.52f));
		model = glm::rotate(model, glm::radians(hipocicloideX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(hipocicloideY), glm::vec3(0.0f, 1.0f, 0.0f));
		JerC5 = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.545f, 0.271f, 0.075f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_Cabeza05.RenderModel();

		model = JerC1;//Aliento de FUEGO
		model = glm::translate(model, glm::vec3(-3.5f, -0.3f, -1.15f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 215 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (mainWindow.getF()) { fuegoTexture.UseTexture(); }
		else { NothingTexture.UseTexture(); }
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = JerC2;//Aliento de RAYO
		model = glm::translate(model, glm::vec3(-3.4f, -2.2f, 1.9f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 34 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 215 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (mainWindow.getT()) { rayoTexture.UseTexture(); }
		else { NothingTexture.UseTexture(); }
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = JerC3;//Aliento de GAS VENENOSO
		model = glm::translate(model, glm::vec3(-3.4f, 0.15f, -1.9f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 195 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.8f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (mainWindow.getG()) { gasVTexture.UseTexture(); }
		else { NothingTexture.UseTexture(); }
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = JerC4;//Aliento de Hielo
		model = glm::translate(model, glm::vec3(-2.95f, 1.32f, -2.8f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.4f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (mainWindow.getH()) { hieloTexture.UseTexture(); }
		else { NothingTexture.UseTexture(); }
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = JerC5;//Aliento de Arena
		model = glm::translate(model, glm::vec3(-1.85f, 1.7f, 3.0f));
		model = glm::rotate(model, -100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (mainWindow.getA()) { arenaTexture.UseTexture(); }
		else { NothingTexture.UseTexture(); }
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		

		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
