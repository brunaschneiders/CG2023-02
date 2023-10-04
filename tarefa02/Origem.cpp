/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico/Computa��o Gr�fica - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 09/08/2023
 *
 */

#include <iostream>
#include <string>
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
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

Camera camera;

char rotateChar;

int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cubo com movimentação da camera!", nullptr, nullptr);
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
	Shader shader("./shaders/cubo.vs", "./shaders/cubo.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();


	glUseProgram(shader.ID);

	// Criando a matriz de modelo => matriz de transforma��o do objeto em si. Precisa colocar ela no shader pq � uma informa��o que vai ser usada por ele.
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	//
	//model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// envia a matriz de model pro shader. glm::value_ptr(model) é o que transforma o glm::mat4 em um array de char.
	// lá ela é multiplicada ela pela matriz que tem as transformações do objeto (model) e pela coordenada do vértice
	shader.setMat4("model", glm::value_ptr(model));

	camera.initialize(&shader, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);


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
		float offset = cos((GLfloat)glfwGetTime());
		// movimenta o objeto
		// model = glm::translate(model, glm::vec3(0.0, 0.0, offset));

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

		// vista de frente
		// case '1':
		// 	cameraPos = glm::vec3(0.0, 0.0, -10.0);
		// 	cameraFront = glm::vec3(0.0, 0.0, 1.0);

		// 	break;

		// // vista de trás
		// case '2':
		// 	cameraPos = glm::vec3(0.0, 0.0, 10.0);
		// 	cameraFront = glm::vec3(0.0, 0.0, -1.0);

		// 	break;

		// //  vista de lado (direito)
		// case '3':
		// 	cameraPos = glm::vec3(10.0, 0.0, 0.0);
		// 	cameraFront = glm::vec3(-1.0, 0.0, 0.0);

		// 	break;
		
	    // // vista de lado (esquerdo)
		// case '4':
		// 	cameraPos = glm::vec3(-10.0, 0.0, 0.0);
		// 	cameraFront = glm::vec3(1.0, 0.0, 0.0);

		// 	break;

		// // vista de cima
		// case '5':
		// 	cameraPos = glm::vec3(0.0, 10.0, 0.0);
		// 	cameraFront = glm::vec3(0.0, -1.0, 0.0);

		// 	break;
		}

		// manda informa��o de model pro shader
		shader.setMat4("model", glm::value_ptr(model));

		camera.update();
		
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		// necessarios dar um bind no VAO antes de desenhar os triangulos de cada geometria.


		glBindVertexArray(VAO);
	    // desenha os triangulos. No caso da primitiva GL_TRIANGLES, a cada 3 v�rices, ele fecha um tri�ngulo v�lido.
		glDrawArrays(GL_TRIANGLES, 0, 42);

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

	float cameraSpeed = 0.1;

	if (key == GLFW_KEY_W) camera.moveForward();
	if (key == GLFW_KEY_S) camera.moveBackward();
	if (key == GLFW_KEY_A) camera.moveLeft();
	if (key == GLFW_KEY_D) camera.moveRight();

	if (key == GLFW_KEY_1) rotateChar = '1';
	if (key == GLFW_KEY_2) rotateChar = '2';
	if (key == GLFW_KEY_3 ) rotateChar = '3';
	if (key == GLFW_KEY_4) rotateChar = '4';
	if (key == GLFW_KEY_5) rotateChar = '5';
}



// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
// Ela criar o buffer com os v�rtices da geometria.
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {
		// => Cubo

		//Base do cubo: 2 tri�ngulos (violeta)
		//x    y    z    r    g    b
		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // v0
		-0.5, -0.5,  0.5, 1.0, 0.0, 1.0, // v1
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // v2

		 -0.5, -0.5, 0.5, 1.0, 0.0, 1.0, //v3
		  0.5, -0.5, 0.5, 1.0, 0.0, 1.0, //v4 
		  0.5, -0.5,-0.5, 1.0, 0.0, 1.0, //v5

		  //Parte superior do cubo: 2 tri�ngulos (verde)
		//x    y    z    r    g    b
		-0.5, 0.5, -0.5, 0.0, 1.0, 0.0, // v6
		-0.5, 0.5,  0.5, 0.0, 1.0, 0.0, // v7
		 0.5, 0.5, -0.5, 0.0, 1.0, 0.0, // v8

		 -0.5, 0.5, 0.5, 0.0, 1.0, 0.0, //v9
		  0.5, 0.5, 0.5, 0.0, 1.0, 0.0, //v10 
		  0.5, 0.5,-0.5, 0.0, 1.0, 0.0, //v11

		  // Quadrado frontal (verde claro)
		  -0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //v12
		   -0.5, 0.5, -0.5, 1.0, 1.0, 0.0, //v13
		   0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //v14

		   -0.5, 0.5, -0.5, 1.0, 1.0, 0.0, //v15
		   0.5, 0.5, -0.5, 1.0, 1.0, 0.0, //v16
		   0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //v17

		   // Quadrado lateral direito (azul)
		   0.5, -0.5, -0.5, 0.0, 1.0, 1.0, //v18
		   0.5, 0.5, -0.5, 0.0, 1.0, 1.0, //v19
		   0.5, -0.5, 0.5, 0.0, 1.0, 1.0, //v20

		   0.5, 0.5, -0.5, 0.0, 1.0, 1.0, //v21
		   0.5, 0.5, 0.5, 0.0, 1.0, 1.0, //v22
		   0.5, -0.5, 0.5, 0.0, 1.0, 1.0, //v23

		   // Quadrado lateral esquerdo (vermelho)

			-0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // v24
			-0.5, 0.5, -0.5, 1.0, 0.0, 0.0, // v25
			-0.5, -0.5, 0.5, 1.0, 0.0, 0.0, // v26

			 -0.5, 0.5, -0.5, 1.0, 0.0, 0.0, //v27
			 -0.5, 0.5, 0.5, 1.0, 0.0, 0.0, //v28
			 -0.5, -0.5, 0.5, 1.0, 0.0, 0.0, //v29

		  // Quadrado traseiro (amarelo)
		 -0.5, -0.5, 0.5, 0.8, 1.0, 0.7, //v30
		 -0.5, 0.5, 0.5, 0.8, 1.0, 0.7, //v31
		  0.5, -0.5, 0.5, 0.8, 1.0, 0.7, //v32

		  -0.5, 0.5, 0.5, 0.8, 1.0, 0.7, //v33
		  0.5, 0.5,  0.5, 0.8, 1.0, 0.7, //v34
		  0.5, -0.5, 0.5, 0.8, 1.0, 0.7, //v35

		  // => Ch�o (marrom)
		-2.0, -0.5, -2.0, 0.9, 0.5, 0.4, // v0
		-2.0, -0.5,  2.0, 0.9, 0.5, 0.4, // v1
		 2.0, -0.5, -2.0, 0.9, 0.5, 0.4, // v2

		 -2.0, -0.5, 2.0, 0.9, 0.5, 0.4, //v3
		  2.0, -0.5, 2.0, 0.9, 0.5, 0.4, //v4 
		  2.0, -0.5, -2.0, 0.9, 0.5, 0.4, //v5
	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouseMovement(window, xpos, ypos);
}