#pragma once
#include "../Calculation/vector_3d.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../Data/text_data.hpp"

struct EscapeIconData final
{
	unsigned int	base_gauge_u_int_color		= 0xffffff;
	unsigned int	escape_gauge_u_int_color	= 0xffffff;

	Vector2D<int>	screen_size					= {};
	int				gauge_radius				= 0;
	int				gauge_thickness				= 0;
	std::string		base_gauge_hex_color		= "";
	std::string		escape_gauge_hex_color		= "";
	Vector2D<int>	escape_text_offset			= {};
	Vector2D<int>	hold_text_offset			= {};
	TextData		escape_text_data			= {};
	TextData		hold_text_data				= {};
	std::string		blur_circle_graphic_path	= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, EscapeIconData& escape_icon_data)
{
	j_data.at("screen_size")				.get_to(escape_icon_data.screen_size);
	j_data.at("gauge_radius")				.get_to(escape_icon_data.gauge_radius);
	j_data.at("gauge_thickness")			.get_to(escape_icon_data.gauge_thickness);
	j_data.at("base_gauge_color")			.get_to(escape_icon_data.base_gauge_hex_color);
	j_data.at("escape_gauge_color")			.get_to(escape_icon_data.escape_gauge_hex_color);
	j_data.at("escape_text_offset")			.get_to(escape_icon_data.escape_text_offset);
	j_data.at("hold_text_offset")			.get_to(escape_icon_data.hold_text_offset);
	j_data.at("escape_text_data")			.get_to(escape_icon_data.escape_text_data);
	j_data.at("hold_text_data")				.get_to(escape_icon_data.hold_text_data);
	j_data.at("blur_circle_graphic_path")	.get_to(escape_icon_data.blur_circle_graphic_path);
}

inline void to_json(nlohmann::json& j_data, const EscapeIconData& escape_icon_data)
{
	j_data = nlohmann::json
	{
		{ "screen_size",					escape_icon_data.screen_size },
		{ "gauge_radius",					escape_icon_data.gauge_radius },
		{ "gauge_thickness",				escape_icon_data.gauge_thickness },
		{ "base_gauge_color",				escape_icon_data.base_gauge_hex_color },
		{ "escape_gauge_color",				escape_icon_data.escape_gauge_hex_color },
		{ "escape_text_offset",				escape_icon_data.escape_text_offset },
		{ "hold_text_offset",				escape_icon_data.hold_text_offset },
		{ "escape_text_data",				escape_icon_data.escape_text_data },
		{ "hold_text_data",					escape_icon_data.hold_text_data },
		{ "blur_circle_graphic_path",		escape_icon_data.blur_circle_graphic_path },
	};
}
#pragma endregion
