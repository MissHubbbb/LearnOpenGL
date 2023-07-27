#include "LightPoint.h"

LightPoint::LightPoint(glm::vec3 position, glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f)) :position(position), lightColor(lightColor) {
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
}