#include <iostream>
#include "Game.h"
#include "ResourceManager.h"
#include <vector>
#include <ctime>
#include "glm/glm.hpp"
#include "inputs/KeyBoardInput.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "renderer/Renderer.h"
#include "entities/Sprite.h"
#include "entities/Ball.h"
#include "entities/Paddle.h"
#include "renderer/ParticleRenderer.h"
#include "entities/particles/ParticleEmitter.h"
#include "textures/RenderTexture.h"

Renderer* renderer;
ParticleRenderer* p_renderer;
ParticleEmitter* particleEmitter;
RenderTexture* renderTexture;

Sprite* background;
Ball* ball;
Paddle* paddle;

const glm::vec2 BALL_INITIAL_VELOCITY = glm::vec2(200.0f, -60.0f);
const float PADDLE_VELOCITY = 200.0f;

Game::Game(unsigned int width, unsigned int height)
	: State(GameState::GAME_ACTIVE), Width(width), Height(height), m_CurrentLevel(-1)
{
	srand((unsigned)time(0));
}

Game::~Game()
{

}

void Game::Init()
{
	// LOAD SHADERS
	ResourceManager::LoadShader("sprite", "assets/shaders/basic.shader");
	ResourceManager::LoadShader("particles", "assets/shaders/particles.shader");
	ResourceManager::LoadShader("renderTexture", "assets/shaders/renderTexture.shader");

	// CONFIGURE SHADERS
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	glm::mat4 view(1.0f);

	Shader shader = ResourceManager::GetShader("sprite");
	shader.Bind();
	shader.SetUniform1i("image", 0);
	shader.SetUniformMat4f("projection", projection);
	shader.SetUniformMat4f("view", view);

	shader = ResourceManager::GetShader("particles");
	shader.Bind();
	shader.SetUniform1i("image", 0);
	shader.SetUniformMat4f("projection", projection);
	shader.SetUniformMat4f("view", view);


	// LOAD TEXTURES
	ResourceManager::LoadTexture("white", "assets/textures/white.png");
	ResourceManager::LoadTexture("ball", "assets/textures/face.png");
	ResourceManager::LoadTexture("background", "assets/textures/background.jpg");
	ResourceManager::LoadTexture("block", "assets/textures/block.png");
	ResourceManager::LoadTexture("block_solid", "assets/textures/block_solid.png");
	ResourceManager::LoadTexture("paddle", "assets/textures/paddle.png");
	ResourceManager::LoadTexture("particle", "assets/textures/particle.png");

	// LOAD LEVELS
	GameLevel level;
	level.Load("assets/levels/Level1.level", this->Width, this->Height / 2);
	m_Levels.push_back(level);
	m_CurrentLevel = 0;

	// RENDERERS
	renderer = new Renderer(ResourceManager::GetShader("sprite"));
	p_renderer = new ParticleRenderer(ResourceManager::GetShader("particles"));

	// RENDER TEXTURE
	renderTexture = new RenderTexture(Width, Height);
	renderTexture->SetShader(ResourceManager::GetShader("renderTexture"));

	particleEmitter = new ParticleEmitter(*p_renderer);
	particleEmitter->ParticleTexture = ResourceManager::GetTexture("particle");
	particleEmitter->AlphaOverLifeTime = glm::vec2(0.0f, 1.0f);
	particleEmitter->ScaleOverLifeTime = glm::vec2(0.0f, 1.0f);
	particleEmitter->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0);
	particleEmitter->Looping = true;
	particleEmitter->Duration = 2.0f;
	particleEmitter->LifeTime = 1.0f;
	particleEmitter->SpawnRate = 0.05f;
	particleEmitter->InitialScale = 20.0f;
	particleEmitter->transform.Position = glm::vec2(Width / 2.0f, Height / 2.0f);
	particleEmitter->RandomizePosition = true;


	background = new Sprite(ResourceManager::GetTexture("background"));
	background->transform.Scale = glm::vec2(Width, Height);
	background->transform.Position = glm::vec2(0.5f * Width, 0.5f * Height);

	ball = new Ball(ResourceManager::GetTexture("ball"), 20.0f);
	ball->transform.Position = glm::vec2(0.5f * Width, 0.9f * Height);
	ball->Velocity = BALL_INITIAL_VELOCITY;

	paddle = new Paddle(ResourceManager::GetTexture("paddle"));
	paddle->transform.Scale = glm::vec2(100.0f, 20.0f);
	paddle->transform.Position = glm::vec2(0.5f * Width, Height - 10.0f);

}



void Game::Update(float dt)
{
	if (State == GameState::GAME_ACTIVE)
	{
		//particleEmitter->Color = glm::vec4((float)(rand() % 255) / 255.0f, (float)(rand() % 255) / 255.0f, (float)(rand() % 255) / 255.0f, 1.0f);
		paddle->Update(dt);
		paddle->ClampXPosition(0, Width);
		if (ball->IsStuck)
		{
			ball->transform.Position = paddle->transform.Position - glm::vec2(0, paddle->transform.Scale.y * 0.5 + ball->Radius);
		}
		else
		{
			ball->Move(dt);
			ball->ReflectBallOnBounds(Width, Height);
			DoCollisions();

			if (ball->transform.Position.y >= Height)
			{
				ball->Reset(glm::vec2(0.5f * Width, Height - ball->Radius), BALL_INITIAL_VELOCITY);
				paddle->Reset(glm::vec2(0.5f * Width, Height - paddle->transform.Scale.y * 0.5f), glm::vec2(0.0f));
				ball->IsStuck = true;
			}
		}

		if (KeyboardInput::Keys[GLFW_KEY_SPACE] && ball->IsStuck)
		{
			ball->IsStuck = false;
		}


		particleEmitter->transform.Position = ball->transform.Position;
		particleEmitter->Velocity = -0.1f * ball->Velocity;
		particleEmitter->Update(dt);

	}
}

void Game::ProcessInput(float dt)
{
	if (State == GameState::GAME_ACTIVE)
	{
		if (KeyboardInput::Keys[GLFW_KEY_A])
		{
			paddle->Velocity.x = -PADDLE_VELOCITY;
		}
		else if (KeyboardInput::Keys[GLFW_KEY_D])
		{
			paddle->Velocity.x = PADDLE_VELOCITY;
		}
	}
}

void Game::Render()
{

	if (State == GameState::GAME_ACTIVE)
	{
		renderTexture->BeginRender();

		background->Render(*renderer);
		this->m_Levels[this->m_CurrentLevel].Render(*renderer);
		paddle->Render(*renderer);
		particleEmitter->Render();
		ball->Render(*renderer);

		renderTexture->EndRender();

		renderTexture->Display();
	}
}

void Game::DoCollisions()
{
	for (Brick& brick : m_Levels[m_CurrentLevel].Bricks)
	{
		if (!brick.Destroyed)
		{
			auto [collided, direction, diff_vector] = CheckCollisions(ball, &brick);
			if (collided)
			{
				if (!brick.IsSolid)
				{
					brick.Destroyed = true;
				}
				if (direction == Direction::LEFT || direction == Direction::RIGHT) // horizontal collision
				{
					ball->Velocity.x = -ball->Velocity.x; // reverse horizontal velocity
					// relocate
					float penetration = ball->Radius - std::abs(diff_vector.x);
					if (direction == Direction::LEFT)
						ball->transform.Position.x += penetration; // move ball to right
					else
						ball->transform.Position.x -= penetration; // move ball to left
				}
				else // vertical collision
				{
					ball->Velocity.y = -1.0f * abs(ball->Velocity.y); ; // reverse vertical velocity
					// relocate
					float penetration = ball->Radius - std::abs(diff_vector.y);
					if (direction == Direction::UP)
						ball->transform.Position.y -= penetration; // move ball back up
					else
						ball->transform.Position.y += penetration; // move ball back down
				}
			}
		}
	}
	auto [collided, direction, diff_vector] = CheckCollisions(ball, paddle);

	if (!ball->IsStuck && collided)
	{
		// check where it hit the board, and change velocity based on where it hit the board
		float distance = ball->transform.Position.x - paddle->transform.Position.x;
		float percentage = distance / (paddle->transform.Scale.x / 2.0f);

		// then move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = ball->Velocity;
		ball->Velocity.x = BALL_INITIAL_VELOCITY.x * percentage * strength;
		ball->Velocity.y = -ball->Velocity.y;
		ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
	}
}

bool Game::CheckCollisions(GameObject* one, GameObject* two)
{
	glm::vec2 diff = two->transform.Position - one->transform.Position;
	bool y_overlap = std::abs(diff.y) <= (one->transform.Scale.y * 0.5f + two->transform.Scale.y * 0.5f);
	bool x_overlap = std::abs(diff.x) <= (one->transform.Scale.x * 0.5f + two->transform.Scale.x * 0.5f);
	return x_overlap && y_overlap;
}


Collision Game::CheckCollisions(Ball* ball, GameObject* two) // AABB - Circle collision
{
	// get center point circle first 
	glm::vec2 center(ball->transform.Position);
	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two->transform.Scale.x * 0.5f, two->transform.Scale.y * 0.5f);
	glm::vec2 aabb_center(
		two->transform.Position.x,
		two->transform.Position.y
	);
	// get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	if (glm::length(difference) <= ball->Radius)
	{
		return { true,VectorDirection(difference), difference };
	}
	else {

		return { false,Direction::UP ,glm::vec2(0.0f) };
	}
}


Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}
