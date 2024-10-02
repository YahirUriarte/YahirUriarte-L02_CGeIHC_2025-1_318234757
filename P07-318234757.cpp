//Práctica 7: Iluminación 1		Uriarte Ortiz Enrique Yahir

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

Model Carro_Llantas;
Model Carro_Cuerpo;
Model Carro_Cofre;

Model Lampara;

Model Blackhawk_M;
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

DirectionalLight mainLight;// luz direccional

//para declarar varias luces de tipo pointlight
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
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Carro_Cuerpo = Model();
	Carro_Cuerpo.LoadModel("Models/Carro_Cuerpo.obj");
	Carro_Cofre = Model();
	Carro_Cofre.LoadModel("Models/Carro_Cofre.obj");
	Carro_Llantas = Model();
	Carro_Llantas.LoadModel("Models/Carro_Llanta.obj");

	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.obj");

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	

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

	mainLight = DirectionalLight(
		1.0f,1.0f, 1.0f,
		0.8f,0.2f,
		0.0f,0.0f,-1.0f);

	unsigned int pointLightCount = 0; //Luz de Lampara
	pointLights[0] = PointLight(
		1.0f, 1.0f, 1.0f,
		1.8f, 1.0f,
	    1.45f, 2.62f,-10.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f,1.0f,1.0f,	0.0f,2.0f,	  0.0f,0.0f,0.0f,	  0.0f,-1.0f,0.0f,	  1.0f,0.0f,0.0f,	5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(	//Foco Frontal
		0.0f, 0.0f, 1.0f,
		4.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
	   -1.0f, 0.0f, 0.0f,
		0.0f,0.05f, 0.0f,
		8.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(	//Foco Helicoptero
		1.0f, 1.0f, 0.0f,
		4.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;
	
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;//Crean mas luces puntuales y spotlight 
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	while (!mainWindow.getShouldClose()){////Loop mientras no se cierra la ventana
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

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);

		shaderList[0].SetPointLights(pointLights, pointLightCount);//Luz de Lampara

		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);		//Origen de las jererquias.
		glm::mat4 modelaux(1.0);
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

		spotLights[1].SetPos(glm::vec3(-3.5f + mainWindow.getmuevex(),0.55f, 0.4f));

		//Ejercicio 1: Agregarle a coche propio luz de 1 faro frontal color azul.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.7f, 2.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		Cofre = model;
		Llanta01 = model;
		Llanta02 = model;
		Llanta03 = model;
		Llanta04 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
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
		model = glm::translate(model, glm::vec3(0.0f, 11.0f, 50.0f));
		Cofre = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cofre.RenderModel();

		spotLights[2].SetPos(glm::vec3(-0.5f + mainWindow.getmuevex2(), 4.85f, 6.0f));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex2(), 5.0f, 6.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, -1.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();}
	return 0;}