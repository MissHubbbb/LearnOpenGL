#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightDirection {
public:
	LightDirection(glm::vec3 lightAngle, glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f));

	glm::vec3 lightAngles;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 lightColor;

	void UpdateDirection();
};