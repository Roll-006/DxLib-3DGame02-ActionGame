#pragma once
#include <string>
#include <sstream>
#include <Windows.h>

namespace type_converter
{
	/// @brief 文字列16進数からUINTへ変換する
	/// @param hex_str 変換元HEX (例：#ffffff)
	/// @return 変換後UINT (例：16777215)
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

	/// @brief UINTから文字列16進数へ変換する
	/// @param 変換元u_int (例：16777215)
	/// @param is_add_sharp 変換後文字列の先頭に[#]を付けるかどうか (初期値：true)
	/// @return 変換後HEX (例：#ffffff)
	[[nodiscard]] inline std::string ConvertUINTToHEX(const unsigned int u_int, const bool is_add_sharp = true)
	{
		// UINTをHEXに変換
		std::stringstream ss;
		ss << std::hex << u_int;
		const auto hex = is_add_sharp ? '#' + ss.str() : ss.str();

		return hex;
	}

	/// @brief UTF-8からShift-JISへ変換する
	/// @param utf_8 変換元UTF-8
	/// @return 変換後Shift-JIS
	[[nodiscard]] inline std::string ConvertUTF8ToShiftJIS(const std::string& utf_8)
	{
		if (utf_8.empty()) { return utf_8; }

		// UTF-8 ➡ UTF-16
		const auto utf_16_length = MultiByteToWideChar(CP_UTF8, 0, utf_8.c_str(), -1, nullptr, 0);
		if (utf_16_length == 0) { return utf_8; }

		// 終端は除外
		std::wstring utf_16(utf_16_length - 1, L'\0');
		if (!MultiByteToWideChar(CP_UTF8, 0, utf_8.c_str(), -1, &utf_16[0], utf_16_length)) { return utf_8; }

		// UTF-16 ➡ Shift-JIS
		int s_jis_length = WideCharToMultiByte(CP_ACP, 0, utf_16.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (s_jis_length == 0) return {};

		// 終端は除外
		std::string sjis_str(s_jis_length - 1, '\0');
		if (!WideCharToMultiByte(CP_ACP, 0, utf_16.c_str(), -1, &sjis_str[0], s_jis_length, nullptr, nullptr)) { return utf_8; }

		return sjis_str;
	}
};
