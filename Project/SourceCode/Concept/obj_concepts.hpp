#pragma once
#include <concepts>

class ObjBase;
class PhysicalObjBase;
class CharaBase;
class WeaponBase;
class GunBase;

namespace obj_concepts
{
	/// @brief オブジェクト型である
	template<typename T>
	concept ObjT			= std::is_base_of_v<ObjBase, T>;

	/// @brief 物理オブジェクト型である
	template<typename T>
	concept PhysicalObjT	= std::is_base_of_v<PhysicalObjBase, T>;

	/// @brief キャラオブジェクト型である
	template<typename T>
	concept CharaObjT		= std::is_base_of_v<CharaBase, T>;

	/// @brief 武器オブジェクト型である
	template<typename T>
	concept WeaponObjT		= std::is_base_of_v<WeaponBase, T>;

	/// @brief 銃オブジェクト型である
	template<typename T>
	concept GunObjT			= std::is_base_of_v<GunBase, T>;
}
