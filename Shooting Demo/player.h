#pragma once

#include "spotlight.h"
#include "camera.h"
#include "character.h"
#include "flashlight.h"

class Player : public Character {
private:
	static const int STARTING_HP;
	static const int ITEM_PICKUP_RAY_SCALE;

	Flashlight flashlight;
		
public:
	Player();

	// getters
	SpotLight get_flashlight() const;

	// setters
	void set_pos(const glm::vec3& pos);
	void set_direction(const glm::vec3& direction);

	void toggle_flashlight();

	glm::vec3 get_ray_item_pickup_end_point();
};