#include "../Font/font_handler.hpp"
#include "../Data/text_data.hpp"

namespace text
{
	// テキスト情報を構築する
	// 構築情報 : ハンドル, 色, サイズ
	inline void CreateText(TextData& text_data)
	{
		text_data.font_handle	= FontHandler::GetInstance()->GetFontHandle(text_data.font_path);
		text_data.u_int_color	= type_converter::ConvertHEXToUINT(text_data.hex_color);
		text_data.size			= { GetDrawStringWidthToHandle(text_data.text.c_str(), -1, text_data.font_handle), GetFontSizeToHandle(text_data.font_handle) };
	}
}