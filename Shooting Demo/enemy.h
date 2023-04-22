#pragma once

#include "moveable_entity.h"
#include "character.h"
#include "bounding_box.h"

class Enemy : public Character, public MoveableEntity {
private:
	std::string name;

public:
	Enemy(const std::string& name, const glm::vec3& pos, const glm::vec3& direction, int starting_hp,
		const glm::vec3& scale, const glm::vec3& rotaton_axis, float rotation_angle, const Model& model);

	virtual void die();
};