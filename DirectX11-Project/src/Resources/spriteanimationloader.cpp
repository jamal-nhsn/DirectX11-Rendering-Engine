#include "pch.h"
#include "resourcemanager.h"

namespace Engine
{
	namespace
	{
		const char* s_delimiters = ",() \n:\t";

		void LoadCount(char* data, size_t& count)
		{
			// Try to read value.
			char* token = strtok_s(data, s_delimiters, &data);
			if (token != 0) {
				count = static_cast<size_t>(strtol(token, &token, 10));
			}
		}

		void LoadSlide(char* data, std::tuple<int, int>& slide)
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

		void LoadTexture(char* data, ResourceManager* resourceManager, Texture2D*& texture)
		{
			// Try to read value.
			char* token = strtok_s(data, s_delimiters, &data);
			if (token != 0) {
				texture = resourceManager->GetTexture2D(token);
			}
		}

		void LoadDimensions(char* data, std::tuple<int, int>& dimensions)
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

		void LoadSourceOrigin(char* data, std::tuple<int, int>& sourceOrigin)
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


		void LoadDuration(char* data, float& duration)
		{
			// Try to read value.
			char* token = strtok_s(data, s_delimiters, &data);
			if (token != 0) {
				duration = strtof(token, &token);
			}
		}

		void LoadFrame(SpriteAnimation* spriteAnimation, std::ifstream& file, ResourceManager* resourceManager)
		{
			// Default values in case some fields are excluded.
			size_t count = 1;
			std::tuple<int, int> slide = std::make_tuple(0, 0);
			Texture2D* texture = 0;
			std::tuple<int, int> dimensions = std::make_tuple(0, 0);
			std::tuple<int, int> sourceOrigin = std::make_tuple(0, 0);
			float duration = 1.0f;

			// Tokenize input and read in data.
			std::string line;
			std::streampos prevLine = file.tellg();
			while (std::getline(file, line)) {
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
					LoadTexture(data, resourceManager, texture);
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
					file.seekg(prevLine);
					break;
				}

				// Save previous line location.
				prevLine = file.tellg();
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
	}

	std::unique_ptr<SpriteAnimation> ResourceManager::SpriteAnimationLoader::LoadSpriteAnimation(const std::filesystem::path& filepath, ResourceManager* resourceManager)
	{
		std::unique_ptr<SpriteAnimation> spriteAnimation = std::make_unique<SpriteAnimation>();

		std::ifstream file(filepath);
		if (!file.is_open()) {
			return 0;
		}

		// Tokenize input and read in data.
		std::string line;
		std::streampos prevLine = file.tellg();
		while (std::getline(file, line)) {
			char* data = &line[0];
			char* label = strtok_s(data, s_delimiters, &data);

			if (!label) {
				continue;
			}

			if (strcmp(label, "#FRAME") == 0) {
				LoadFrame(spriteAnimation.get(), file, resourceManager);
			}
		}

		// File contained no frames.
		if (spriteAnimation->size() == 0) {
			return 0;
		}

		return spriteAnimation;
	}
}