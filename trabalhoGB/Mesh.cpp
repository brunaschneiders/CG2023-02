#include "Mesh.h"
#include <vector>

void Mesh::initialize(GLuint VAO, Shader* shader, int nVerts, GLuint texID, glm::vec3 initialPosition, float initialScale, glm::vec3 ka, glm::vec3 ks, float q)
{
	this->VAO = VAO;
	this->shader = shader;
	this->nVerts = nVerts;
	this->texID = texID;
	this->scaleLevel = initialScale;
	this->initialPosition = initialPosition;


	// criando as informações de iluminação
	shader->setVec3("ka", ka[0], ka[1], ka[2]);
	shader->setFloat("kd", 1.0f);
	shader->setVec3("ks", ks[0], ks[1], ks[2]);
	shader->setFloat("q", q);
}

void Mesh::update(char rotateChar)
{
	// Criando a matriz de modelo => matriz de transforma��o do objeto em si. Precisa colocar ela no shader pq � uma informa��o que vai ser usada por ele.
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	setupRotation(rotateChar, model);
	setupTranslation(model);
	setupScale(model);
	// envia a matriz de model pro shader. glm::value_ptr(model) é o que transforma o glm::mat4 em um array de char.
	// lá ela é multiplicada pela matriz que tem as transformações do objeto (model) e pela coordenada do vértice
	shader->setMat4("model", glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(shader->ID, "tex_buffer"), 0);
}

// configura a rotação do objeto, se ela existir
void Mesh::setupRotation(char rotateChar, glm::mat4& model)
{
	float angle = (GLfloat)glfwGetTime();

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
}

// configura a translação do objeto, se ela existir
void Mesh::setupTranslation(glm::mat4& model)
{
	model = glm::translate(model, glm::vec3(initialPosition.x + translateXOffset, initialPosition.y + translateYOffset, initialPosition.z + translateZOffset));
}

// configura a escala do objeto. A escala é a mesma para todos os eixos
void Mesh::setupScale(glm::mat4& model)
{
	model = glm::scale(model, glm::vec3(scaleLevel, scaleLevel, scaleLevel));
}

void Mesh::incrementTranslationOffset(char translateChar, float translateStep)
{
	switch (translateChar) {
	case 'x':
		translateXOffset += translateStep;
		break;
	case 'y':
		translateYOffset += translateStep;
		break;
	case 'z':
		translateZOffset += translateStep;
		break;
	}
}

void Mesh::decrementTranslationOffset(char translateChar)
{
	float translateStep = 0.1f;

	switch (translateChar) {
	case 'x':
		translateXOffset -= translateStep;
		break;
	case 'y':
		translateYOffset -= translateStep;
		break;
	case 'z':
		translateZOffset -= translateStep;
		break;
	}
}

void Mesh::incrementScale()
{
	float scaleStep = 0.1f;
	scaleLevel += scaleStep;
}

void Mesh::decrementScale(float scaleStep)
{
	scaleLevel -= scaleStep;
}

void Mesh::draw()
{
	// Chamada de desenho - drawcall
	// Poligono Preenchido - GL_TRIANGLES
	// necessario dar um bind no VAO antes de desenhar os triangulos de cada geometria.
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	
	// desenha os triangulos. No caso da primitiva GL_TRIANGLES, a cada 3 v�rices, ele fecha um tri�ngulo v�lido.
	glDrawArrays(GL_TRIANGLES, 0, nVerts);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::deleteVAO() {
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::setTextureFilePath(string path) {
	this->texFilePath = path;
}

string Mesh::getTextureFilePath() {
	return texFilePath;
}