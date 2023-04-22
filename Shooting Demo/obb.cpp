#include "obb.h"

const glm::vec3 OBB::up(0.0f, 1.0f, 0.0f);
// TODO temporary model. REPLACE
//const Model OBB::model("../Shooting Demo/Resources/Models/Misc/arrow.obj");

OBB::OBB(const glm::vec3& pos, const glm::vec3& front, float width, float height, float depth, const glm::vec3& rotation_axis, float rotation_angle) :
	pos(pos), width(width), height(height), depth(depth), rotation_axis(rotation_axis), rotation_angle(rotation_angle) {
	this->u = glm::normalize(front);
	this->w = glm::normalize(glm::cross(u, up));
	this->v = glm::normalize(glm::cross(u, w));
}

/*
void OBB::update_model_matrix(bool should_scale, bool should_rotate) {
	this->model_matrix = glm::mat4();
	this->model_matrix = glm::translate(this->model_matrix, this->pos);

	if (should_rotate) {
		this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation_angle), this->rotation_axis);
	}
	if (should_scale) {
		this->model_matrix = glm::scale(this->model_matrix, this->scale);
	}
}
*/

void OBB::draw(const Shader& shader) {
	if (this->vao == NULL) {
		float points[] = {
			pos.x, pos.y, pos.z
		};

		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vbo);

		glBindVertexArray(this->vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	shader.set_vec3("u", this->u);
	shader.set_vec3("w", this->w);
	shader.set_vec3("v", this->v);

	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINT, 0, 1);
	glBindVertexArray(0);
}



// setters
void OBB::set_pos(const glm::vec3& pos) {
	this->pos = pos;
}

void OBB::set_model_matrix(const glm::mat4& model_matrix) {
	this->model_matrix = model_matrix;
}

// getters
glm::vec3 OBB::get_pos() const {
	return this->pos;
}

glm::vec3 OBB::get_scale() const {
	return this->scale;
}

glm::vec3 OBB::get_rotation_axis() const {
	return this->rotation_axis;
}

float OBB::get_rotation_angle() const {
	return this->rotation_angle;
}

glm::mat4 OBB::get_model_matrix() const {
	return this->model_matrix;
}