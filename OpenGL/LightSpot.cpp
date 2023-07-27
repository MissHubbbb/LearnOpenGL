#include "LightSpot.h"

LightSpot::LightSpot(glm::vec3 position, glm::vec3 lightAngle, glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f)):position(position),lightAngles(lightAngle),lightColor(lightColor)
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;

	UpdateDirection();
}

void LightSpot::UpdateDirection() {
	direction = glm::vec3(0, 0, 1.0f);
	direction = glm::rotateX(direction, lightAngles.x);
	direction = glm::rotateY(direction, lightAngles.y);
	direction = glm::rotateZ(direction, lightAngles.z);

	direction *= -1.0f;
}