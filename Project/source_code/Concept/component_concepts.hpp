#pragma once
#include <concepts>

class ComponentBase;

// Œ^§ŒÀ
namespace component_concepts
{
	/// @brief ComponentBase‚Ì”h¶ƒNƒ‰ƒX‚Å‚ ‚é
	template<typename T>
	concept DerivedT = std::is_base_of_v<ComponentBase, T>;
}
