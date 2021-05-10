#include "GameLevel.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "ResourceManager.h"
#include "vendor/glm/glm.hpp"

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	this->Bricks.clear();

	unsigned int tileCode;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;

	if (fstream)
	{
		while (getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		m_HorizontalTiles = tileData[0].size();
		m_VerticalTiles = tileData.size();

		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
		else
		{
			std::cout << "Loading level at path \"" << file << "\" failed. File does not have correct level data." << std::endl;
		}
	}
	else
	{
		std::cout << "There was an error opening the file at path \"" << file << "\"" << std::endl;
	}


}

void GameLevel::Render(Renderer& renderer)
{
	for (Brick& tile : this->Bricks)
	{
		if (!tile.Destroyed)
		{
			tile.Render(renderer);
		}
	}
}

bool GameLevel::IsCompleted()
{
	for (Brick& tile : this->Bricks)
		if (!tile.IsSolid && !tile.Destroyed)
			return false;
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	float unit_width = levelWidth / static_cast<float>(m_HorizontalTiles);
	float unit_height = levelHeight / m_VerticalTiles;

	for (unsigned int y = 0; y < m_VerticalTiles; y++)
	{
		for (unsigned int x = 0; x < m_HorizontalTiles; x++)
		{
			

			if (tileData[y][x] == 1)
			{
				glm::vec2 pos(unit_width * x + 0.5f * unit_width, unit_height * y + 0.5f * unit_height);
				glm::vec2 size(unit_width, unit_height);

				Brick obj;
				obj.transform.Scale = size;
				obj.transform.Position = pos;
				obj.SetTexture(ResourceManager::GetTexture("block_solid"));
				obj.SetColor(glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;

				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unit_width * x + 0.5f * unit_width, unit_height * y + 0.5f * unit_height);
				glm::vec2 size(unit_width, unit_height);
				Brick obj;
				obj.transform.Position = pos;
				obj.transform.Scale = size;
				obj.SetTexture(ResourceManager::GetTexture("block"));
				obj.SetColor(color);

				this->Bricks.push_back(obj);
			}
		}
	}
}
