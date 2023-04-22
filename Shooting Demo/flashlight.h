#pragma once

#include <iostream>

#include "spotlight.h"

class Flashlight : public SpotLight {
private:
	static const std::string SPOT_LIGHT_UNIFORM_NAME;
	static const glm::vec3 SPOT_LIGHT_AMBIENT;
	static const glm::vec3 SPOT_LIGHT_DIFFUSE;
	static const glm::vec3 SPOT_LIGHT_SPECULAR;
	static const float SPOT_LIGHT_CUT_OFF;
	static const float SPOT_LIGHT_OUTER_CUTOFF;
	static const float STARTING_SPOT_LIGHT_CONSTANT;
	static const float STARTING_SPOT_LIGHT_LINEAR;
	static const float STARTING_SPOT_LIGHT_QUADRATIC;

public:
	Flashlight(const glm::vec3& pos, const glm::vec3& direction);
};