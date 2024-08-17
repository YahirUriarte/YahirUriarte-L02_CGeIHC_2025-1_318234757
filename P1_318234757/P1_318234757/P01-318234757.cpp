#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h>  // Para rand()
#include <time.h>    // Para time()

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);	 			\n\
}";

void CrearTriangulo(){
	GLfloat vertices[] = {
		//Coordenadas para Letra U
		-0.2f,0.1f,0.0f,-0.8f,0.1f,0.0f,-0.2f,0.3f,0.0f,//Parte inferior 1
		-0.8f,0.1f,0.0f,-0.8f,0.3f,0.0f,-0.2f,0.3f,0.0f,//Parte inferior 2
		-0.8f,0.3f,0.0f,-0.6f,0.3f,0.0f,-0.6f,0.9f,0.0f,//Columna izquierda 1
		-0.8f,0.3f,0.0f,-0.8f,0.9f,0.0f,-0.6f,0.9f,0.0f,//Columna izquierda 2
		-0.2f,0.3f,0.0f,-0.4f,0.3f,0.0f,-0.4f,0.9f,0.0f,//Columna derecha 1
		-0.2f,0.3f,0.0f,-0.4f,0.9f,0.0f,-0.2f,0.9f,0.0f,//Columna derecha 2

		//Coordenadas para Letra O
		0.2f,0.1f,0.0f,0.4f,0.1f,0.0f,0.4f,0.7f,0.0f,//Rectangulo 1.1
		0.2f,0.1f,0.0f,0.2f,0.7f,0.0f,0.4f,0.7f,0.0f,//Rectangulo 1.2
		0.4f,0.1f,0.0f,0.8f,0.1f,0.0f,0.8f,0.3f,0.0f,//Rectangulo 2.1
		0.4f,0.1f,0.0f,0.4f,0.3f,0.0f,0.8f,0.3f,0.0f,//Rectangulo 2.2
		0.8f,0.3f,0.0f,0.8f,0.9f,0.0f,0.6f,0.3f,0.0f,//Rectangulo 3.1
		0.6f,0.3f,0.0f,0.6f,0.9f,0.0f,0.8f,0.9f,0.0f,//Rectangulo 3.2
		0.2f,0.7f,0.0f,0.6f,0.7f,0.0f,0.6f,0.9f,0.0f,//Rectangulo 4.1
		0.2f,0.7f,0.0f,0.2f,0.9f,0.0f,0.6f,0.9f,0.0f,//Rectangulo 4.2

		//Coordenadas para Letra E
		-0.8f,-0.1f,0.0f,-0.8f,-0.9f,0.0f,-0.6f,-0.1f,0.0f,//Columna 1
		-0.8f,-0.9f,0.0f,-0.6f,-0.9f,0.0f,-0.6f,-0.1f,0.0f,//Columna 2
		-0.6f,-0.1f,0.0f,-0.2f,-0.1f,0.0f,-0.6f,-0.3f,0.0f,//Rectangulo 1.1
		-0.6f,-0.3f,0.0f,-0.2f,-0.3f,0.0f,-0.2f,-0.1f,0.0f,//Rectangulo 1.2
		-0.6f,-0.4f,0.0f,-0.2f,-0.4f,0.0f,-0.6f,-0.6f,0.0f,//Rectangulo 2.1
		-0.6f,-0.6f,0.0f,-0.2f,-0.6f,0.0f,-0.2f,-0.4f,0.0f,//Rectangulo 2.2
		-0.6f,-0.7f,0.0f,-0.6f,-0.9f,0.0f,-0.2f,-0.7f,0.0f,//Rectangulo 3.1
		-0.6f,-0.9f,0.0f,-0.2f,-0.9f,0.0f,-0.2f,-0.7f,0.0f,//Rectangulo 3.2

		//Coordenadas para Letra Y
		0.4f,-0.9f,0.0f,0.6f,-0.9f,0.0f,0.4f,-0.5f,0.0f,//Columna 1
		0.4f,-0.5f,0.0f,0.6f,-0.9f,0.0f,0.6f,-0.5f,0.0f,//Columna 2
		0.4f,-0.5f,0.0f,0.1f,-0.2f,0.0f,0.2f,-0.1f,0.0f,//Rectangulo 1.1
		0.4f,-0.5f,0.0f,0.5f,-0.4f,0.0f,0.2f,-0.1f,0.0f,//Rectangulo 1.2
		0.6f,-0.5f,0.0f,0.5f,-0.4f,0.0f,0.8f,-0.1f,0.0f,//Rectangulo 2.1
		0.6f,-0.5f,0.0f,0.9f,-0.2f,0.0f,0.8f,-0.1f,0.0f,//Rectangulo 2.2
		0.4f,-0.5f,0.0f,0.6f,-0.5f,0.0f,0.5f,-0.4f,0.0f,//Triangulo central
	};

	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){ //Función para agregar los shaders a la tarjeta gráfica, the Program recibe los datos de theShader
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);//verificaciones y prevención de errores
	if (!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders(){
	shader = glCreateProgram();//se crea un programa
	if (!shader){
		printf("Error creando el shader");
		return;}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	GLint result = 0;//Para terminar de linkear el programa y ver que no tengamos errores
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	glGetProgramiv(shader, GL_LINK_STATUS, &result);//verificaciones y prevención de errores
	if (!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;}
}

int main(){
	if (!glfwInit()){ //Inicialización de GLFW
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);//CREAR VENTANA
	if (!mainWindow){
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;}

	int BufferWidth, BufferHeight;//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
	glfwMakeContextCurrent(mainWindow);//asignar el contexto

	glewExperimental = GL_TRUE;//permitir nuevas extensiones
	if (glewInit() != GLEW_OK){
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;}

	// Asignar valores de la ventana y coordenadas
	glViewport(0, 0, BufferWidth, BufferHeight);//Asignar Viewport

	//Crear tríangulo
	CrearTriangulo();
	CompileShaders();

	srand(time(NULL));//Generar número aleatorio
	int Color = rand() % 3;//Selecciona el color aleatorio
	double anterior = glfwGetTime();//Tiempo del último cambio

	while (!glfwWindowShouldClose(mainWindow)){
		//Loop mientras no se cierra la ventana
		glfwPollEvents();//Recibir eventos del usuario

		double tiempo = glfwGetTime();// Obtener el tiempo actual

		if (tiempo - anterior >= 2.0){//Cambiar cada 2 segundos
			Color = rand()%3;//Seleccionar un color aleatorio
			anterior = tiempo;}

		switch (Color){//Establecer el color de fondo según el color actual
		case 0: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break;//Rojo
		case 1: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break;//Verde
		case 2: glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break;//Azul
		}

		//Limpiar la ventana
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 87);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}
	return 0;
}