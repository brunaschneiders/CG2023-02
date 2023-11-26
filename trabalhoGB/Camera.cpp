#include "Camera.h"

void Camera::initialize(Shader* shader, int width, int height)
{
	this -> shader = shader;

	cameraPos = glm::vec3(0.0, 0.0, 10.0);
	cameraFront = glm::vec3(0.0, 0.0, -1.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);

	firstMouse = true;
	lastX = width / 2.0;
	lastY = height / 2.0;
	sensitivity = 0.5;
	pitch = 0.0;
	yaw = -90.0;

	// Criando a matriz de view
	view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), cameraUp);
	shader->setMat4("view", value_ptr(view));
	
	// Criando a matriz de projecao
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	shader->setMat4("projection", glm::value_ptr(projection));

	// criando as informações de iluminação
	shader->setVec3("lightPos", 2.0f, 2.0f, 2.0f);
	shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
}

void Camera::update()
{
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	shader->setMat4("view", glm::value_ptr(view));
}

void Camera::mouseMovement(GLFWwindow* window, double xpos, double ypos)
{
	//cout << xpos << "\t" << ypos << endl;

	if (firstMouse)

	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	// //Precisamos tamb�m atualizar o cameraUp!! Pra isso, usamos o Up do  
	// //mundo (y), recalculamos Right e depois o Up
	glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0, 1.0, 0.0)));
	cameraUp = glm::normalize(glm::cross(right, cameraFront));
}

void Camera::moveForward()
{
	cameraPos += sensitivity * cameraFront;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::moveBackward()
{
	cameraPos -= sensitivity * cameraFront;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::moveLeft()
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * sensitivity;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::moveRight()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * sensitivity;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::moveUp()
{
	cameraPos += glm::vec3(0.0, 1 * sensitivity, 0.0);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::viewTop()
{
	cameraPos = glm::vec3(0.0, 10.0, 0.0);
	cameraFront = glm::vec3(0.0, -1.0, 0.0);
	cameraUp = glm::vec3(-1.0f, 0.0f, 0.0f);
}

void Camera::viewFront()
{
	cameraPos = glm::vec3(0.0, 0.0, 10.0);
	cameraFront = glm::vec3(0.0, 0.0, -1.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
}

void Camera::viewBack()
{
	cameraPos = glm::vec3(0.0, 0.0, -10.0);
	cameraFront = glm::vec3(0.0, 0.0, 1.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
}

void Camera::viewLeft()
{
	cameraPos = glm::vec3(-10.0, 0.0, 0.0);
	cameraFront = glm::vec3(1.0, 0.0, 0.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
}

void Camera::viewRight()
{
	cameraPos = glm::vec3(10.0, 0.0, 0.0);
	cameraFront = glm::vec3(-1.0, 0.0, 0.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
}
