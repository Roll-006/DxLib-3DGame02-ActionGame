#pragma once
#include <vector>
#include "../Calculation/vector_2d.hpp"
#include "../Data/text_data.hpp"
#include "../Data/single_button_prompt_data.hpp"

struct ButtonPromptData
{
	std::vector<SingleButtonPromptData>	single_button_prompt_data	= {};
	Vector2D<int>						screen_size					= {};
	float								height_ratio				= 0.0f;
	float								graphic_scale				= 1.0f;
	int									offset						= 0;
	TextData							text_data					= {};
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, ButtonPromptData& button_prompt_data)
{
	j_data.at("single_button_prompt_data")	.get_to(button_prompt_data.single_button_prompt_data);
	j_data.at("screen_size")				.get_to(button_prompt_data.screen_size);
	j_data.at("height_ratio")				.get_to(button_prompt_data.height_ratio);
	j_data.at("graphic_scale")				.get_to(button_prompt_data.graphic_scale);
	j_data.at("offset")						.get_to(button_prompt_data.offset);
	j_data.at("text_data")					.get_to(button_prompt_data.text_data);
}

inline void to_json(nlohmann::json& j_data, const ButtonPromptData& button_prompt_data)
{
	j_data = nlohmann::json
	{
		{ "single_button_prompt_data",	button_prompt_data.single_button_prompt_data },
		{ "screen_size",				button_prompt_data.screen_size },
		{ "height_ratio",				button_prompt_data.height_ratio },
		{ "graphic_scale",				button_prompt_data.graphic_scale },
		{ "offset",						button_prompt_data.offset },
		{ "text_data",					button_prompt_data.text_data },
	};
}
#pragma endregion
