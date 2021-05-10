#pragma once
#include <string>

template<typename T>
class GeneratableAsset {
public:
	virtual T& Generate(const std::string& filepath) = 0;
};