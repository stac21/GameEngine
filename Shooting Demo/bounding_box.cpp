#include "bounding_box.h"

const glm::vec3 BoundingBox::up(0.0f, 1.0f, 0.0f);
char* BoundingBox::MODEL_FILE_PATH = "Resources\\Models\\Lamps\\box_lamp.obj";

BoundingBox::BoundingBox(const glm::vec3& pos, const glm::vec3& front, float width, float height, float depth) :
	pos(pos), width(width), height(height), depth(depth), model(MODEL_FILE_PATH) {
	this->u = glm::normalize(front);
	this->w = glm::normalize(glm::cross(u, up));
	this->v = glm::normalize(glm::cross(u, w));
}

BoundingBox::~BoundingBox() {}

bool BoundingBox::is_point_in_box(const glm::vec3& point) {
	if (point.x < (this->pos.x + this->width) &&
		point.x > (this->pos.x - this->width) &&
		point.y < (this->pos.y + this->height) &&
		point.y > (this->pos.y - this->height) &&
		point.z < (this->pos.z + this->depth) &&
		point.z > (this->pos.z - this->depth)) {
		
		return true;
	}

	return false;
}

void BoundingBox::update(const glm::vec3& pos) {
	this->pos = pos;
}

void BoundingBox::update(const glm::vec3& pos, const glm::vec3& scale) {
	this->pos = pos;

	// width corresponds to x
	this->width *= scale.x;
	// height corresponds to y
	this->height *= scale.y;
	// depth corresponds to z
	this->depth *= scale.z;
}

bool BoundingBox::check_intersect(const RayCast& ray) {
	float t;
	bool intersects = false;

	// top plane
	t = (1 / (glm::dot(ray.get_direction(), this->w))) * (glm::dot(this->w, this->pos + glm::vec3(0.0f, this->height, 0.0f)) - glm::dot(this->w, ray.get_start_point()));

	if (t > 0 && this->is_point_in_box(ray.get_end_point(t))) {
		intersects = true;
	}

	// bottom plane
	if (!intersects) {
		t = (1 / (glm::dot(ray.get_direction(), -this->w))) * (glm::dot(-this->w, this->pos - glm::vec3(0.0f, this->height, 0.0f)) - glm::dot(-this->w, ray.get_start_point()));

		if (t > 0 && this->is_point_in_box(ray.get_end_point(t))) {
			intersects = true;
		}
	}

	// left plane
	if (!intersects) {
		t = (1 / (glm::dot(ray.get_direction(), this->u))) * (glm::dot(this->u, this->pos + glm::vec3(this->width, 0.0f, 0.0f)) - glm::dot(this->u, ray.get_start_point()));

		if (t > 0 && this->is_point_in_box(ray.get_end_point(t))) {
			intersects = true;
		}
	}

	// right plane
	if (!intersects) {
		t = (1 / (glm::dot(ray.get_direction(), -this->u))) * (glm::dot(-this->u, this->pos - glm::vec3(this->width, 0.0f, 0.0f)) - glm::dot(-this->u, ray.get_start_point()));

		if (t > 0 && this->is_point_in_box(ray.get_end_point(t))) {
			intersects = true;
		}
	}

	// front plane
	if (!intersects) {
		t = (1 / (glm::dot(ray.get_direction(), this->v))) * (glm::dot(this->v, this->pos + glm::vec3(0.0f, 0.0f, this->depth)) - glm::dot(this->v, ray.get_start_point()));

		if (t > 0 && this->is_point_in_box(ray.get_end_point(t))) {
			intersects = true;
		}
	}

	// back plane
	if (!intersects) {
		t = (1 / (glm::dot(ray.get_direction(), -this->v))) * (glm::dot(-this->v, this->pos - glm::vec3(0.0f, 0.0f, this->depth)) - glm::dot(-this->v, ray.get_start_point()));

		if (t > 0 && this->is_point_in_box(ray.get_end_point(t))) {
			intersects = true;
		}
	}

	std::cout << "end point: " << ray.get_end_point(t).x << " " << ray.get_end_point(t).y << " " << ray.get_end_point(t).z << "\n";

	return intersects;
}

// getters
glm::vec3 BoundingBox::get_pos() const {
	return this->pos;
}


void BoundingBox::draw(const Shader& shader) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->model.draw(shader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}