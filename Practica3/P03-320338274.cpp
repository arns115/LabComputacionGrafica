//pr�ctica 3: Modelado Geom�trico y C�mara Sint�tica.
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
#include <gtc\quaternion.hpp>
//clases para dar orden y limpieza al c�digo
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
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*> meshListColor;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular(glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, glm::vec3 color4)
{
	GLfloat fac = sqrt(3);
	GLfloat altura = sqrt(2) / fac;
	GLfloat z = 1 / (fac * 2);
	GLfloat cc = altura / 4.0f; 
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -cc, -z,	color1[0], color1[1], color1[2], //0
		0.5f,-cc, -z,	color1[0], color1[1], color1[2],//1
		0.0f, -cc, 2 * z,	color1[0], color1[1], color1[2],//2

		0.0f,altura - cc,0.0f,	color2[0], color2[1], color2[2],//3
		0.5f,-cc,-z,	color2[0], color2[1], color2[2],//1
		-0.5f,-cc, -z,	color2[0], color2[1], color2[2],//2

		0.0f,altura - cc,0.0f,	color3[0], color3[1], color3[2],//3
		0.5f,-cc, -z,	color3[0], color3[1], color3[2],//2
		0.0f, -cc, 2 * z,	color3[0], color3[1], color3[2], //0

		0.0f, altura - cc,0.0f,	color4[0], color4[1], color4[2], //0
		-0.5f,-cc, -z,	color4[0], color4[1], color4[2],//1
		0.0f,-cc, 2 * z,	color4[0], color4[1], color4[2],//3

	};
	MeshColor* obj1 = new MeshColor();
	obj1->CreateMeshColor(vertices_piramide_triangular, 72);
	meshListColor.push_back(obj1);

}
/*
Crear cilindro, cono y esferas con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
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

	//ciclo for para crear la circunferencia inferior
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

	//ciclo for para crear la circunferencia superior
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

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los v�rtices de la circunferencia del cono
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


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

// Funci�n para crear aristas de la pir�mide triangular
void CrearAristas()
{
	GLfloat fac = sqrt(3);
	GLfloat altura = sqrt(2) / fac;
	GLfloat z = 1 / (fac * 2);
	GLfloat cc = altura / 4.0f;
	GLfloat grosor = 0.04f;

	vector<GLfloat> vertices_aristas;
	vector<unsigned int> indices_aristas;

	// Vertices de la pir�mide triangular
	glm::vec3 piramide_vertices[4] = {
		{-0.5f, -cc, -z},
		{0.5f, -cc, -z},
		{0.0f, -cc, 2 * z},
		{0.0f, altura - cc, 0.0f}
	};

	// Definici�n de las aristas de la pir�mide
	int aristas_piramide[6][2] = {
		{0, 1}, {1, 2}, {2, 0},  // Base 
		{0, 3}, {1, 3}, {2, 3}   
	};

	for (int e = 0; e < 6; e++) {
		glm::vec3 inicio = piramide_vertices[aristas_piramide[e][0]];
		glm::vec3 fin = piramide_vertices[aristas_piramide[e][1]];
		glm::vec3 direccion = glm::normalize(fin - inicio);
		
		// Calcular vectores perpendiculares para crear el grosor de la arista
		glm::vec3 perpendicular1 = glm::vec3(-direccion.y, direccion.x, 0.0f);
		if (glm::length(perpendicular1) < 0.1f) {
			perpendicular1 = glm::vec3(0.0f, -direccion.z, direccion.y);
		}
		perpendicular1 = glm::normalize(perpendicular1) * grosor;

		glm::vec3 perpendicular2 = glm::cross(direccion, perpendicular1) * grosor;

		int indiceBase = vertices_aristas.size() / 3;

		// Crear los 8 v�rtices del prisma rectangular que representa la arista
		glm::vec3 vertices_arista[8] = {
			inicio + perpendicular1 + perpendicular2,  inicio + perpendicular1 - perpendicular2,
			inicio - perpendicular1 + perpendicular2,  inicio - perpendicular1 - perpendicular2,
			fin + perpendicular1 + perpendicular2,     fin + perpendicular1 - perpendicular2,
			fin - perpendicular1 + perpendicular2,     fin - perpendicular1 - perpendicular2
		};

		// Agregar v�rtices al vector
		for (int i = 0; i < 8; i++) {
			vertices_aristas.insert(vertices_aristas.end(), {
				vertices_arista[i].x, vertices_arista[i].y, vertices_arista[i].z
			});
		}

		// �ndices para formar las caras del prisma (linea con mas grosor)
		unsigned int indices_prisma[] = {
			indiceBase + 0, indiceBase + 1, indiceBase + 2,
			indiceBase + 1, indiceBase + 3, indiceBase + 2,
			indiceBase + 4, indiceBase + 6, indiceBase + 5,
			indiceBase + 5, indiceBase + 6, indiceBase + 7,
			indiceBase + 0, indiceBase + 2, indiceBase + 4,
			indiceBase + 2, indiceBase + 6, indiceBase + 4,
			indiceBase + 1, indiceBase + 5, indiceBase + 3,
			indiceBase + 3, indiceBase + 5, indiceBase + 7,
			indiceBase + 0, indiceBase + 4, indiceBase + 1,
			indiceBase + 1, indiceBase + 4, indiceBase + 5,
			indiceBase + 2, indiceBase + 3, indiceBase + 6,
			indiceBase + 3, indiceBase + 7, indiceBase + 6
		};


		indices_aristas.insert(indices_aristas.end(), 
			indices_prisma, indices_prisma + 36);
	}


	Mesh* aristas = new Mesh();
	aristas->CreateMeshGeometry(vertices_aristas, indices_aristas, 
		vertices_aristas.size(), indices_aristas.size());
	meshList.push_back(aristas);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
 shader2->CreateFromFiles(vShaderColor, fShader);
 shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	glm::vec3 colorRojo = glm::vec3(1.0f, 0.0f, 0.0f); // color 1 para cara de piramide (rojo)
	glm::vec3 colorVerde = glm::vec3(0.0f, 1.0f, 0.0f); // color 2 para cara de piramide (verde)
	glm::vec3 colorAzul = glm::vec3(0.0f, 0.0f, 1.0f); // color 3 para cara de piramide (azul)
	glm::vec3 colorRandom = glm::vec3(0.827f, 0.323f, 0.653f); // color 4 para cara de piramide (color random)
	glm::vec3 colorNegro = glm::vec3(0.0f, 0.0f, 0.0f); // color 5 para cara piramide negra (color negro)

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular(colorRojo, colorVerde, colorAzul, colorRandom);//�ndice 0 en MeshListColor
	CrearPiramideTriangular(colorNegro, colorNegro, colorNegro, colorNegro);//�ndice 1 en MeshListColor
	CrearAristas(); //indice 1 en MeshList
	CrearCilindro(5, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();

	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	glm::vec3 rotacionY(0.0f, 1.0f, 0.0f);
	glm::vec3 rotacionX(1.0f, 0.0f, 0.0f);
	glm::vec3 rotacionZ(0.0f, 0.0f, 1.0f);


	glm::quat quaternion;
	glm::quat quaternion1;
	glm::quat quaternion2;

	glm::mat4 matrizRotacion;




	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();

		//model = glm::mat4(1.0);
		////Traslaci�n inicial para posicionar en -Z a los objetos
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		////otras transformaciones para el objeto
		////model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		////model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		////model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		////model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la l�nea de proyecci�n solo se manda una vez a menos que en tiempo de ejecuci�n
		//se programe cambio entre proyecci�n ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		////meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular
		////meshList[2]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro, cono, pir�mide base cuadrangular
		//sp.render(); //dibuja esfera
		//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.285f, -2.6f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[1]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.51f, -1.96f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.08f, -2.26f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.38f, -2.6f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.29f, -0.08f, -2.78f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.275f, -0.51f, -2.45f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.51f, -2.92f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.275f, -0.51f, -2.45f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.285f, -0.08f, -2.77f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.548f, -0.51f, -2.92f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.51f, -2.92f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.51f, -2.3f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(180.0f), rotacionY);
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.28f, -0.51f, -2.78));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(180.0f), rotacionY);
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.28f, -0.51f, -2.78f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(180.0f), rotacionY);
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.12f, 0.05f, -2.54f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(55.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, -0.38f, -2.7f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(52.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();
		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -0.38f, -2.2f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.12f, -0.38f, -2.23f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(-5.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.39f, -0.38f, -2.71f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(-5.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.11f, 0.05f, -2.53f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(-5.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -2.75f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(-5.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-58.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.27f, -0.38f, -2.9f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(-5.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-58.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.27f, -0.38f, -2.9f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(-5.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-58.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListColor[0]->RenderMeshColor();

		////model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.34f, -2.6f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//quaternion = glm::angleAxis(glm::radians(30.0f), rotacionY);
		//matrizRotacion = glm::mat4_cast(quaternion);
		//model = model * matrizRotacion;
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//meshListColor[0]->RenderMeshColor();

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.51f, -1.96f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.08f, -2.26f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.38f, -2.6f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.29f, -0.08f, -2.78f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.275f, -0.51f, -2.45f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.51f, -2.92f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.275f, -0.51f, -2.45f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.285f, -0.08f, -2.77f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.548f, -0.51f, -2.92f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.51f, -2.92f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.51f, -2.3f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(180.0f), rotacionY);
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.28f, -0.51f, -2.78));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(180.0f), rotacionY);
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.28f, -0.51f, -2.78f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(180.0f), rotacionY);
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.12f, 0.05f, -2.54f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(55.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, -0.38f, -2.7f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(52.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -0.38f, -2.2f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.12f, -0.38f, -2.23f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(-5.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.39f, -0.38f, -2.71f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(-5.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.11f, 0.05f, -2.53f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(50.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(-5.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-35.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -2.75f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(-5.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-58.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.27f, -0.38f, -2.9f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(-5.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-58.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.27f, -0.38f, -2.9f));
		model = glm::scale(model, glm::vec3(0.43f, 0.43f, 0.43f));
		quaternion = glm::angleAxis(glm::radians(-5.0f), rotacionX);
		quaternion1 = glm::angleAxis(glm::radians(10.0f), rotacionY);
		quaternion2 = glm::angleAxis(glm::radians(-58.0f), rotacionZ);
		quaternion = quaternion2 * quaternion1 * quaternion;
		matrizRotacion = glm::mat4_cast(quaternion);
		model = model * matrizRotacion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();



		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


