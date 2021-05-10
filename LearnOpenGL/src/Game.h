#pragma once
#include <vector>
#include <tuple>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "vendor/glm/glm.hpp"
#include "entities/GameLevel.h"


class Ball;

enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

using Collision = std::tuple<bool, Direction, glm::vec2>;
Direction VectorDirection(glm::vec2 target);

class Game
{
public:
	// game state
	GameState State;
	unsigned int Width, Height;

	// constructor/destructor
	Game(unsigned int width, unsigned int height);
	~Game();
	// initialize game state (load all shaders/textures/levels)
	void Init();
	// game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void DoCollisions();


private:
	std::vector<GameLevel> m_Levels;
	int m_CurrentLevel;

	bool CheckCollisions(GameObject* one, GameObject* two);
	Collision CheckCollisions(Ball* ball, GameObject* two);
	
};