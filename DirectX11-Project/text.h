#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "textdata.h"

class Text
{
public:
	Text(int entityId)
		: m_entityId(entityId)
	{
	}
	Text(const Text&) = default;
	~Text() = default;

	int GetEntityId();

	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);
	void SetText(std::string text);
	void SetCharacterWidth(int width);
	void SetCharacterHeight(int height);
	void SetTint(float red, float green, float blue, float alpha);

	const TextData& GetTextData();

public:
	static const ComponentId ID;

private:
	int m_entityId;
	TextData m_textData;
};