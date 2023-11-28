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
	void initialize(GLuint VAO, Shader* shader, int nVerts, GLuint texID, glm::vec3 initialPosition, float initialScale, glm::vec3 ka, glm::vec3 ks, float q);
	void update(glm::vec3 pointOnCurve); // atualiza a matriz de transforma��es
	void setupRotation(glm::mat4& model);
	void setupTranslation(glm::mat4& model, glm::vec3 pointOnCurve);
	void setupScale(glm::mat4& model);
	void updateRotateChar(char newRotateChar);
	void updateTranslateChar(char newTranslateChar);
	void incrementTranslationOffset();
	void decrementTranslationOffset();
	void incrementScale();
	void decrementScale(float scaleStep = 0.1f);
	void resetTranslation();
	void resetScale();
	void resetRotation();
	void draw();
	void deleteVAO();
	string getTextureFilePath();
	void setTextureFilePath(string path);
	glm::vec3 getPosition();

protected:
	GLuint VAO; // id do buffer da geometria
	int nVerts;
	GLuint texID;
	string texFilePath;

	// informa��es que comp�em a matriz de modelo - model matriz
	glm::vec3 position;
	// para aplicar escala
	glm::vec3 dimensions;
	// rota��es em x, y e z
	glm::vec3 angles;

	Shader* shader;

	// define o quanto o objeto est� sendo transladados em cada eixo
	float translateXOffset;
	float translateYOffset;
	float translateZOffset;

	// eixo selecionado para rota��o
	char rotateChar;
	// eixo selecionado para transla��o
	char translateChar;

	glm::vec3 initialPosition;

	// n�vel de escala que � aplicado em todos os eixos
	float scaleLevel;
	float initialScale;
};

