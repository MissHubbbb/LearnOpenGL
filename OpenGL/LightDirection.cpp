#include "LightDirection.h"

LightDirection::LightDirection(glm::vec3 lightAngle, glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f)):lightAngles(lightAngle),lightColor(lightColor)
{
	UpdateDirection();
}

void LightDirection::UpdateDirection() {
	direction = glm::vec3(0, 0, 1.0f);
	direction = glm::rotateX(direction, lightAngles.x);
	direction = glm::rotateY(direction, lightAngles.y);
	direction = glm::rotateZ(direction, lightAngles.z);

	direction *= -1.0f;
}