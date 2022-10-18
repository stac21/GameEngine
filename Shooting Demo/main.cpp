#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <irrKlang/irrKlang.h>

#include <iostream>

#include "stb_image.h"

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "window_info.h"
#include "skybox.h"
#include "spotlight.h"
#include "point_light.h"
#include "directional_light.h"
#include "moveable_entity.h"
#include "static_entity.h"
#include "aabb.h"
#include "lamp.h"
#include "sounds.h"

void process_keyboard_input(GLFWwindow* window);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float get_delta_time();

void render_skybox();
void render_quad();
void render_cube();

float last_frame_time, current_frame_time = 0.0f;

// DELETE these
RayCast ray;
glm::vec3 end_point;
glm::vec3 arrow_pos;

// scale the Camera's front unit vector by this amount for the camera ray
const float RAY_SCALE = 5.0f;

std::vector<AABB> boxes;

irrklang::ISoundEngine* sound_engine = irrklang::createIrrKlangDevice();

enum {

};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WindowInfo::width, WindowInfo::height, WindowInfo::TITLE, NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	// prevents the mouse cursor from leaving the window and hides the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Camera::last_x = WindowInfo::width / 2.0f;
	Camera::last_y = WindowInfo::height / 2.0f;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	Shader lamp_shader("..\\Shooting Demo\\Resources\\Shaders\\lamp.vs", "..\\Shooting Demo\\Resources\\Shaders\\lamp.fs");
	Shader skybox_shader("..\\Shooting Demo\\Resources\\Shaders\\skybox.vs", "..\\Shooting Demo\\Resources\\Shaders\\skybox.fs");
	Shader quad_shader("..\\Shooting Demo\\Resources\\Shaders\\quad.vs", "..\\Shooting Demo\\Resources\\Shaders\\quad.fs");
	Shader model_shader("..\\Shooting Demo\\Resources\\Shaders\\model_loading.vs", "..\\Shooting Demo\\Resources\\Shaders\\model_loading.fs");
	Shader obb_shader("..\\Shooting Demo\\Resources\\Shaders\\obb.vs", "..\\Shooting Demo\\Resources\\Shaders\\obb.fs", "..\\Shooting Demo\\Resources\\Shaders\\obb.gs");
	Shader blur_shader("..\\Shooting Demo\\Resources\\Shaders\\blur.vs", "..\\Shooting Demo\\Resources\\Shaders\\blur.fs");

	std::vector<std::string> red_grey_faces;
	red_grey_faces.push_back("..\\Shooting Demo\\Resources\\Skyboxes\\space-skybox-red-grey\\space-skybox-red-grey-right.png");
	red_grey_faces.push_back("..\\Shooting Demo\\Resources\\Skyboxes\\space-skybox-red-grey\\space-skybox-red-grey-left.png");
	red_grey_faces.push_back("..\\Shooting Demo\\Resources\\Skyboxes\\space-skybox-red-grey\\space-skybox-red-grey-top.png");
	red_grey_faces.push_back("..\\Shooting Demo\\Resources\\Skyboxes\\space-skybox-red-grey\\space-skybox-red-grey-bottom.png");
	red_grey_faces.push_back("..\\Shooting Demo\\Resources\\Skyboxes\\space-skybox-red-grey\\space-skybox-red-grey-front.png");
	red_grey_faces.push_back("..\\Shooting Demo\\Resources\\Skyboxes\\space-skybox-red-grey\\space-skybox-red-grey-back.png");

	// skybox data
	Texture skybox_cubemap(GL_TEXTURE_CUBE_MAP, Texture::DIFFUSE);

	// arrow data
	Model arrow_model("../Shooting Demo/Resources/Models/Misc/arrow.obj");
	glm::vec3 arrow_scale(0.5f);

	// zombies
	Model zombie_model("../Shooting Demo/Resources/Models/Zombies/OBJ/Zed_1.obj");
	glm::vec3 zombie_scale(1.0f);
	glm::vec3 zombie_pos(40.0f, 3.0f, 5.0f);

	// vurdalak
	Model vurdalak_model("../Shooting Demo/Resources/Models/vurdalak/vurdalak model/vurdalak_low.obj");
	glm::vec3 vurdalak_scale(3.0f);
	glm::vec3 vurdalak_pos(40.0f, 3.0f, 10.0f);

	// set up skybox
	skybox_cubemap.bind();
	skybox_cubemap.generate_cubemap(red_grey_faces, GL_RGBA);
	skybox_shader.use();
	skybox_shader.set_int("skyboxCubemap", 0);

	// set up quad
	quad_shader.use();
	quad_shader.set_int("screenTexture", 0);

	// temp: dir_light data
	std::string dir_light_uniform_name = "dirLight";
	glm::vec3 dir_light_color = glm::vec3(1.0f);
	glm::vec3 dir_light_ambient = dir_light_color * glm::vec3(0.1f);					// low influence
	glm::vec3 dir_light_diffuse = dir_light_color * glm::vec3(0.23f, 0.2f, 0.2f);		// decrease influence and make it slightly red
	glm::vec3 dir_light_specular = dir_light_color;
	glm::vec3 direction(-1.0f, 0.0f, -0.3f);

	// temp: point_light data
	std::string point_light_uniform_name = "pointLights";
	glm::vec3 point_light_color = glm::vec3(1.00f, 0.0f, 0.0f);
	glm::vec3 point_light_ambient = glm::vec3(0.5f);
	glm::vec3 point_light_diffuse = point_light_color;
	glm::vec3 point_light_specular = glm::vec3(1.0f);
	glm::vec3 ship_pos = glm::vec3(40.0f, 3.0f, 0.0f);
	glm::vec3 point_light_pos[] = {
		glm::vec3(ship_pos.x - 2.0f, ship_pos.y + 3.0f, ship_pos.z + 1.0f),
		glm::vec3(ship_pos.x + 2.0f, ship_pos.y - 3.0f, ship_pos.z - 1.0f)
	};
	float point_light_constant = 1.0f;
	float point_light_linear = .09f;
	float point_light_quadratic = 0.032f;

	// AABB data
	AABB aabbs[2] = {
		AABB(point_light_pos[0], glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f),
		AABB(point_light_pos[1], glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f),
	};

	boxes.push_back(aabbs[0]);
	boxes.push_back(aabbs[1]);

	std::cout << "box1: " << aabbs[0].get_pos().x << " " << aabbs[0].get_pos().y << " " << aabbs[0].get_pos().z << "\n";
	std::cout << "box2: " << aabbs[1].get_pos().x << " " << aabbs[1].get_pos().y << " " << aabbs[1].get_pos().z << "\n";

	// OBB data
	// OBB obb(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 2.0f, 3.0f), 1.0f, 1.0f, 1.0f);

	// temp: spot_light data
	std::string spot_light_uniform_name = "spotLight";
	glm::vec3 spot_light_ambient = glm::vec3(0.0f);
	glm::vec3 spot_light_diffuse = glm::vec3(0.5f);
	glm::vec3 spot_light_specular = glm::vec3(1.0f);
	glm::vec3 spot_light_pos = Camera::pos;
	glm::vec3 spot_light_direction = Camera::front;
	float spot_light_cut_off = glm::cos(glm::radians(12.5f));
	float spot_light_outer_cut_off = glm::cos(glm::radians(17.5f));
	float spot_light_constant = 1.0f;
	float spot_light_linear = 0.9f;
	float spot_light_quadratic = 0.032f;

	DirectionalLight dir_light(dir_light_uniform_name, dir_light_ambient, dir_light_diffuse, dir_light_specular, direction);
	PointLight point_light(point_light_uniform_name, point_light_ambient, point_light_diffuse, point_light_specular, point_light_pos[0], point_light_constant,
		point_light_linear, point_light_quadratic);
	SpotLight spot_light(spot_light_uniform_name, spot_light_ambient, spot_light_diffuse, spot_light_specular, spot_light_pos, spot_light_direction,
		spot_light_cut_off, spot_light_outer_cut_off, spot_light_constant, spot_light_linear, spot_light_quadratic);

	spot_light.print_values();

	// initialize static entities
	StaticEntity static_arrow(Camera::pos, arrow_scale, arrow_model);
	StaticEntity static_zombie(zombie_pos, zombie_scale, zombie_model);
	StaticEntity static_vurdalak(vurdalak_pos, vurdalak_scale, vurdalak_model);

	// initialize moveable entities
	MoveableEntity moveable_arrow(Camera::pos, arrow_scale, arrow_model);

	// configure depth map fbo
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depth_map_fbo;
	glGenFramebuffers(1, &depth_map_fbo);
	
	// create the cubemap for shadows
	unsigned int depth_cubemap;
	glGenTextures(1, &depth_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cubemap);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	// attach depth texture as depth_map_fbo's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// hdr framebuffer
	unsigned int hdr_fbo;
	glGenFramebuffers(1, &hdr_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);

	// color buffers for bloom
	unsigned int color_buffers[2];
	glGenTextures(2, color_buffers);

	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, color_buffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowInfo::width, WindowInfo::height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_buffers[i], 0);
	}

	// create depth buffer (renderbuffer)
	unsigned int rbo_depth_buffer;
	glGenRenderbuffers(1, &rbo_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowInfo::width, WindowInfo::height);

	unsigned int attachments[2] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1
	};
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// framebuffers for blur
	unsigned int ping_pong_fbo[2];
	unsigned int ping_pong_buffer[2];
	glGenFramebuffers(2, ping_pong_fbo);
	glGenTextures(2, ping_pong_buffer);

	for (unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, ping_pong_fbo[i]);
		glBindTexture(GL_TEXTURE_2D, ping_pong_buffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowInfo::width, WindowInfo::height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ping_pong_buffer[i], 0);
	}

	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_buffer);

	// check for framebuffer completion
	glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "hdr_fbo not complete!" << std::endl;
	}

	// shadow transforms
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	float near = 1.0f;
	float far = 25.0f;
	glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), aspect, near, far);


	// normal transforms
	glm::mat4 projection = glm::perspective(Camera::fov, (float)WindowInfo::width / WindowInfo::height,
		Camera::near_plane, Camera::far_plane);
	glm::mat4 view, skybox_view;

	glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(window)) {
		last_frame_time = current_frame_time;
		current_frame_time = (float)glfwGetTime();
		float angle = current_frame_time;
		view = glm::lookAt(Camera::pos, Camera::pos + Camera::front, Camera::up);
		skybox_view = glm::mat4(glm::mat3(view));

		// input
		process_keyboard_input(window);

		// render
		// render scene to depth cubemap
		/*glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
		glClear(GL_DEPTH_BUFFER_BIT);*/
		// configure shader and matrices
		// render scene


		// render scene as normal
		glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// should probably place this elsewhere but it's okay for now
		lamp_shader.use();

		lamp_shader.set_float("mouseX", Camera::last_x);
		lamp_shader.set_float("mouseY", Camera::last_y);

		//std::cout << "mouse_x: " << Camera::last_x << " mouse_y: " << Camera::last_y << '\n';

		// set model_shader uniforms
		model_shader.use();

		model_shader.set_mat4("projection", projection);
		glUniformMatrix4fv(glGetUniformLocation(model_shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		model_shader.set_vec3("viewPos", Camera::pos);
		model_shader.set_dir_light(dir_light);
		for (unsigned int i = 0; i < 2; i++) {
			model_shader.use();
			std::string num = std::to_string(i);

			std::string uniform_name = point_light_uniform_name + "[" + num + "]";
			point_light.set_uniform_name(uniform_name);
			point_light.set_pos(point_light_pos[i]);

			model_shader.set_point_light(point_light);

			lamp_shader.use();

			glm::mat4 model = glm::mat4();
			model = glm::translate(model, point_light_pos[i]);

			lamp_shader.set_mat4("projection", projection);
			glUniformMatrix4fv(glGetUniformLocation(lamp_shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
			lamp_shader.set_mat4("model", model);

			lamp_shader.set_vec3("lightColor", point_light_color);

			render_cube();
		}

		model_shader.use();
		model_shader.set_spot_light(spot_light);

		// create the RayCast at the player's position (subtract because front is actually in back)
		ray.set_start_point(Camera::pos);
		ray.set_direction(Camera::front);
		glm::vec3 end_point = ray.get_end_point(RAY_SCALE);

		// update the moveable entities
		//moveable_arrow.update(model_shader, end_point.x, end_point.y, end_point.z);
		//arrow_pos = moveable_arrow.get_pos();

		//std::cout << "ray.x = " << ray.get_start_point().x << " ray.y = " << ray.get_start_point().y << "\n";

		// draw static entities
		//static_arrow.draw(model_shader);
		static_vurdalak.draw(model_shader);

		// draw the obb vectors
		/*
		obb_shader.use();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		obb.draw(obb_shader);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		*/

		// skybox (must be rendered second to last)
		skybox_shader.use();
		glDepthFunc(GL_LEQUAL);

		skybox_shader.set_mat4("projection", projection);
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.ID, "view"), 1, GL_FALSE, &skybox_view[0][0]);

		glActiveTexture(GL_TEXTURE0);
		skybox_cubemap.bind();

		render_skybox();
		glDepthFunc(GL_LESS);

		// draw hud (must be rendered last)
		// not having a hud for this demo

		// blur bright fragments with two-pass Gaussian Blur
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 30;
		blur_shader.use();

		for (unsigned int i = 0; i < amount; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, ping_pong_fbo[horizontal]);
			blur_shader.set_int("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? color_buffers[1] : ping_pong_buffer[!horizontal]);
			render_quad();
			horizontal = !horizontal;
			if (first_iteration) {
				first_iteration = false;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// draw quad on the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 5.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quad_shader.use();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, color_buffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ping_pong_buffer[!horizontal]);

		quad_shader.set_float("exposure", Camera::exposure);
		quad_shader.set_int("bloom", true);

		render_quad();

		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	sound_engine->drop();
	glfwTerminate();

	return 0;
}

void process_keyboard_input(GLFWwindow* window) {
	// keyboard input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		// set the window to fullscreen
		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vid_mode = glfwGetVideoMode(primary_monitor);

		glfwSetWindowMonitor(window, primary_monitor, 0, 0, WindowInfo::width, WindowInfo::height, vid_mode->refreshRate);

		framebuffer_size_callback(window, WindowInfo::width, WindowInfo::height);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (!sound_engine->isCurrentlyPlaying(Sounds::FOOTSTEPS)) {
			sound_engine->play2D(Sounds::FOOTSTEPS);
		}
		Camera::process_keyboard_input(Camera::CAMERA_DIRECTIONS::FORWARD, get_delta_time());
	} if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (!sound_engine->isCurrentlyPlaying(Sounds::FOOTSTEPS)) {
			sound_engine->play2D(Sounds::FOOTSTEPS);
		}
		Camera::process_keyboard_input(Camera::CAMERA_DIRECTIONS::BACKWARD, get_delta_time());
	}  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (!sound_engine->isCurrentlyPlaying(Sounds::FOOTSTEPS)) {
			sound_engine->play2D(Sounds::FOOTSTEPS);
		}
		Camera::process_keyboard_input(Camera::CAMERA_DIRECTIONS::LEFT, get_delta_time());
	} if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (!sound_engine->isCurrentlyPlaying(Sounds::FOOTSTEPS)) {
			sound_engine->play2D(Sounds::FOOTSTEPS);
		}
		Camera::process_keyboard_input(Camera::CAMERA_DIRECTIONS::RIGHT, get_delta_time());
	}  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Camera::process_keyboard_input(Camera::CAMERA_DIRECTIONS::UP, get_delta_time());
	}  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Camera::process_keyboard_input(Camera::CAMERA_DIRECTIONS::DOWN, get_delta_time());
	} if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		Camera::camera_speed = Camera::BASE_CAMERA_SPEED * 2;
	} else {
		Camera::camera_speed = Camera::BASE_CAMERA_SPEED;
	} if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		Camera::exposure += 0.1f;
	} if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		Camera::exposure -= 0.1f;
	} if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		std::cout << "Camera::pos = (" << Camera::pos.x << ", " << Camera::pos.y << ", " << Camera::pos.z << ")\n";
		std::cout << "start_point = (" << ray.get_start_point().x << ", " << ray.get_start_point().y << ", " << ray.get_start_point().z << ")\n";
		std::cout << "Camera::front = (" << Camera::front.x << ", " << Camera::front.y << ", " << Camera::front.z << ")\n";
		std::cout << "end_point = (" << ray.get_end_point(RAY_SCALE).x << ", " << ray.get_end_point(RAY_SCALE).y << ", " << ray.get_end_point(RAY_SCALE).z << ")\n";
		std::cout << "arrow pos = (" << arrow_pos.x << ", " << arrow_pos.y << ", " << arrow_pos.z << ")\n\n";
	}

	// mouse input
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		if (!sound_engine->isCurrentlyPlaying(Sounds::GUNSHOT)) {
			sound_engine->play2D(Sounds::GUNSHOT, GL_FALSE);
		}
		// check for collisions
		for (int i = 0; i < boxes.size(); i++) {
			if (boxes.at(i).check_intersect(ray)) {
				std::cout << "Collision detected\n";
			}
		}
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	Camera::process_mouse_input(xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, WindowInfo::width, WindowInfo::height);
}

float get_delta_time() {
	return current_frame_time - last_frame_time;
}

unsigned int vao, vbo;
void render_skybox() {
	// if the vertex array and vertex buffer objects have not been initialized, initialize them
	if (vao == NULL) {
		float vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int quad_vao, quad_vbo;
void render_quad() {
	if (quad_vao == NULL) {
		float vertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quad_vao);
		glGenBuffers(1, &quad_vbo);

		glBindVertexArray(quad_vao);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int cube_vao, cube_vbo;
void render_cube() {
	if (cube_vao == NULL) {
		float vertices[] = {
			// positions		 // mormals	
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		glGenVertexArrays(1, &cube_vao);
		glGenBuffers(1, &cube_vbo);

		glBindVertexArray(cube_vao);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	glBindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}