#pragma once
#include "../Font/font_handler.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../Part/type_converter.hpp"

struct TextData
{
	int				font_handle = -1;
	Vector2D<int>	font_size	= {};
	unsigned int	u_int_color = 0xffffff;

	std::string		font_path	= "";
	std::string		text		= "";
	std::string		hex_color	= "#ffffff";
	Vector2D<int>	offset		= {};
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, TextData& text_data)
{
	j_data.at("font_path").get_to(text_data.font_path);
	j_data.at("text")		.get_to(text_data.text);
	j_data.at("hex_color").get_to(text_data.hex_color);
	j_data.at("offset")	.get_to(text_data.offset);
}

inline void to_json(nlohmann::json& j_data, const TextData& text_data)
{
	j_data = nlohmann::json
	{
		{ "font_path",  text_data.font_path },
		{ "text",		text_data.text },
		{ "hex_color",	text_data.hex_color },
		{ "offset",		text_data.offset },
	};
}
#pragma endregion
