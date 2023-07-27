#include "Material.h"

Material::Material(Shader* shader, unsigned int _diffuse, unsigned int _specular, unsigned int _emission, glm::vec3 _ambient, float _shininess) :
	shader(shader), diffuse(_diffuse), specular(_specular), emission(_emission), ambient(_ambient), shininess(_shininess) {

}