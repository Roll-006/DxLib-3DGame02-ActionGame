#pragma once
#include "../Calculation/vector_2d.hpp"
#include "../Data/text_data.hpp"

struct GameOverTextData
{
	Vector2D<int> screen_size	= {};
	float		  height_ratio	= 0.0f;
	TextData	  text_data		= {};
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, GameOverTextData& game_over_text_data)
{
	data.at("screen_size")	.get_to(game_over_text_data.screen_size);
	data.at("height_ratio")	.get_to(game_over_text_data.height_ratio);
	data.at("text_data")	.get_to(game_over_text_data.text_data);
}

inline void to_json(nlohmann::json& data, const GameOverTextData& game_over_text_data)
{
	data = nlohmann::json
	{
		{ "screen_size",	game_over_text_data.screen_size },
		{ "height_ratio",	game_over_text_data.height_ratio },
		{ "text_data",		game_over_text_data.text_data },
	};
}
#pragma endregion
