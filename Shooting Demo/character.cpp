#include "character.h"

Character::Character(const glm::vec3& pos, const glm::vec3& direction, 
	int starting_hp) : pos(pos), direction(direction), hp(starting_hp), ray(pos, direction) {

	this->pos = pos;
	this->direction = direction;
	this->hp = starting_hp;
}

Character::~Character() {
}

// getters
glm::vec3 Character::get_pos() const {
	return this->pos;
}

glm::vec3 Character::get_direction() const {
	return this->direction;
}

int Character::get_hp() const {
	return this->hp;
}

RayCast Character::get_ray() const {
	return this->ray;
}


// setters
void Character::set_pos(const glm::vec3& pos) {
	this->pos = pos;

	this->ray.set_start_point(this->pos);
}

void Character::set_direction(const glm::vec3& direction) {
	this->direction = direction;

	this->ray.set_direction(this->direction);
}

void Character::inflict_damage(int damage_points) {
	this->hp -= damage_points;
}

bool Character::is_alive() const {
	return this->hp > 0;
}