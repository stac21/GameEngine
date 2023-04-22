#pragma once

#include <vector>

#include "point_light.h"
#include "spotlight.h"

class Chunk {
private:	
	std::vector<PointLight> point_lights;
	std::vector<SpotLight> spot_lights;

public:
	// getters
	std::vector<PointLight> get_point_lights() const;
	std::vector<SpotLight> get_spot_lights() const;

	
};