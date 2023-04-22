#pragma once

#include <glm/glm.hpp>

#include "ray_cast.h"

class Character {
protected:
	glm::vec3 pos;
	glm::vec3 direction;
	RayCast ray;

	int hp;

public:
	Character(const glm::vec3& pos, const glm::vec3& direction, int starting_hp);
	virtual ~Character();

	// getters
	glm::vec3 get_pos() const;
	glm::vec3 get_direction() const;
	int get_hp() const;
	RayCast get_ray() const;

	// setters
	virtual void set_pos(const glm::vec3& pos);
	virtual void set_direction(const glm::vec3& direction);

	void inflict_damage(int damage_points);
	bool is_alive() const;
};