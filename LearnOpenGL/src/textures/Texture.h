#pragma once
#include "IGeneratableAsset.h"

class Texture:public GeneratableAsset<Texture>
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture();
	~Texture();

	inline unsigned int GetTextureID() const
	{
		return m_RendererID;
	}

	Texture& Generate(const std::string& filepath) override;
	Texture& Generate(unsigned int width,unsigned int height);
	void RemoveTexture();
	void Bind(unsigned int slot = 0) const;
	void Unbind();


	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};

