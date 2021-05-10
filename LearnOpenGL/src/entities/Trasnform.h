#pragma once
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

struct Transform {
	glm::vec2 Position;
	float Rotation;
	glm::vec2 Scale;

	glm::vec2 Pivot;

	Transform()
		:Position(0.0f, 0.0f), Rotation(0.0f), Scale(50.0f, 50.0f), Pivot(0.5f, 0.5f)
	{
	}

	glm::mat4 EvaluateModelMat() {
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(Position, 0.0f));
		model = glm::translate(model, glm::vec3(-Pivot.x * Scale.x, -Pivot.y * Scale.y, 0.0f));

		model = glm::translate(model, glm::vec3(Pivot.x * Scale.x, Pivot.y * Scale.y, 0.0f));
		model = glm::rotate(model, glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-Pivot.x * Scale.x, -Pivot.y * Scale.y, 0.0f));

		model = glm::scale(model, glm::vec3(Scale, 1.0f));
		return model;
	}
};