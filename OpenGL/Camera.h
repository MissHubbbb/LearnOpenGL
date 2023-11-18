#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	//�����һЩ����
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//ŷ����
	float Yaw;
	float Pitch;

	//���ѡ��
	float MovementSpeed;	//�ƶ��ٶ�
	float MouseSensitivity;	//���������
	float Zoom;		//fov	

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
		Position = position;
		WorldUp = worldUp;
		Yaw = yaw;
		//Yaw = 90.0f;	//�ı�Z�ᳯ��
		//Pitch = pitch;
		Pitch = -89.9f;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}	
	//�õ���ͼ����
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);			
	}	

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;

		//Position.y = 0.0f;		//����̶���xzƽ�棬����y��
	}

	//�����������룬��������ŷ���ǵĽǶȷ�Χ
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		updateCameraVectors();
	}

	//���������ֵĺ���
	void ProcessMouseScroll(float yoffset) {
		if (Zoom >= 1.0f && Zoom <= 89.0f)
			Zoom -= (float)yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 89.0f)
			Zoom = 89.0f;
	}

private:
	//�����������������������ȫ������Ҫ��һ����
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		Front = glm::normalize(front);
		//�����¼�����������������
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif