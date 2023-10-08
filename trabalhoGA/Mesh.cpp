#include "Mesh.h"

void Mesh::initialize(GLuint VAO, Shader* shader, int nVerts)
{
	this->VAO = VAO;
	this->shader = shader;
	this->nVerts = nVerts;
}

void Mesh::update(char rotateChar)
{
	// Criando a matriz de modelo => matriz de transforma��o do objeto em si. Precisa colocar ela no shader pq � uma informa��o que vai ser usada por ele.
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	setupRotation(rotateChar, model);
	// envia a matriz de model pro shader. glm::value_ptr(model) é o que transforma o glm::mat4 em um array de char.
	// lá ela é multiplicada pela matriz que tem as transformações do objeto (model) e pela coordenada do vértice
	shader->setMat4("model", glm::value_ptr(model));
}

// configura a rotação do objeto, se ela existir
void Mesh::setupRotation(char rotateChar, glm::mat4& model)
{
	float angle = (GLfloat)glfwGetTime();

	switch (rotateChar) {
	case 'x':
		//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		break;

	case 'y':
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'z':
		model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		break;
	}
}

void Mesh::draw()
{
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
}
