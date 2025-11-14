#include "spriteanimationloader.h"

const char* SpriteAnimationLoader::s_delimiters = ",() \n:\t";

SpriteAnimationLoader::SpriteAnimationLoader()
{
}

SpriteAnimationLoader::SpriteAnimationLoader(const SpriteAnimationLoader& other)
{
}

SpriteAnimationLoader::~SpriteAnimationLoader()
{
}

std::vector<SpriteAnimationFrame>* SpriteAnimationLoader::LoadSpriteAnimation(const char* filePath, TextureManager* textureManager)
{
	std::vector<SpriteAnimationFrame>* spriteAnimation = new std::vector<SpriteAnimationFrame>;

	// Open the file for reading.
	FILE* filePtr;
	int error = 0;
	error = fopen_s(&filePtr, filePath, "r");
	if (error != 0) {
		delete spriteAnimation;
		return 0;
	}

	// Tokenize input and read in data.
	char line[128];
	while (fgets(line, sizeof(line), filePtr) != 0) {
		char* data = &line[0];
		char* label = strtok_s(data, s_delimiters, &data);
		if (!label) {
			continue;
		}

		if (strcmp(label, "#FRAME") == 0) {
			LoadFrame(spriteAnimation, filePtr, textureManager);
		}
	}

	// Close the file
	fclose(filePtr);

	// File contained no frames.
	if (spriteAnimation->size() == 0) {
		delete spriteAnimation;
		return 0;
	}

	return spriteAnimation;
}

void SpriteAnimationLoader::LoadFrame(std::vector<SpriteAnimationFrame>* spriteAnimation, FILE* filePtr, TextureManager* textureManager)
{
	// Get default values in case some fields are left out.
	size_t count = 1;
	std::tuple<int, int> slide = std::make_tuple(0, 0);
	std::shared_ptr<Engine::Texture2D> texture = 0;
	std::tuple<int, int> dimensions = std::make_tuple(0, 0);
	std::tuple<int, int> sourceOrigin = std::make_tuple(0, 0);
	float duration = 1.0f;

	// Tokenize input and read in data.
	char line[128];
	long prevLine = ftell(filePtr);
	while (fgets(line, sizeof(line), filePtr) != 0) {
		char* data = &line[0];
		char* field = strtok_s(data, s_delimiters, &data);
		if (!field) {
			continue;
		}

		if (strcmp(field, "Count") == 0) {
			LoadCount(data, count);
		}
		else if (strcmp(field, "Slide") == 0) {
			LoadSlide(data, slide);
		}
		else if (strcmp(field, "Texture") == 0) {
			LoadTexture(data, textureManager, texture);
		}
		else if (strcmp(field, "Dimensions") == 0) {
			LoadDimensions(data, dimensions);
		}
		else if (strcmp(field, "Source_Origin") == 0) {
			LoadSourceOrigin(data, sourceOrigin);
		}
		else if (strcmp(field, "Duration") == 0) {
			LoadDuration(data, duration);
		}
		else if (strcmp(field, "#FRAME") == 0) {
			// Encountered next frame, set back to previous line and stop.
			fseek(filePtr, prevLine, SEEK_SET);
			break;
		}

		// Save previous line location.
		prevLine = ftell(filePtr);
	}

	SpriteAnimationFrame frame;
	frame.texture = texture;
	frame.width = std::get<0>(dimensions);
	frame.height = std::get<1>(dimensions);
	frame.sourceX = std::get<0>(sourceOrigin);
	frame.sourceY = std::get<1>(sourceOrigin);
	frame.duration = duration / static_cast<float>(count);

	// If no width was specified, infer from the slide or the texture.
	if (!frame.width) {
		if (frame.texture && !std::get<0>(slide)) {
			frame.width = frame.texture->GetWidth();
		}
		else if (std::get<0>(slide)) {
			frame.width = std::get<0>(slide);
		}
		else {
			frame.width = 1;
		}
	}

	// If no height was specified, infer from the slide or the texture.
	if (!frame.height) {
		if (frame.texture && !std::get<1>(slide)) {
			frame.height = frame.texture->GetHeight();
		}
		else if (std::get<1>(slide)) {
			frame.height = std::get<1>(slide);
		}
		else {
			frame.height = 1;
		}
	}

	for (size_t i = 0; i < count; i++) {
		spriteAnimation->push_back(frame);

		frame.sourceX += std::get<0>(slide);
		frame.sourceY += std::get<1>(slide);
	}
}

void SpriteAnimationLoader::LoadCount(char* data, size_t& count)
{
	// Try to read value.
	char* token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		count = static_cast<size_t>(strtol(token, &token, 10));
	}
}

void SpriteAnimationLoader::LoadSlide(char* data, std::tuple<int, int>& slide)
{
	// Try to read first value.
	char* token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		std::get<0>(slide) = strtol(token, &token, 10);
	}

	// Try to read second value.
	token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		std::get<1>(slide) = strtol(token, &token, 10);
	}
}

void SpriteAnimationLoader::LoadTexture(char* data, TextureManager* textureManager, std::shared_ptr<Engine::Texture2D>& texture)
{
	// Try to read value.
	char* token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		texture = textureManager->GetTexture(token);
	}
}

void SpriteAnimationLoader::LoadDimensions(char* data, std::tuple<int, int>& dimensions)
{
	// Try to read first value.
	char* token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		std::get<0>(dimensions) = strtol(token, &token, 10);
	}

	// Try to read second value.
	token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		std::get<1>(dimensions) = strtol(token, &token, 10);
	}
}

void SpriteAnimationLoader::LoadSourceOrigin(char* data, std::tuple<int, int>& sourceOrigin)
{
	// Try to read first value.
	char* token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		std::get<0>(sourceOrigin) = strtol(token, &token, 10);
	}

	// Try to read second value.
	token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		std::get<1>(sourceOrigin) = strtol(token, &token, 10);
	}
}


void SpriteAnimationLoader::LoadDuration(char* data, float& duration)
{
	// Try to read value.
	char* token = strtok_s(data, s_delimiters, &data);
	if (token != 0) {
		duration = strtof(token, &token);
	}
}
