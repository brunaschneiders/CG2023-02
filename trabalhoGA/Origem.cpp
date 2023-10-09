#include <iostream>
#include <fstream>	
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


// Configuracao da window
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Prot�tipos das fun��es
int loadSimpleOBJ(string filepath, int& nVerts, glm::vec3 color = glm::vec3(1.0, 0.0, 0.0));

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

Camera camera;
char actionChar;
char rotateChar;
char translateChar;
bool displayAllObjects = false;

std::vector<std::string> objFilesPath = {
	"../3DModels/Suzannes/suzanneTriLowPoly.obj",
	"../3DModels/NaveET/NaveET.obj",
	"../3DModels/Classic-NoTexture/bunny.obj",
	"../3DModels/Classic-NoTexture/camel.obj",
	"../3DModels/Classic-NoTexture/cat.obj",
};

std::vector<Mesh> objects;
int currentObjectIndex = 0;

int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho GA - Visualizador 3D!", nullptr, nullptr);
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
	Shader shader("./shaders/mesh.vs", "./shaders/mesh.fs");

	glUseProgram(shader.ID);

	// Carrega a geometria dos obj
	for (const std::string& path : objFilesPath) {
		Mesh object;
		GLuint VAO;
		int nVerts = 0;

		VAO = loadSimpleOBJ(path, nVerts);
		object.initialize(VAO, &shader, nVerts);
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


		if (displayAllObjects) {
			float translationOffset = 0.0;

			for (Mesh object : objects) {
				object.incrementTranslationOffset('x', translationOffset);
				translationOffset += 2.0f;
				object.decrementScale(0.6f);
				object.update(rotateChar);
				object.draw();
			}
		}
		else {
			for (Mesh object : objects) {
				objects[currentObjectIndex].update(rotateChar);
				objects[currentObjectIndex].draw();
			}
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

	// controle do tipo de ação que está sendo realizada (rotação, translação ou alteração de escala)
	if (key == GLFW_KEY_T && action == GLFW_PRESS) actionChar = 't';
	if (key == GLFW_KEY_R && action == GLFW_PRESS) actionChar = 'r';
	if (key == GLFW_KEY_C && action == GLFW_PRESS) actionChar = 'c';

	// controla o eixo em que a ação será aplicada
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (actionChar == 't') translateChar = 'x';
		else if (actionChar == 'r') rotateChar = 'x';
	};
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		if (actionChar == 't') translateChar = 'y';
		else if (actionChar == 'r') rotateChar = 'y';
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (actionChar == 't') translateChar = 'z';
		else if (actionChar == 'r') rotateChar = 'z';
	}

	// controla a direção da translação
	if (key == GLFW_KEY_UP && actionChar == 't' && action == GLFW_PRESS) objects[currentObjectIndex].incrementTranslationOffset(translateChar);
	if (key == GLFW_KEY_DOWN && actionChar == 't' && action == GLFW_PRESS) objects[currentObjectIndex].decrementTranslationOffset(translateChar);

	// controla se a escala diminui ou aumenta
	if (key == GLFW_KEY_UP && actionChar == 'c' && action == GLFW_PRESS) objects[currentObjectIndex].incrementScale();
	if (key == GLFW_KEY_DOWN && actionChar == 'c' && action == GLFW_PRESS) objects[currentObjectIndex].decrementScale();

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

	// controla a exibição de todos os objetos na cena
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		displayAllObjects = !displayAllObjects;
	}

	// controla a movimentação da câmera
	if (key == GLFW_KEY_W) camera.moveForward();
	if (key == GLFW_KEY_S) camera.moveBackward();
	if (key == GLFW_KEY_A) camera.moveLeft();
	if (key == GLFW_KEY_D) camera.moveRight();
	if (key == GLFW_KEY_Q) camera.moveUp();

	// controla a alteração dos pontos de vista da câmera
	if (key == GLFW_KEY_1) camera.viewFront();
	if (key == GLFW_KEY_2) camera.viewRight();
	if (key == GLFW_KEY_3 ) camera.viewBack();
	if (key == GLFW_KEY_4) camera.viewLeft();
	if (key == GLFW_KEY_5) camera.viewTop();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouseMovement(window, xpos, ypos);
}

// gera o buffer de geometria e retorna o identificador do VAO que foi gerado a partir do arquivo lido
int loadSimpleOBJ(string filepath, int& nVerts, glm::vec3 color)
{
	//vector em que estaremos armazenando o buffer de geometria => é o array de floats da geometria
	vector <GLfloat> vbuffer;
	//vectors auxiliares na leitura    
	vector <glm::vec3> vertices;
	vector <glm::vec3> normals;
	vector <glm::vec2> texcoords;

	//stream de leitura de arquivo em C++    
	ifstream inputFile;
	// o c_str pega o array de char de dentro da string que é o que o open() espera.
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
			// word é a primeira palavra do obj
			ssline >> word;
			if (word == "v")
			{
				glm::vec3 v;
				// posição x, y e z do vértice
				ssline >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			if (word == "f")
			{
				string tokens[3];
				ssline >> tokens[0] >> tokens[1] >> tokens[2];
				for (int i = 0; i < 3; i++)
				{
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);

					// o -1 é pq no arquivo obj é considerado o indice começando em 1 
					int index = atoi(token.c_str()) - 1;


					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);

					vbuffer.push_back(rand() % 256 / 255.0);
					vbuffer.push_back(rand() % 256 / 255.0);
					vbuffer.push_back(rand() % 256 / 255.0);
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

	// retorna o num de vértices. 6 floats tem a informação do vértice nesse momento (sem textura e normal)
	nVerts = vbuffer.size() / 6; //Provisório    

	//Geração do identificador do VBO   
	glGenBuffers(1, &VBO);    //Faz a conexão (vincula) do buffer como um buffer de array    
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Envia os dados do array de floats para o buffer da OpenGl    
	// .size() * sizeof(GLfloat) é usado para pegar o numero de bytes do vbuffer, já que agora ele é alocado dinamicamente
	glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)    
	glGenVertexArrays(1, &VAO);    // Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices    
	// e os ponteiros para os atributos     

	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:     // Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)    // Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)     // Tipo do dado    // Se está normalizado (entre zero e um)    // Tamanho em bytes     // Deslocamento a partir do byte zero     //Atributo posição (x, y, z)    
	// 6 é o numero de floats que armazenam as informações do vértice. Começa em 0, e desloca de 3 em 3.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)    
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice     // atualmente vinculado - para que depois possamos desvincular com segurança    
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)    
	glBindVertexArray(0);

	return VAO;
}