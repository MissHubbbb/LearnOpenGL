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
	//相机的一些属性
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//欧拉角
	float Yaw;
	float Pitch;

	//相机选项
	float MovementSpeed;	//移动速度
	float MouseSensitivity;	//鼠标灵敏度
	float Zoom;		//fov	

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
		Position = position;
		WorldUp = worldUp;
		Yaw = yaw;
		//Yaw = 90.0f;	//改变Z轴朝向
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
	//得到视图矩阵
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

		//Position.y = 0.0f;		//将其固定在xz平面，锁定y轴
	}

	//处理鼠标的输入，并且限制欧拉角的角度范围
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

	//处理鼠标滚轮的函数
	void ProcessMouseScroll(float yoffset) {
		if (Zoom >= 1.0f && Zoom <= 89.0f)
			Zoom -= (float)yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 89.0f)
			Zoom = 89.0f;
	}

private:
	//更新摄像机的三个轴向量（全部都需要归一化）
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		Front = glm::normalize(front);
		//再重新计算右向量和上向量
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif