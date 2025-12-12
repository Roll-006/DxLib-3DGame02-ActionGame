#pragma once
#include <string>

struct FontData
{
	std::string font_path			= "";
	std::string true_type_font		= "";
	int			size				= 0;
	int			thick				= 0;
	int			type				= -1;
	int			char_set			= -1;
	int			edge_size			= -1;
	bool		italic				= false;
	int			handle				= -1;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, FontData& font_data)
{
	j_data.at("font_path")		.get_to(font_data.font_path);
	j_data.at("true_type_font")	.get_to(font_data.true_type_font);
	j_data.at("size")			.get_to(font_data.size);
	j_data.at("thick")			.get_to(font_data.thick);
	j_data.at("type")			.get_to(font_data.type);
	j_data.at("char_set")		.get_to(font_data.char_set);
	j_data.at("edge_size")		.get_to(font_data.edge_size);
	j_data.at("italic")			.get_to(font_data.italic);
	j_data.at("handle")			.get_to(font_data.handle);
}

inline void to_json(nlohmann::json& j_data, const FontData& font_data)
{
	j_data = nlohmann::json
	{
		{ "font_path",			font_data.font_path },
		{ "true_type_font",		font_data.true_type_font },
		{ "size",				font_data.size },
		{ "thick",				font_data.thick },
		{ "type",				font_data.type },
		{ "char_set",			font_data.char_set },
		{ "edge_size",			font_data.edge_size },
		{ "italic",				font_data.italic },
		{ "handle",				font_data.handle },
	};
}
#pragma endregion
