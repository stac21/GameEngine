#include "enemy.h"

void Enemy::die() {
	std::cout << "This enemy died";
}

Enemy::Enemy(const std::string& name, const glm::vec3& pos, const glm::vec3& direction,
	int starting_hp, const glm::vec3& scale, const glm::vec3& rotaton_axis,
	float rotation_angle, const Model& model) :
	Character(pos, direction, starting_hp), MoveableEntity(pos, scale, rotation_axis, rotation_angle, model) {

	this->name = name;

}