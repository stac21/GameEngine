#include "flashlight.h"

const std::string Flashlight::SPOT_LIGHT_UNIFORM_NAME = "spotLights[0]";
const glm::vec3 Flashlight::SPOT_LIGHT_AMBIENT = glm::vec3(0.0f);
const glm::vec3 Flashlight::SPOT_LIGHT_DIFFUSE = glm::vec3(0.5f);
const glm::vec3 Flashlight::SPOT_LIGHT_SPECULAR = glm::vec3(1.0f);
const float Flashlight::SPOT_LIGHT_CUT_OFF = glm::cos(glm::radians(12.5f));
const float Flashlight::SPOT_LIGHT_OUTER_CUTOFF = glm::cos(glm::radians(17.5f));
const float Flashlight::STARTING_SPOT_LIGHT_CONSTANT = 1.0f;
const float Flashlight::STARTING_SPOT_LIGHT_LINEAR = 0.9f;
const float Flashlight::STARTING_SPOT_LIGHT_QUADRATIC = 0.032f;

Flashlight::Flashlight(const glm::vec3& pos, const glm::vec3& direction) : 
	SpotLight(SPOT_LIGHT_UNIFORM_NAME, SPOT_LIGHT_AMBIENT, SPOT_LIGHT_DIFFUSE,
		SPOT_LIGHT_SPECULAR, pos, direction,
		SPOT_LIGHT_CUT_OFF, SPOT_LIGHT_OUTER_CUTOFF, STARTING_SPOT_LIGHT_CONSTANT,
		STARTING_SPOT_LIGHT_LINEAR, STARTING_SPOT_LIGHT_QUADRATIC) {
	std::cout << "uniform_name: " << SPOT_LIGHT_UNIFORM_NAME << "\n";;
}
