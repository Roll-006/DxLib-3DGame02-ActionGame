#pragma once
#include <concepts>

class ObjBase;

// Œ^§ŒÀ
namespace obj_concepts
{
	/// @brief ObjBase‚Ì”h¶ƒNƒ‰ƒX‚Å‚ ‚é
	template<typename T>
	concept DerivedT = std::is_base_of_v<ObjBase, T>;
}
