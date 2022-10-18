#pragma once

#include <glm/glm.hpp>

#include "model.h"
#include "ray_cast.h"
#include "shader.h"

class OBB {
private:
	static const glm::vec3 up;
	glm::vec3 pos, scale, rotation_axis;
	float rotation_angle;
	glm::mat4 model_matrix;
	glm::vec3 u, v, w;
	unsigned int vao, vbo;

	float width, height, depth;

	//void update_model_matrix(bool should_scale, bool should_rotate);
public:
	OBB(const glm::vec3& pos, const glm::vec3& front, float width, float height, float depth, const glm::vec3& rotation_axis, float rotation_angle);
	void draw(const Shader& shader);

	void update(const glm::vec3& pos);
	void update(const glm::vec3& pos, const glm::vec3& scale);
	void update(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle);
	void update(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle);

	bool check_intersect(RayCast ray);

	// setters
	void set_pos(const glm::vec3& pos);
	void set_model_matrix(const glm::mat4& model_matrix);

	// getters
	glm::vec3 get_pos() const;
	glm::vec3 get_scale() const;
	glm::vec3 get_rotation_axis() const;
	float get_rotation_angle() const;
	glm::mat4 get_model_matrix() const;
};