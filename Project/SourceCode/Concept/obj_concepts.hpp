#pragma once
#include <concepts>

class ObjBase;
class PhysicalObjBase;
class CharacterBase;
class IItem;
class WeaponBase;
class KnifeBase;
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
	concept CharacterT		= std::is_base_of_v<CharacterBase, T>;

	/// @brief アイテム型である
	template<typename T>
	concept ItemT			= std::is_base_of_v<IItem, T>;

	/// @brief 武器オブジェクト型である
	template<typename T>
	concept WeaponT			= std::is_base_of_v<WeaponBase, T>;

	/// @brief ナイフオブジェクト型である
	template<typename T>
	concept KnifeT			= std::is_base_of_v<KnifeBase, T>;

	/// @brief 銃オブジェクト型である
	template<typename T>
	concept GunT			= std::is_base_of_v<GunBase, T>;
}
