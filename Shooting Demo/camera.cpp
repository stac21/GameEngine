#include "camera.h"

float Camera::camera_speed = BASE_CAMERA_SPEED;
float Camera::sensitivity = 0.05f;
float Camera::fov = glm::radians(45.0f);
float Camera::pitch = 0.0f;
float Camera::yaw = -90.0f;
float Camera::last_x = 0.0f;
float Camera::last_y = 0.0f;
float Camera::near_plane = 0.1f;
float Camera::far_plane = 300.0f;
float Camera::exposure = 0.5f;
const float Camera::PITCH_LIMIT = 89.0f;
const float Camera::BASE_CAMERA_SPEED = 4.0f;

glm::vec3 Camera::pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);

void Camera::process_keyboard_input(CAMERA_DIRECTIONS direction, float delta_time) {
	float distance = camera_speed * delta_time;

	switch (direction) {
	case FORWARD: {
		glm::vec3 right = glm::normalize(glm::cross(front, up));
		glm::vec3 movement_dir = glm::normalize(glm::cross(up, right));
		pos += distance * movement_dir;
	}
				  break;
	case BACKWARD: {
		glm::vec3 right = glm::normalize(glm::cross(front, up));
		glm::vec3 movement_dir = glm::normalize(glm::cross(up, right));
		pos -= distance * movement_dir;
	}
				   break;
	case LEFT:
		pos -= distance * glm::normalize(glm::cross(front, up));
		break;
	case RIGHT:
		pos += distance * glm::normalize(glm::cross(front, up));
		break;
	case UP:
		pos += distance * up;
		break;
	case DOWN:
		pos -= distance * up;
		break;
	}
}

void Camera::process_mouse_input(double xpos, double ypos) {
	float x_offset = sensitivity * ((float)xpos - last_x);
	float y_offset = sensitivity * (last_y - (float)ypos);

	last_x = xpos;
	last_y = ypos;

	yaw += x_offset;
	pitch += y_offset;

	if (pitch > PITCH_LIMIT) {
		pitch = 89.0f;
	} else if (pitch < -PITCH_LIMIT) {
		pitch = -89.0f;
	}

	glm::vec3 vec;
	vec.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	vec.y = sin(glm::radians(pitch));
	vec.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	front = glm::normalize(vec);
}