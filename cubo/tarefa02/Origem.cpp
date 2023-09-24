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


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

bool rotateX=false, rotateY=false, rotateZ=false;

// Variáveis de controle da câmera
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 15.0); // posição da camera => x, y, z
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0); // z é negativo pq o z de cameraPos é positivo (precisa sempre ser o inverso)
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0); // eixo y

// Fun��o MAIN
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
	Shader shader("../shaders/cubo.vs", "../shaders/cubo.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();


	glUseProgram(shader.ID);

	// Criando a matriz de modelo => matriz de transforma��o do objeto em si. Precisa colocar ela no shader pq � uma informa��o que vai ser usada por ele.
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// envia a matriz de model pro shader. glm::value_ptr(model) é o que transforma o glm::mat4 em um array de char
	shader.setMat4("model", glm::value_ptr(model));

	//Criando a matriz de proje��o
	glm::mat4 projection = glm::mat4(1); //matriz identidade;
	// projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0); // x min, x max, y min, y max, z min, z max. z min deve ser negativo e z max, positivo. Como o plano de projeção fica no v0, o valor de z não é muito relevante, já que ele trata da distância do observador e na projeção ortográfica isso não é relevante (independente da distancia, a visualização fica igual).
	projection = glm::perspective(glm::radians(40.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f); // FOV, aspecto ratio, z-near, z-far
	// envia a matriz de model pro shader e lá ela é multiplicada ela pela matriz que tem as transformações do objeto (model) e pela coordenada do vértice
	// se não criamos essa projeção, o openGl preenche como se fosse uma projeção paralela (ortográfica) cujo tamanho do volume de visualização forma um cubo.
	shader.setMat4("projection", glm::value_ptr(projection));

	//Criando a matriz de view
	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), cameraUp);
	shader.setMat4("view", glm::value_ptr(view));

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
		//model = glm::translate(model, glm::vec3(0.0, 0.0, offset));

		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			
		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		// manda informa��o de model pro shader
		shader.setMat4("model", glm::value_ptr(model));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", glm::value_ptr(view));
		
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

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	float cameraSpeed = 0.1;

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}

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
		-2.0, -2.0, -2.0, 4.0, 0.0, 4.0, // v0
		-2.0, -2.0,  2.0, 4.0, 0.0, 4.0, // v1
		 2.0, -2.0, -2.0, 4.0, 0.0, 4.0, // v2

		 -2.0, -2.0, 2.0, 4.0, 0.0, 4.0, //v3
		  2.0, -2.0, 2.0, 4.0, 0.0, 4.0, //v4 
		  2.0, -2.0,-2.0, 4.0, 0.0, 4.0, //v5

		  //Parte superior do cubo: 2 tri�ngulos (verde)
		//x    y    z    r    g    b
		-2.0, 2.0, -2.0, 0.0, 2.0, 0.0, // v6
		-2.0, 2.0,  2.0, 0.0, 2.0, 0.0, // v7
		 2.0, 2.0, -2.0, 0.0, 2.0, 0.0, // v8

		 -2.0, 2.0, 2.0, 0.0, 2.0, 0.0, //v9
		  2.0, 2.0, 2.0, 0.0, 2.0, 0.0, //v10 
		  2.0, 2.0,-2.0, 0.0, 2.0, 0.0, //v11

		  // Quadrado frontal (amarelo)
		  -2.0, -2.0, -2.0, 4.0, 4.0, 0.0, //v12
		   -2.0, 2.0, -2.0, 4.0, 4.0, 0.0, //v13
		   2.0, -2.0, -2.0, 4.0, 4.0, 0.0, //v14

		   -2.0, 2.0, -2.0, 4.0, 4.0, 0.0, //v15
		   2.0, 2.0, -2.0, 4.0, 4.0, 0.0, //v16
		   2.0, -2.0, -2.0, 4.0, 4.0, 0.0, //v17

		   // Quadrado lateral direito (azul)
		   2.0, -2.0, -2.0, 0.0, 4.0, 4.0, //v18
		   2.0, 2.0, -2.0, 0.0, 4.0, 4.0, //v19
		   2.0, -2.0, 2.0, 0.0, 4.0, 4.0, //v20

		   2.0, 2.0, -2.0, 0.0, 4.0, 4.0, //v21
		   2.0, 2.0, 2.0, 0.0, 4.0, 4.0, //v22
		   2.0, -2.0, 2.0, 0.0, 4.0, 4.0, //v23

		   // Quadrado lateral esquerdo (vermelho)

			-2.0, -2.0, -2.0, 4.0, 0.0, 0.0, // v24
			-2.0, 2.0, -2.0, 4.0, 0.0, 0.0, // v25
			-2.0, -2.0, 2.0, 4.0, 0.0, 0.0, // v26

			 -2.0, 2.0, -2.0, 4.0, 0.0, 0.0, //v27
			 -2.0, 2.0, 2.0, 4.0, 0.0, 0.0, //v28
			 -2.0, -2.0, 2.0, 4.0, 0.0, 0.0, //v29

		  // Quadrado traseiro (laranja)
		 -2.0, -2.0, 2.0, 4.0, 2.0, 0.0, //v30
		 -2.0, 2.0, 2.0, 4.0, 2.0, 0.0, //v31
		  2.0, -2.0, 2.0, 4.0, 2.0, 0.0, //v32

		  -2.0, 2.0, 2.0, 4.0, 2.0, 0.0, //v33
		  2.0, 2.0,  2.0, 4.0, 2.0, 0.0, //v34
		  2.0, -2.0, 2.0, 4.0, 2.0, 0.0, //v35

		  // => Ch�o (marrom)
		-4.0, -2.0, -4.0, 0.9, 0.5, 0.4, // v0
		-4.0, -2.0,  4.0, 0.9, 0.5, 0.4, // v1
		 4.0, -2.0, -4.0, 0.9, 0.5, 0.4, // v2

		 -4.0, -2.0, 4.0, 0.9, 0.5, 0.4, //v3
		  4.0, -2.0, 4.0, 0.9, 0.5, 0.4, //v4 
		  4.0, -2.0, -4.0, 0.9, 0.5, 0.4, //v5
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

