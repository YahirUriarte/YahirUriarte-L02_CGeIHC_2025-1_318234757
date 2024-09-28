//Práctica 6: Texturizado.		Uriarte Ortiz Enrique Yahir.
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
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Texture elefanteTexture;
Texture pulpoTexture;
Texture mariquitaTexture;
Texture vacaTexture;
Texture perico_cerdoTexture;

Model Carro_Llantas;
Model Carro_Cuerpo;
Model Carro_Cofre;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

static const char* vShader = "shaders/shader_texture.vert";//Vertex Shader
static const char* fShader = "shaders/shader_texture.frag";//Fragment Shader

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,//Cálculo del promedio de las normales para sombreado de Phong.
	unsigned int vLength, unsigned int normalOffset){
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
	unsigned int indices[] = {0, 3, 1,	 1, 3, 2,	2, 3, 0,	0, 1, 2};
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f};

	unsigned int floorIndices[] = {0, 2, 1,		1, 2, 3};
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {0, 1, 2,	0, 2, 3,	4,5,6,	4,6,7};
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
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

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
}

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);}

void CrearDado_CarasElefante(){
	unsigned int cubo_indices[] = {
		0,1,3,		1,2,3,	//Cara 1
		4,5,7,		5,6,7,	//Cara 9
	};
	GLfloat cubo_vertices[] = {
		//Cara 1
		1.00f, 0.00f, 3.08f,	0.25f, 0.30f,	0.00f, 0.00f,-1.00f,//A - 0
	   -1.00f,-0.50f, 3.08f,	0.50f, 0.20f,	0.00f, 0.00f,-1.00f,//B - 1
	   -2.62f, 0.00f, 1.90f,	0.75f, 0.30f,	0.00f, 0.00f,-1.00f,//C - 2
	    0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	0.00f, 0.00f,-1.00f,//K	- 3
		//Cara 9
		2.62f,-0.50f,-1.90f,	0.25f, 0.70f,	0.00f, 0.00f,-1.00f,//H - 4
		3.24f, 0.00f, 0.00f,	0.50f, 0.80f,	0.00f, 0.00f,-1.00f,//I - 5
		2.62f,-0.50f, 1.90f,	0.75f, 0.70f,	0.00f, 0.00f,-1.00f,//J - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	0.00f, 0.00f,-1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);}

void CrearDado_CarasPulpo() {
	unsigned int cubo_indices[] = {
		0,1,3,		2,1,3,	//Cara 2
		4,5,7,		5,6,7,	//Cara 7
	};
	GLfloat cubo_vertices[] = {
		//Cara 2
	   -2.62f, 0.00f, 1.90f,	0.13f, 0.35f,	0.00f, 0.00f,-1.00f,//C - 0
	   -3.24f,-0.50f, 0.00f,	0.50f, 0.25f,	0.00f, 0.00f,-1.00f,//D - 1
	   -2.62f, 0.00f,-1.90f,	0.87f, 0.35f,	0.00f, 0.00f,-1.00f,//E - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	0.00f, 0.00f,-1.00f,//K	- 3
		//Cara 7
	   -3.24f,-0.50f, 0.00f,	0.13f, 0.65f,	0.00f, 0.00f,-1.00f,//D - 4
	   -2.62f, 0.00f,-1.90f,	0.50f, 0.75f,	0.00f, 0.00f,-1.00f,//E - 5
	   -1.00f,-0.50f,-3.08f,	0.87f, 0.65f,	0.00f, 0.00f,-1.00f,//F - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	0.00f, 0.00f,-1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);
}

void CrearDado_CarasMariquita() {
	unsigned int cubo_indices[] = {
		0,1,3,		2,1,3,	//Cara 3
		4,5,7,		5,6,7,	//Cara 10
	};
	GLfloat cubo_vertices[] = {
		//Cara 3
	   -2.62f, 0.00f,-1.90f,	0.10f, 0.35f,	0.00f, 0.00f,-1.00f,//E - 0
	   -1.00f,-0.50f,-3.08f,	0.50f, 0.25f,	0.00f, 0.00f,-1.00f,//F - 1
	    1.00f, 0.00f,-3.08f,	0.90f, 0.35f,	0.00f, 0.00f,-1.00f,//G - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	0.00f, 0.00f,-1.00f,//K	- 3
		//Cara 10
		2.62f,-0.50f, 1.90f,	0.10f, 0.65f,	0.00f, 0.00f,-1.00f,//J - 4
		1.00f, 0.00f, 3.08f,	0.50f, 0.75f,	0.00f, 0.00f,-1.00f,//A - 5
	   -1.00f,-0.50f, 3.08f,	0.90f, 0.65f,	0.00f, 0.00f,-1.00f,//B - 6
	    0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	0.00f, 0.00f,-1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);
}

void CrearDado_CarasVaca() {
	unsigned int cubo_indices[] = {
		0,1,3,		2,1,3,	//Cara 4
		4,5,7,		5,6,7,	//Cara 8
	};
	GLfloat cubo_vertices[] = {
		//Cara 4
		1.00f, 0.00f,-3.08f,	0.10f, 0.35f,	0.00f, 0.00f,-1.00f,//G - 0
	    2.62f,-0.50f,-1.90f,	0.50f, 0.25f,	0.00f, 0.00f,-1.00f,//H - 1
		3.24f, 0.00f, 0.00f,	0.90f, 0.35f,	0.00f, 0.00f,-1.00f,//I - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.00f,	0.00f, 0.00f,-1.00f,//K	- 3
		//Cara 8
	   -1.00f,-0.50f,-3.08f,	0.10f, 0.65f,	0.00f, 0.00f,-1.00f,//F - 4
	    1.00f, 0.00f,-3.08f,	0.50f, 0.75f,	0.00f, 0.00f,-1.00f,//G - 5
	    2.62f,-0.50f,-1.90f,	0.90f, 0.65f,	0.00f, 0.00f,-1.00f,//H - 6
		0.00f,-4.50f, 0.00f,	0.50f, 0.00f,	0.00f, 0.00f,-1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);
}

void CrearDado_Caras_PC() {
	unsigned int cubo_indices[] = {
		0,1,3,		2,1,3,	//Cara 5
		4,5,7,		5,6,7,	//Cara 6
	};
	GLfloat cubo_vertices[] = {
		//Cara 5
		3.24f, 0.00f, 0.00f,	0.10f, 0.625f,	0.00f, 0.00f,-1.00f,//I - 0
		2.62f,-0.50f, 1.90f,	0.50f, 0.58f,	0.00f, 0.00f,-1.00f,//J - 1
		1.00f, 0.00f, 3.08f,	0.90f, 0.625f,	0.00f, 0.00f,-1.00f,//A - 2
		0.00f, 4.00f, 0.00f,	0.50f, 1.10f,	0.00f, 0.00f,-1.00f,//K	- 3
		//Cara 6
	   -1.00f,-0.50f, 3.08f,	0.05f, 0.375f,	0.00f, 0.00f,-1.00f,//B - 4
	   -2.62f, 0.00f, 1.90f,	0.45f, 0.46f,	0.00f, 0.00f,-1.00f,//C - 5
	   -3.24f,-0.50f, 0.00f,	0.85f, 0.375f,	0.00f, 0.00f,-1.00f,//D - 6
		0.00f,-4.50f, 0.00f,	0.45f,-0.10f,	0.00f, 0.00f,-1.00f,//L - 7
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 64, 12);
	meshList.push_back(dado);
}

int main(){
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();

	CrearDado_CarasElefante();
	CrearDado_CarasPulpo();
	CrearDado_CarasMariquita();
	CrearDado_CarasVaca();
	CrearDado_Caras_PC();

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

	elefanteTexture = Texture("Textures/cara_ejefante.tga");
	elefanteTexture.LoadTextureA();
	pulpoTexture = Texture("Textures/cara_pulpo.tga");
	pulpoTexture.LoadTextureA();
	mariquitaTexture = Texture("Textures/cara_mariquita.tga");
	mariquitaTexture.LoadTextureA();
	vacaTexture = Texture("Textures/cara_vaca.tga");
	vacaTexture.LoadTextureA();
	perico_cerdoTexture = Texture("Textures/cara_perico-cerdo.tga");
	perico_cerdoTexture.LoadTextureA();

	Carro_Cuerpo = Model();
	Carro_Cuerpo.LoadModel("Models/Carro_Cuerpo.obj");
	Carro_Cofre = Model();
	Carro_Cofre.LoadModel("Models/Carro_Cofre.obj");
	Carro_Llantas = Model();
	Carro_Llantas.LoadModel("Models/Carro_Llanta.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);		//Origen de las jererquias.
	glm::mat4 Cofre(1.0);		//Jerarquia con Cofre.
	glm::mat4 Llanta01(1.0);	//Jerarquia con llanta 1.
	glm::mat4 Llanta02(1.0);	//Jerarquia con llanta 2.
	glm::mat4 Llanta03(1.0);	//Jerarquia con llanta 3.
	glm::mat4 Llanta04(1.0);	//Jerarquia con llanta 4.

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	while (!mainWindow.getShouldClose()){//Loop mientras no se cierra la ventana
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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Ejercicio 1: Crear un dado de 10 caras y texturizarlo por medio de código.

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		elefanteTexture.UseTexture();
		meshList[4]->RenderMesh();
		pulpoTexture.UseTexture();
		meshList[5]->RenderMesh();
		mariquitaTexture.UseTexture();
		meshList[6]->RenderMesh();
		vacaTexture.UseTexture();
		meshList[7]->RenderMesh();
		perico_cerdoTexture.UseTexture();
		meshList[8]->RenderMesh();
		
		//Ejercicio 2:Importar el modelo de su coche con sus 4 llantas acomodadas y tener texturizadas las 4 llantas (diferenciar caucho y rin)

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
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

		model = Llanta01;//Articulación - Llanta01. Derecha.
		model = glm::translate(model, glm::vec3(30.0f, -20.0f, 70.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta01 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();
		
		model = Llanta02;//Articulación - Llanta02. Izquierda.
		model = glm::translate(model, glm::vec3(-30.0f, -20.0f, 70.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta02 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();

		model = Llanta03;//Articulación - Llanta03. Izquierda.
		model = glm::translate(model, glm::vec3(-30.0f, -21.0f, -55.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta03 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();

		model = Llanta04;//Articulación - Llanta04. Derecha.
		model = glm::translate(model, glm::vec3(30.0f, -21.0f, -55.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		Llanta04 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Llantas.RenderModel();
		
		model = Cofre;//Articulación - Cofre de carro.
		model = glm::translate(model, glm::vec3(0.0f, 11.0f, 50.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		Cofre = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cofre.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();}

	return 0;
}