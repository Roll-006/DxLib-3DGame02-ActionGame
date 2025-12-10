#pragma once
#include "../Font/font_handler.hpp"
#include "../Calculation/vector_2d.hpp"

struct TextData
{
	int				font_handle = -1;
	Vector2D<int>	font_size	= {};

	std::string		font_path	= "";
	std::string		text		= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, TextData& text_data)
{
	data.at("font_path").get_to(text_data.font_path);
	data.at("text")		.get_to(text_data.text);
}

inline void to_json(nlohmann::json& data, const TextData& text_data)
{
	data = nlohmann::json
	{
		{ "font_path",  text_data.font_path },
		{ "text",		text_data.text },
	};
}
#pragma endregion
