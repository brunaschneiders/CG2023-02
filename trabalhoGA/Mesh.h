#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Mesh
{
public:
	Mesh() {}
	~Mesh() {}
	void initialize(GLuint VAO, Shader* shader, int nVerts);
	void update(char rotateChar); // atualizar a matriz de transformações
	void setupRotation(char rotateChar, glm::mat4& model);
	void draw();

	// Métodos getters e setters
	void setPosition(/**/);
	// no código principal objeto1.setShader(&shader);
	//void setShader(Shader* shader) { this->shader = shader; }

protected:
	// lembrar bindVertexArray antes de desenhar
	GLuint VAO; // id do buffer da geometria
	int nVerts;

	// informações que compõem a matriz de modelo - model matriz
	glm::vec3 position;
	// para aplicar escala
	glm::vec3 dimensions;
	// rotações em x, y e z
	glm::vec3 angles;

	Shader* shader;
};

