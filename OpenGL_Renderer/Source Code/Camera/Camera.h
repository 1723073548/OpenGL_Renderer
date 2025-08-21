#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public: // Constructor
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp);
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

public: // Public-Fields
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float MouseSensitive = 0.02f;
	float MoveSpeed = 0.01f;

private: // Private-Fields
	bool FirstMouse;
	float LastMousePosX;
	float LastMousePosY;
	float Yaw;
	float Pitch;

public: // Public-Methods
	void MoveCamera(glm::vec2 moveDirection);
	void MoveCamera(glm::vec2 moveDirection, float speed);
	void RotateCamera(float currentMousePosX, float currentMousePosY);
	void ResetFirstMouse();
	void UpdateCameraVectors();
	glm::mat4 GetViewMatrix();
};