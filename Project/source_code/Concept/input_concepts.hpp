#pragma once
#include <concepts>
#include "../Data/input_data.hpp"

namespace input_concepts
{
	/// @brief パッド入力用の型である
	template<typename T>
	concept PadInputT = std::is_same_v<int,			T>
					 || std::is_same_v<TriggerKind, T>
					 || std::is_same_v<StickKind,	T>;

	/// @brief マウス入力用の型である
	template<typename T>
	concept MouseInputT = std::is_same_v<int,				T>
					   || std::is_same_v<MouseSlideDirKind, T>
					   || std::is_same_v<MouseWheelKind,	T>;
}
