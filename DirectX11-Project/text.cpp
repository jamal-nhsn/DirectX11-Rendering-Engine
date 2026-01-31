#include "pch.h"
#include "text.h"

const ComponentId Text::ID = ComponentId::Text;

int Text::GetEntityId()
{
	return m_entityId;
}

void Text::SetShader(Shader* shader)
{
	m_textData.shader = shader;
}

void Text::SetTexture(Engine::Texture2D* texture)
{
	m_textData.texture = texture;
}

void Text::SetText(std::string text)
{
	m_textData.text = text;
}

void Text::SetCharacterWidth(int width)
{
	m_textData.characterDimensions.x = static_cast<float>(width);
}

void Text::SetCharacterHeight(int height)
{
	m_textData.characterDimensions.y = static_cast<float>(height);
}

void Text::SetTint(float red, float green, float blue, float alpha)
{
	m_textData.tint = { red, green, blue, alpha };
}

const TextData& Text::GetTextData()
{
	return m_textData;
}