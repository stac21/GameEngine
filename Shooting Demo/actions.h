#pragma once

#include <string>

class Actions {
private:
	Actions();

public:
	// interface actions
	static const std::string QUIT;
	static const std::string FULLSCREEN;

	// movement actions
	static const std::string FORWARDS;
	static const std::string BACKWARDS;
	static const std::string LEFT;
	static const std::string RIGHT;
	static const std::string UP;
	static const std::string DOWN;
	static const std::string SPRINT;

	// misc actions
	static const std::string SHOOT;
	static const std::string TOGGLE_FLASHLIGHT;
	static const std::string INCREASE_EXPOSURE;
	static const std::string DECREASE_EXPOSURE;
	static const std::string PRINT_STATS;
	static const std::string PICKUP_ITEM;
};