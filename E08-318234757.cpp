//Práctica 8: Iluminación 2		Uriarte Ortiz Enrique Yahir

#define STB_IMAGE_IMPLEMENTATION //para cargar imagen

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
//para probar el importer		#include<assimp/Importer.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
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

Texture AgaveTexture;
Texture elefanteTexture;
Texture pulpoTexture;
Texture mariquitaTexture;
Texture vacaTexture;
Texture perico_cerdoTexture;

Model Lampara;
Material Material_brillante;
Material Material_opaco;
Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

DirectionalLight mainLight;// luz direccional

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

static const char* vShader = "shaders/shader_light.vert"; // Vertex Shader
static const char* fShader = "shaders/shader_light.frag";// Fragment Shader

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
	unsigned int indices[] = {0,3,1,   1,3,2,	2,3,0,	0,1,2};
	GLfloat vertices[] = {
	//	x      y      z			  u	   v		 nx	   ny    nz
	  -1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   0.0f, -1.0f,  1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
	   1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   0.0f,  1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f};

	unsigned int floorIndices[] = {0,2,1,	1,2,3};
	GLfloat floorVertices[] = {
	 -10.0f,  0.0f,-10.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	  10.0f,  0.0f,-10.0f,     10.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	 -10.0f,  0.0f, 10.0f,	    0.0f,10.0f,		0.0f, -1.0f, 0.0f,
	  10.0f,  0.0f, 10.0f,	   10.0f,10.0f,		0.0f, -1.0f, 0.0f};

	unsigned int vegetacionIndices[] = {0,1,2,	0,2,3,	4,5,6,	4,6,7};
	GLfloat vegetacionVertices[] = {
	  -0.5f, -0.5f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   0.5f, -0.5f,  0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   0.5f,  0.5f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	  -0.5f,  0.5f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

	   0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   0.0f, -0.5f,  0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   0.0f,  0.5f,  0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	   0.0f,  0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f};
	
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

void Caras_Elef() {
	unsigned int cubo_indices[] = {0,1,3,  1,2,3,/*Cara 1*/ 4,5,7,  5,6,7/*Cara 9*/};
	GLfloat cubo_vertices[] = {
		//Cara 1
		1.00f, 0.00f, 3.08f,	0.75f, 0.30f,	0.00f,-1.00f,-1.00f,//A - 0
	   -1.00f,-0.50f, 3.08f,	0.50f, 0.20f,	0.00f,-1.00f,-1.00f,//B - 1
	   -2.62f, 0.00f, 1.90f,	0.25f, 0.30f,	0.00f,-1.00f,-1.00f,//C - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	0.00f,-1.00f,-1.00f,//K - 3
		//Cara 9
		2.62f,-0.50f,-1.90f,	0.75f, 0.70f,  -1.00f, 1.00f, 0.00f,//H - 4
		3.24f, 0.00f, 0.00f,	0.50f, 0.80f,  -1.00f, 1.00f, 0.00f,//I - 5
		2.62f,-0.50f, 1.90f,	0.25f, 0.70f,  -1.00f, 1.00f, 0.00f,//J - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,  -1.00f, 1.00f, 0.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);}

void Caras_Pulpo() {
	unsigned int cubo_indices[] = {0,1,3,  2,1,3,/*Cara 2*/ 4,5,7,  5,6,7/*Cara 7*/};
	GLfloat cubo_vertices[] = {
		//Cara 2
	   -2.62f, 0.00f, 1.90f,	0.87f, 0.35f,	1.00f,-1.00f, 0.00f,//C - 0
	   -3.24f,-0.50f, 0.00f,	0.50f, 0.25f,	1.00f,-1.00f, 0.00f,//D - 1
	   -2.62f, 0.00f,-1.90f,	0.13f, 0.35f,	1.00f,-1.00f, 0.00f,//E - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	1.00f,-1.00f, 0.00f,//K	- 3
		//Cara 7
	   -3.24f,-0.50f, 0.00f,	0.87f, 0.65f,	1.00f, 1.00f, 1.00f,//D - 4
	   -2.62f, 0.00f,-1.90f,	0.50f, 0.75f,	1.00f, 1.00f, 1.00f,//E - 5
	   -1.00f,-0.50f,-3.08f,	0.13f, 0.65f,	1.00f, 1.00f, 1.00f,//F - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	1.00f, 1.00f, 1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);}

void Caras_Mariq() {
	unsigned int cubo_indices[] = {0,1,3,  2,1,3,/*Cara 3*/ 4,5,7,  5,6,7/*Cara 10 */};
	GLfloat cubo_vertices[] = {
		//Cara 3
	   -2.62f, 0.00f,-1.90f,	0.90f, 0.35f,	0.00f, -1.00f,1.00f,//E - 0
	   -1.00f,-0.50f,-3.08f,	0.50f, 0.25f,	0.00f, -1.00f,1.00f,//F - 1
		1.00f, 0.00f,-3.08f,	0.10f, 0.35f,	0.00f, -1.00f,1.00f,//G - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	0.00f, -1.00f,1.00f,//K	- 3
		//Cara 10
		2.62f,-0.50f, 1.90f,	0.90f, 0.65f,	0.00f, 1.00f,-1.00f,//J - 4
		1.00f, 0.00f, 3.08f,	0.50f, 0.75f,	0.00f, 1.00f,-1.00f,//A - 5
	   -1.00f,-0.50f, 3.08f,	0.10f, 0.65f,	0.00f, 1.00f,-1.00f,//B - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	0.00f, 1.00f,-1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);} 

void Caras_Vaca() {
	unsigned int cubo_indices[] = {0,1,3,  2,1,3,/*Cara 4*/ 4,5,7,  5,6,7/*Cara 8*/};
	GLfloat cubo_vertices[] = {
		//Cara 4
		1.00f, 0.00f,-3.08f,	0.90f, 0.35f,	-1.00f, -1.00f, 1.00f,//G - 0
		2.62f,-0.50f,-1.90f,	0.50f, 0.25f,	-1.00f, -1.00f, 1.00f,//H - 1
		3.24f, 0.00f, 0.00f,	0.10f, 0.35f,	-1.00f, -1.00f, 1.00f,//I - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	-1.00f, -1.00f, 1.00f,//K	- 3
		//Cara 8
	   -1.00f,-0.50f,-3.08f,	0.90f, 0.65f,	0.00f, 1.00f,1.00f,//F - 4
		1.00f, 0.00f,-3.08f,	0.50f, 0.75f,	0.00f, 1.00f,1.00f,//G - 5
		2.62f,-0.50f,-1.90f,	0.10f, 0.65f,	0.00f, 1.00f,1.00f,//H - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	0.00f, 1.00f,1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);}

void Caras_PC() {
	unsigned int cubo_indices[] = {0,1,3,  2,1,3,/*Cara 5*/ 4,5,7,  5,6,7,/*Cara 6*/};
	GLfloat cubo_vertices[] = {
		//Cara 5
		3.24f, 0.00f, 0.00f,	0.90f, 0.625f,	-1.00f, -1.00f,-1.00f,//I - 0
		2.62f,-0.50f, 1.90f,	0.50f, 0.58f,	-1.00f, -1.00f,-1.00f,//J - 1
		1.00f, 0.00f, 3.08f,	0.10f, 0.625f,	-1.00f, -1.00f,-1.00f,//A - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.10f,	-1.00f, -1.00f,-1.00f,//K - 3
		//Cara 6
	   -1.00f,-0.50f, 3.08f,	0.85f, 0.375f,	1.00f, 1.00f, -1.00f,//B - 4
	   -2.62f, 0.00f, 1.90f,	0.45f, 0.46f,	1.00f, 1.00f, -1.00f,//C - 5
	   -3.24f,-0.50f, 0.00f,	0.05f, 0.375f,	1.00f, 1.00f, -1.00f,//D - 6
		0.00f,-4.50f, 0.00f,	0.45f,-0.10f,	1.00f, 1.00f, -1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);}

int main(){
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	Caras_Elef();	Caras_Pulpo();	Caras_Mariq();	Caras_Vaca();	Caras_PC();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");						brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");							dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");						plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");							pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");						AgaveTexture.LoadTextureA();
	elefanteTexture = Texture("Textures/cara_ejefante.tga");			elefanteTexture.LoadTextureA();
	pulpoTexture = Texture("Textures/cara_pulpo.tga");					pulpoTexture.LoadTextureA();
	mariquitaTexture = Texture("Textures/cara_mariquita.tga");			mariquitaTexture.LoadTextureA();
	vacaTexture = Texture("Textures/cara_vaca.tga");					vacaTexture.LoadTextureA();
	perico_cerdoTexture = Texture("Textures/cara_perico-cerdo.tga");	perico_cerdoTexture.LoadTextureA();

	Lampara = Model();		Lampara.LoadModel("Models/Lampara.obj");

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

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,		0.15f, 0.5f,		0.0f, 0.0f, 0.0f);

	unsigned int pointLightCount = 0; //Luz de Lampara
	pointLights[0] = PointLight(
		1.0f,1.0f,1.0f,  
		1.8f,1.0f,  
		0.0f,0.0f,0.0f,  
		1.0f,0.4f,0.2f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f,1.0f,1.0f,	0.0f,2.0f,	  0.0f,0.0f,0.0f,	  0.0f,-1.0f,0.0f,	  1.0f,0.05f,0.2f,	15.0f);
	spotLightCount++;
	
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;//Crean mas luces puntuales y spotlight 
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();//Recibir eventos del usuario
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Clear the window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();//información en el shader de intensidad especular y brillo
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);
		
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

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

		//Ejercicio 1: Agregar dado de 10 caras y que las caras sean iluminadas correctamente.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		elefanteTexture.UseTexture();		meshList[4]->RenderMesh();
		pulpoTexture.UseTexture();			meshList[5]->RenderMesh();
		mariquitaTexture.UseTexture();		meshList[6]->RenderMesh();
		vacaTexture.UseTexture();			meshList[7]->RenderMesh();
		perico_cerdoTexture.UseTexture();	meshList[8]->RenderMesh();

		//Ejercicio 2: Apagar con teclado la luz (pointlight) de su lámpara.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.95f, -10.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		glm::vec3 lamparaPos = glm::vec3(modelaux[3]);
		pointLights[0].SetPosition(lamparaPos.x + 1.45f, lamparaPos.y + 2.62f, lamparaPos.z);
		if (mainWindow.getLampLightState()){shaderList[0].SetPointLights(pointLights, pointLightCount);}
		else{shaderList[0].SetPointLights(pointLights, 0);} // Luz apagada, enviamos 0 luces puntuales
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

		glUseProgram(0);
		mainWindow.swapBuffers();}
	return 0;}