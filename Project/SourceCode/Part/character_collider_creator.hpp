#pragma once
#include <DxLib.h>

#include "../Base/physical_obj_base.hpp"
#include "../Part/modeler.hpp"

class CharacterColliderCreator
{
public:
	#pragma region コライダーの作成
	/// @brief カプセルコライダーを作成
	void CreateCapsuleCollider	(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float capsule_radius);

	/// @brief 着地トリガーを作成
	/// @brief WARNING : カプセルコライダーが作成された後に呼び出す必要あり
	void CreateLandingTrigger	(PhysicalObjBase* physical_obj, const float sphere_radius);
	
	/// @brief 脚部トリガーを作成
	void CreateLegTrigger		(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float up_leg_capsule_radius, const float down_leg_capsule_radius);
	
	/// @brief 頭部トリガーを作成
	void CreateHeadTrigger		(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float sphere_radius);
	
	/// @brief 胴体トリガーを作成
	void CreateBodyTrigger		(PhysicalObjBase* physical_obj, const std::shared_ptr<Modeler> modeler, const float capsule_radius);
	
	/// @brief メッシュトリガーを作成
	void CreateMeshTrigger		(PhysicalObjBase* phsyical_obj, const std::shared_ptr<Modeler> modeler);
	#pragma endregion


	#pragma region 位置計算
	/// @brief 脚部トリガーの位置を計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcLegTriggerPos (std::shared_ptr<Modeler> modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>> collider);

	/// @brief 胴体トリガーの位置を計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcBodyTriggerPos(std::shared_ptr<Modeler> modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>> collider);
	#pragma endregion


	#pragma region その他計算
	/// @brief カプセルコライダーの長さを計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcCapsuleColliderLength(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler> modeler);
	#pragma endregion
};
