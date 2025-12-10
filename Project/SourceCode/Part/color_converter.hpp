#pragma once
#include <string>
#include <sstream>

namespace color_converter
{
	/// @brief カラーコードをHEXからUINTに変換する
	/// @param  hex_str (例：#ffffff)
	/// @return UINT (例：16777215)
	[[nodiscard]] inline unsigned int ConvertHEXToUINT(const std::string& hex_str)
	{
		// 先頭が#である場合は除外して格納
		const auto hex = hex_str.at(0) == '#' ? hex_str.substr(1) : hex_str;

		// HEXをUINTに変換
		unsigned int u_int = 0;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> u_int;

		return u_int;
	}

	/// @brief カラーコードをUINTからHEXに変換する
	/// @param u_int  (例：16777215)
	/// @param is_add_sharp 変換後の先頭に[#]を付けるかどうか (初期値：true)
	/// @return HEX (例：#ffffff)
	[[nodiscard]] inline std::string ConvertUINTToHEX(const unsigned int u_int, const bool is_add_sharp = true)
	{
		// UINTをHEXに変換
		std::stringstream ss;
		ss << std::hex << u_int;
		const auto hex = is_add_sharp ? '#' + ss.str() : ss.str();

		return hex;
	}
};
