#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightSpot {
public:
	LightSpot(glm::vec3 position, glm::vec3 lightAngle, glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f));

	glm::vec3 position;
	glm::vec3 direction = glm::vec3(0, 0, 1.0f);
	glm::vec3 lightAngles;
	glm::vec3 lightColor;
	float cosInCut = 0.9f;
	float cosOutCut = 0.85f;
	
	float constant;
	float linear;
	float quadratic;

	void UpdateDirection();
};