#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"


class Material {
public:
	Material(Shader* shader, unsigned int _diffuse, unsigned int _specular, unsigned int _emission, glm::vec3 ambient, float _shininess);

	Shader *shader;
	unsigned int diffuse;
	unsigned int specular;
	unsigned int emission;
	glm::vec3 ambient;
	float shininess;
};

