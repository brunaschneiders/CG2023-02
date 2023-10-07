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


// Configuracao da window
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Prot�tipos das fun��es
int loadSimpleOBJ(string filepath, int& nVerts, glm::vec3 color = glm::vec3(1.0, 0.0, 0.0));

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

Camera camera;
char rotateChar;

int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho GA!", nullptr, nullptr);
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

	// Carrega a geometria do obj
	int nVerts;
	GLuint VAO = loadSimpleOBJ("../3DModels/Suzannes/suzanneTriLowPoly.obj", nVerts);


	glUseProgram(shader.ID);

	// Criando a matriz de modelo => matriz de transforma��o do objeto em si. Precisa colocar ela no shader pq � uma informa��o que vai ser usada por ele.
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	//
	//model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// envia a matriz de model pro shader. glm::value_ptr(model) é o que transforma o glm::mat4 em um array de char.
	// lá ela é multiplicada ela pela matriz que tem as transformações do objeto (model) e pela coordenada do vértice
	shader.setMat4("model", glm::value_ptr(model));


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

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1); 

		// movimenta o objeto
		switch (rotateChar) {
		case 'x':
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			break;

		case 'y':
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			break;

		case 'z':
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		}

		// manda informa��o de model pro shader
		shader.setMat4("model", glm::value_ptr(model));

		camera.update();
		
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		// necessario dar um bind no VAO antes de desenhar os triangulos de cada geometria.
		glBindVertexArray(VAO);
	    // desenha os triangulos. No caso da primitiva GL_TRIANGLES, a cada 3 v�rices, ele fecha um tri�ngulo v�lido.
		glDrawArrays(GL_TRIANGLES, 0, nVerts);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		// VERTICES - GL_POINTS
		
		//glDrawArrays(GL_POINTS, 0, 426);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
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

	if (key == GLFW_KEY_X && action == GLFW_PRESS) rotateChar = 'x';
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) rotateChar = 'y';
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) rotateChar = 'z';

	if (key == GLFW_KEY_W) camera.moveForward();
	if (key == GLFW_KEY_S) camera.moveBackward();
	if (key == GLFW_KEY_A) camera.moveLeft();
	if (key == GLFW_KEY_D) camera.moveRight();
	if (key == GLFW_KEY_Q) camera.moveUp();

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