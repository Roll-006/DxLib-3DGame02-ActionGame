#pragma once
#include <concepts>
#include "../Data/IncludeList/input.hpp"

namespace input_concepts
{
	/// @brief パッド入力用の型である
	template<typename T>
	concept PadInputT = std::is_same_v<int,			T>
					 || std::is_same_v<pad::TriggerKind, T>
					 || std::is_same_v<pad::StickKind,	T>;

	/// @brief マウス入力用の型である
	template<typename T>
	concept MouseInputT = std::is_same_v<int,				T>
					   || std::is_same_v<mouse::SlideDirKind, T>
					   || std::is_same_v<mouse::WheelKind,	T>;
}
