#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

Camera::Camera(vec3 position, float pitch, float yaw, vec3 worldUp) {
	Position = position;
	WorldUp = worldUp;
	Yaw = yaw;
	Pitch = pitch;

	UpdateCameraVectors();
	FirstMouse = true;
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
	Position = position;
	WorldUp = worldUp;
	Front = target - position;

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Front, Right));
	FirstMouse = true;
}

void Camera::MoveCamera(glm::vec2 moveDirection) {
	Position += Front * moveDirection.y * MoveSpeed;
	Position += glm::normalize(glm::cross(Front, WorldUp)) * moveDirection.x * MoveSpeed;
}

void Camera::MoveCamera(glm::vec2 moveDirection, float speed) {
	Position += Front * moveDirection.y * speed;
	Position += glm::normalize(glm::cross(Front, WorldUp)) * moveDirection.x * speed;
}

void Camera::UpdateCameraVectors() {
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	// also re-calculate the Right and Up vector
	// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Right = glm::normalize(glm::cross(Front, WorldUp));  
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::RotateCamera(float currentMousePosX, float currentMousePosY) {
	float deltaPosX, deltaPosY;

	if (FirstMouse) {
		LastMousePosX = currentMousePosX;
		LastMousePosY = currentMousePosY;
		FirstMouse = false;
		return;
	}

	deltaPosX = currentMousePosX - LastMousePosX;
	deltaPosY = LastMousePosY - currentMousePosY;

	LastMousePosX = currentMousePosX;
	LastMousePosY = currentMousePosY;

	Yaw += deltaPosX * MouseSensitive;
	Pitch += deltaPosY * MouseSensitive;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	UpdateCameraVectors();
}

void Camera::ResetFirstMouse() {
	this->FirstMouse = true;
}