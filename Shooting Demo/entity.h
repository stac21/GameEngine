#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "model.h"
#include "shader.h"
#include "obb.h"

// TOOD add a boundingbox 
class Entity {
protected:
	glm::vec3 pos, scale, rotation_axis;
	float rotation_angle;
	Model model;
	glm::mat4 model_matrix;

	// the only necessary data for constructing a model matrix is the pos, so only require that and make the other vectors optional 
	// (they will not be factored into calculation if not passed in)
	Entity(const glm::vec3& pos, const Model& model);
	Entity(const glm::vec3& pos, const glm::vec3& scale, const Model& model);
	Entity(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model);
	Entity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model);
	virtual ~Entity();

	// should_scale and should_rotate are used to indicate whether the entity is rotating or not to avoid unnecessary calculation expenses
	void update_model_matrix(bool should_scale, bool should_rotate);
public:
	// setters
	void set_model(const Model& model);

	// getters
	glm::vec3 get_pos() const;
	float get_x_pos() const;
	float get_y_pos() const;
	float get_z_pos() const;
	glm::vec3 get_scale() const;
	glm::vec3 get_rotation_axis() const;
	float get_rotation_angle() const;
	glm::mat4 get_model_matrix() const;
};
