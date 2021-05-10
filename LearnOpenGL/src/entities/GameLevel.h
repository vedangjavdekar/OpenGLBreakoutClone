#pragma once
#include <vector>
#include "renderer/Renderer.h"
#include "Brick.h"

class GameLevel
{
public:
	// level state
	std::vector<Brick> Bricks;
	// constructor
	GameLevel()
	{
		m_HorizontalTiles = m_VerticalTiles = 0;
	}
	// loads level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	// render level
	void Render(Renderer& renderer);
	// check if the level is completed (all non-solid tiles are destroyed)
	bool IsCompleted();
private:
	int m_HorizontalTiles, m_VerticalTiles;

	// initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};