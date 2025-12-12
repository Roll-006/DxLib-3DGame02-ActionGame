#include "font_handler.hpp"

FontHandler::FontHandler()
{
	CreateFontHandle();
}

FontHandler::~FontHandler()
{

}

void FontHandler::CreateFontHandle()
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/font_data.json", j_data))
	{
		const auto font_json = j_data.at("font_data");
		for (const auto [font_name, value] : font_json.items())
		{
			auto font_data = value.get<FontData>();

			// 標準フォントでない場合はフォントを追加
			if (font_data.true_type_font != "")
			{
				AddFontResourceExA(font_data.true_type_font.c_str(), FR_PRIVATE, nullptr);
			}

			m_font_handle[font_name] = CreateFontToHandle(
				font_data.font_path.c_str(),
				font_data.size,
				font_data.thick,
				font_data.type,
				font_data.char_set,
				font_data.edge_size,
				font_data.italic,
				font_data.handle);
		}
	}
}
