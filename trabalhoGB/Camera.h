#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <vector>

class Camera
{
public:
	Camera() {}
	~Camera() {}
	void initialize(Shader* shader, int width, int height, vector<GLfloat> ka, vector<GLfloat> ks, float q);
	void update();
	void mouseMovement(GLFWwindow* window, double xpos, double ypos);
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
	void viewFront();
	void viewBack();
	void viewLeft();
	void viewRight();
	void viewTop();

	bool firstMouse;
	float sensitivity;

protected:
	// Variáveis de controle da câmera
	// camera pos é um ponto e camera front é um vetor normalizado
	glm::vec3 cameraPos; // posição da camera => x, y, z
	glm::vec3 cameraFront; // vetor correspondente ao eixo de profundidade => pra frente e pra trás
	glm::vec3 cameraUp; // eixo que apontaria pra cima em relação à camera (eixo y)
	// direita e esquerda à partir da camera. Ñ precisa armazenar, usamos só no momento de mover a camera pra direita ou esquerda, calculando no momento que a tecla é pressionada
	// cameraRight é calculado pelo prosproduct do cameraFront e cameraUp versus taxa do cameraSpeed

	// Matriz de projecao perspectiva: definindo o volume de visualizacao (frustum)
	glm::mat4 projection;
	// Matriz de view: posicao e orientacao da camera
	glm::mat4 view;

	Shader* shader;

	// lastX e lastY: servem para calcular o quanto que o mouse deslocou​
	float lastX;
	float lastY;
	float pitch;
	float yaw;
};
