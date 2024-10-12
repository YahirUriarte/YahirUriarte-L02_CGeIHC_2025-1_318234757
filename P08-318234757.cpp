//Práctica 8: Iluminación 2		Uriarte Ortiz Enrique Yahir.
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Model Carro_Llantas;
Model Carro_Cuerpo;
Model Carro_Cofre;
Model Lampara;
Model Movil;

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
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;}

	for (size_t i = 0; i < verticeCount / vLength; i++){
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;}
}

void CreateObjects(){
	unsigned int indices[] = {0,3,1, 1,3,2, 2,3,0, 0,1,2};
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {0,2,1, 1,2,3};
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {0,1,2, 0,2,3, 4,5,6, 4,6,7};
	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	
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

int main(){
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	Carro_Cuerpo = Model();
	Carro_Cuerpo.LoadModel("Models/Carro_Cuerpo.obj");
	Carro_Cofre = Model();
	Carro_Cofre.LoadModel("Models/Carro_Cofre.obj");
	Carro_Llantas = Model();
	Carro_Llantas.LoadModel("Models/Carro_Llanta.obj");

	Lampara = Model();		Lampara.LoadModel("Models/Lampara.obj");
	Movil = Model();		Movil.LoadModel("Models/Movil.obj");

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

	mainLight = DirectionalLight(1.0f,1.0f,1.0f, 0.05f,0.5f, 0.0f,0.0f,0.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight( //Luz de Lampara.
		1.0f, 1.0f, 1.0f,
		1.8f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.4f, 0.2f);
	pointLightCount++;

	pointLights[1] = PointLight( //Luz de Celular.
		1.0f, 1.0f, 1.0f,
		1.8f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.4f, 0.2f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	//Luz de cofre.
	spotLights[0] = SpotLight(1.0f,0.0f,0.0f, 4.0f,1.0f, 0.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, 1.0f,0.1f,0.1f, 25.0f);
	spotLightCount++;

	//Foco Frontal.
	spotLights[1] = SpotLight(0.0f,0.0f,1.0f, 4.0f,1.0f, 0.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, 1.0f,0.1f,0.05f, 8.0f);
	spotLightCount++;

	//Foco Trasero.
	spotLights[2] = SpotLight(1.0f,0.0f,1.0f, 4.0f,1.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.1f,0.05f, 8.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
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
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		int direccionCoche = mainWindow.getDireccionCoche();

		unsigned int activeLightCount = 0;
		PointLight activeLights[2];

		glm::mat4 model(1.0);		//Origen de las jererquias.
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::mat4 Cofre(1.0);		//Jerarquia con Cofre.
		glm::mat4 Llanta01(1.0);	//Jerarquia con llanta 1.
		glm::mat4 Llanta02(1.0);	//Jerarquia con llanta 2.
		glm::mat4 Llanta03(1.0);	//Jerarquia con llanta 3.
		glm::mat4 Llanta04(1.0);	//Jerarquia con llanta 4.
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.7f, 2.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		Cofre = model;
		Llanta01 = model;
		Llanta02 = model;
		Llanta03 = model;
		Llanta04 = model;

		glm::vec3 posicionCoche = glm::vec3(model[3]);
		spotLights[1].SetPos_Especifica(posicionCoche.x, posicionCoche.y + 0.5f, posicionCoche.z + 2.0f);
		spotLights[2].SetPos_Especifica(posicionCoche.x, posicionCoche.y + 0.5f, posicionCoche.z - 2.0f);

		// Ejercicio2. 2. Cuando el coche avanza se enciende un spotlight hacia adelante, al retroceder enciende un spotlight hacia atrás
		if (direccionCoche == 1) {// Encender foco frontal, apagar foco trasero
			spotLights[1].SetFlash(glm::vec3(posicionCoche.x - 4.0f, posicionCoche.y - 0.15f, posicionCoche.z - 1.5f),
								   glm::vec3(-1.0f, 0.0f, 0.0f));}
		else if (direccionCoche == -1) {// Apagar foco frontal, encender foco trasero
			spotLights[2].SetFlash(glm::vec3(posicionCoche.x + 4.0f, posicionCoche.y + -0.15f, posicionCoche.z - 1.5f),
								   glm::vec3(1.0f, 0.0f, 0.0f));}
		else {// Apagar ambos focos cuando no hay movimiento
			spotLights[1].SetFlash(posicionCoche, glm::vec3(0.0f));
			spotLights[2].SetFlash(posicionCoche, glm::vec3(0.0f));}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cuerpo.RenderModel();

		model = Llanta01;//Llanta01. Derecha.
		model = glm::translate(model, glm::vec3(30.0f, -20.0f, 70.0f));
		Llanta01 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();

		model = Llanta02;//Llanta02. Izquierda.
		model = glm::translate(model, glm::vec3(-30.0f, -20.0f, 70.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Llanta02 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();

		model = Llanta03;//Llanta03. Izquierda.
		model = glm::translate(model, glm::vec3(-30.0f, -20.0f, -55.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Llanta03 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();

		model = Llanta04;//Llanta04. Derecha.
		model = glm::translate(model, glm::vec3(30.0f, -20.0f, -55.0f));
		Llanta04 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();
		
		model = Cofre;//Cofre de carro.
		model = glm::translate(model, glm::vec3(0.0f, 11.0f, 51.125f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		Cofre = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cofre.RenderModel();
		
		// Ejercicio 1. Spotlight en el cofre de su coche, al abrir y cerrar el cofre ilumine en esa dirección.
		glm::vec3 LuzCofre_P = glm::vec3(Cofre[3]);
		glm::vec3 LuzCofre_D = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::mat4 RotCofre = glm::rotate(glm::mat4(1.0f), glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, -1.0f));
		LuzCofre_D = glm::vec3(RotCofre * glm::vec4(LuzCofre_D, 0.0f));
		LuzCofre_D = glm::normalize(LuzCofre_D);//Normalizar la dirección
		spotLights[0].SetFlash(LuzCofre_P, LuzCofre_D);
		

		// Ejercicio 3. Dos luces puntuales cada una a un modelo diferente,se prenden y apagan de forma independiente.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.95f, -10.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		glm::vec3 lamparaPos = glm::vec3(modelaux[3]);
		pointLights[0].SetPosition(lamparaPos.x + 1.45f, lamparaPos.y + 2.62f, lamparaPos.z);
		if (mainWindow.getLampLightState()) {activeLights[activeLightCount++] = pointLights[0];}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 15.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Movil.RenderModel();
		glm::vec3 MovilPos = glm::vec3(modelaux2[3]);
		pointLights[1].SetPosition(MovilPos.x + 0.0f, MovilPos.y + 1.0f, MovilPos.z);
		if (mainWindow.getNewLightState()) {activeLights[activeLightCount++] = pointLights[1];}
		
		shaderList[0].SetPointLights(activeLights, activeLightCount);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();}
	return 0;
}
