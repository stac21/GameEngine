#include "player.h"

const int Player::STARTING_HP = 50;
const int Player::ITEM_PICKUP_RAY_SCALE = 5.0f;

Player::Player() : 
	Character(Camera::pos, Camera::front, STARTING_HP), flashlight(Character::pos, Character::direction) {}

// getters
SpotLight Player::get_flashlight() const {
	return this->flashlight;
}

// setters
void Player::set_pos(const glm::vec3& pos) {
	Character::set_pos(pos);

	this->flashlight.set_pos(pos);
}

void Player::set_direction(const glm::vec3& direction) {
	Character::set_direction(direction);

	this->flashlight.set_direction(direction);
}

void Player::toggle_flashlight() {
	this->flashlight.toggle();
}

glm::vec3 Player::get_ray_item_pickup_end_point() {
	return ray.get_end_point(ITEM_PICKUP_RAY_SCALE);
}