 

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <map>


using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB image
#define STB_IMAGE_IMPLEMENTATION   
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


// Configuracao da window
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Prot�tipos das fun��es
int loadSimpleOBJ(string filepath, int& nVerts, glm::vec3 color = glm::vec3(1.0, 0.0, 0.0));
int loadTexture(string path);
void loadMtl(string path, string& textureFilePathProp, glm::vec3& ka, glm::vec3& ks, float& q);
void loadObjectConfig(string path, string& objFilePath, string& mtlFilePath, glm::vec3& objPosition, float& objScale);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Variáveis
Camera camera;

// tipo de ação realizada na cena (translação, rotação ou alteração de escala)
char actionChar;
char TRANSLATE_ACTION_CHAR = 't';
char ROTATE_ACTION_CHAR = 'r';
char SCALE_ACTION_CHAR = 'c';
// eixo selecionado para rotação
char rotateChar;
// eixo selecionado para translação
char translateChar;

std::vector<std::string> configFilesPath = {
	"../3DModels/config/suzanne.txt",
	"../3DModels/config/terra.txt",
	"../3DModels/config/mercurio.txt",
	"../3DModels/config/hello-noise.txt",
};

std::vector<Mesh> objects;
int currentObjectIndex = 0;


int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho GB - Visualizador 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// Fazendo o registro da função mouse_callback  para a janela GLFW
	glfwSetCursorPosCallback(window, mouse_callback);

	// seta a posição do mouse para começar no meio da tela
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader("./shaders/objectsView.vs", "./shaders/objectsView.fs");

	glUseProgram(shader.ID);

	// Carrega a geometria e textura dos obj
	for (const std::string& path : configFilesPath) {
		Mesh object;
		GLuint VAO;

		string objFilePath, mtlFilePath, textureFilePath;

		int nVerts = 0;
		glm::vec3 objPosition;
		//iluminacao 
		glm::vec3 ka, ks;
		float ns, objScale = 1.0f;

		loadObjectConfig(path, objFilePath, mtlFilePath, objPosition, objScale);
		loadMtl(mtlFilePath, textureFilePath, ka, ks, ns);
		GLuint texID = loadTexture(textureFilePath);

		VAO = loadSimpleOBJ(objFilePath, nVerts);

		cout << "nVerts: " << nVerts << "mtlFilePath" << mtlFilePath << endl;
		object.initialize(VAO, &shader, nVerts, texID, objPosition, objScale, ka, ks, ns);
		objects.push_back(object);
	};

	glEnable(GL_DEPTH_TEST);

	camera.initialize(&shader, WIDTH, HEIGHT);


	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);


		float translationOffset = 0.0;

		for (Mesh object : objects) {
			//object.incrementTranslationOffset('x', translationOffset);
			//translationOffset += 2.0f;
			//object.decrementScale(0.6f);
			object.update(rotateChar);
			object.draw();
		}

		camera.update();

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	for (Mesh object : objects) {
		object.deleteVAO();
	}
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// controla a seleção do objeto
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		if (currentObjectIndex == objects.size() - 1) currentObjectIndex = 0;
		else currentObjectIndex += 1;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		if (currentObjectIndex == 0) currentObjectIndex = objects.size() - 1;
		else currentObjectIndex -= 1;
	}

	// controle do tipo de ação que está sendo realizada (rotação, translação ou alteração de escala)
	if (key == GLFW_KEY_T && action == GLFW_PRESS) actionChar = TRANSLATE_ACTION_CHAR;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) actionChar = ROTATE_ACTION_CHAR;
	if (key == GLFW_KEY_C && action == GLFW_PRESS) actionChar = SCALE_ACTION_CHAR;

	// controla o eixo em que a ação será aplicada
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (actionChar == TRANSLATE_ACTION_CHAR) translateChar = 'x';
		else if (actionChar == ROTATE_ACTION_CHAR) rotateChar = 'x';
	};
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		if (actionChar == TRANSLATE_ACTION_CHAR) translateChar = 'y';
		else if (actionChar == ROTATE_ACTION_CHAR) rotateChar = 'y';
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (actionChar == TRANSLATE_ACTION_CHAR) translateChar = 'z';
		else if (actionChar == ROTATE_ACTION_CHAR) rotateChar = 'z';
	}

	// controla a direção da translação
	if (key == GLFW_KEY_UP && actionChar == TRANSLATE_ACTION_CHAR) objects[currentObjectIndex].incrementTranslationOffset(translateChar);
	if (key == GLFW_KEY_DOWN && actionChar == TRANSLATE_ACTION_CHAR) objects[currentObjectIndex].decrementTranslationOffset(translateChar);

	// controla se a escala diminui ou aumenta
	if (key == GLFW_KEY_UP && actionChar == SCALE_ACTION_CHAR && action == GLFW_PRESS) objects[currentObjectIndex].incrementScale();
	if (key == GLFW_KEY_DOWN && actionChar == SCALE_ACTION_CHAR && action == GLFW_PRESS) objects[currentObjectIndex].decrementScale();

	// controla a movimentação da câmera
	if (key == GLFW_KEY_W) camera.moveForward();
	if (key == GLFW_KEY_S) camera.moveBackward();
	if (key == GLFW_KEY_A) camera.moveLeft();
	if (key == GLFW_KEY_D) camera.moveRight();
	if (key == GLFW_KEY_Q) camera.moveUp();

	// controla a alteração dos pontos de vista da câmera
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) camera.viewFront();
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) camera.viewRight();
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) camera.viewBack();
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) camera.viewLeft();
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) camera.viewTop();


	// reseta as transformações
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		actionChar = NULL;
		rotateChar = NULL;
		translateChar = NULL;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouseMovement(window, xpos, ypos);
}

// gera o buffer de geometria e retorna o identificador do VAO que foi gerado a partir do arquivo lido
int loadSimpleOBJ(string filepath, int& nVerts, glm::vec3 color)
{
	//vector em que estaremos armazenando o buffer de geometria
	vector <GLfloat> vbuffer;
	//vectors auxiliares na leitura    
	vector <glm::vec3> vertices;
	vector <glm::vec3> normals;
	vector <glm::vec2> texcoords;

	//stream de leitura de arquivo em C++    
	ifstream inputFile;
	inputFile.open(filepath.c_str());

	if (inputFile.is_open())
	{
		char line[100];
		string sline;
		while (!inputFile.eof())
		{
			inputFile.getline(line, 100);
			sline = line;
			string word;

			istringstream ssline(line);
			ssline >> word;
			if (word == "v")
			{
				glm::vec3 v;
				ssline >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			if (word == "vn")
			{
				glm::vec3 vn;
				ssline >> vn.x >> vn.y >> vn.z;
				normals.push_back(vn);

			}
			if (word == "vt")
			{
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				texcoords.push_back(vt);

			}
			if (word == "f")
			{
				string tokens[3];
				ssline >> tokens[0] >> tokens[1] >> tokens[2];
				for (int i = 0; i < 3; i++)
				{
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);
					int index = atoi(token.c_str()) - 1;

					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);

					vbuffer.push_back(1.0);
					vbuffer.push_back(0.0);
					vbuffer.push_back(0.0);

					//buscando o índice da coordenada de textura vt
					tokens[i] = tokens[i].substr(pos + 1, tokens[i].size());
					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					int indext = atoi(token.c_str()) - 1;
					//cout << token << endl;
					vbuffer.push_back(texcoords[indext].s);
					vbuffer.push_back(texcoords[indext].t);

					//buscando o índice do vetor normal vn
					token = tokens[i].substr(pos + 1, tokens[i].size());
					int indexn = atoi(token.c_str()) - 1;
					//cout << token << endl;
					vbuffer.push_back(normals[indexn].x);
					vbuffer.push_back(normals[indexn].y);
					vbuffer.push_back(normals[indexn].z);

				}
			}
		}
	}
	else
	{
		cout << "Problema ao encontrar o arquivo " << filepath << endl;
	}
	inputFile.close();
	GLuint VBO, VAO;

	nVerts = vbuffer.size() / 11;

	//Geração do identificador do VBO   

	glGenBuffers(1, &VBO);    //Faz a conexão (vincula) do buffer como um buffer de array    
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Envia os dados do array de floats para o buffer da OpenGl    
	glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)    
	glGenVertexArrays(1, &VAO);    // Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices    
	// e os ponteiros para os atributos     

	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:     // Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)    // Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)     // Tipo do dado    // Se está normalizado (entre zero e um)    // Tamanho em bytes     // Deslocamento a partir do byte zero     //Atributo posição (x, y, z)    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)    
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice     // atualmente vinculado - para que depois possamos desvincular com segurança    


	//Atributo coordenada de texturas (s,t)    
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice     // atualmente vinculado - para que depois possamos desvincular com segurança   

	//Atributo normal do vértice (x,y,z)    
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice     // atualmente vinculado - para que depois possamos desvincular com segurança    

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)    
	glBindVertexArray(0);

	return VAO;
}

void loadObjectConfig(string path, string& objFilePath, string& mtlFilePath, glm::vec3& objPosition, float& objScale) {
	Mesh object;

	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Failed to open the config file." << std::endl;
	}

	string line;

	while (std::getline(file, line)) {
		if (line.length() > 0) {

			std::istringstream iss(line);
			string prefix;
			iss >> prefix;
			if (prefix == "obj") {
				iss >> objFilePath;
				std::cout << objFilePath << std::endl;
			}
			else if (prefix == "mtl") {
				iss >> mtlFilePath;
				std::cout << mtlFilePath << std::endl;
			}
			else if (prefix == "position") {
				char comma;
				iss >> objPosition.x >> comma >> objPosition.y >> comma >> objPosition.z;
			}
			else if (prefix == "scale") {
				iss >> objScale;
			}
		}
	}

	file.close();
}

int loadTexture(string path)
{
	GLuint tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

void loadMtl(string path, string& textureFilePathProp, glm::vec3& ka, glm::vec3& ks, float& q) {
	std::ifstream file(path);

	cout << path << endl;

	if (!file.is_open()) {
		std::cout << "Failed to open mtl file." << std::endl;
	}

	string line, textureFilePath;

	while (std::getline(file, line)) {
		if (line.length() > 0) {

			std::istringstream iss(line);
			string prefix;
			iss >> prefix;

			if (prefix == "map_Kd") {
				iss >> textureFilePath;
				textureFilePathProp = textureFilePath;
			}
			else if (prefix == "Ka") {
				iss >> ka.x >> ka.y >> ka.z;

			}
			else if (prefix == "Ks") {
				iss >> ks.x >> ks.y >> ks.z;
			}
			else if (prefix == "Ns") {
				iss >> q;
			}
		}
	}

	file.close();
}