#pragma once
#include "../Data/humanoid_leg_matrix_data.hpp"

class IHumanoid;
class Animator;
class Modeler;
class PhysicalObjBase;
struct HumanoidLegRayData;
struct ModelFrameAngleLimitData;

class HumanoidFootIKSolver final
{
public:
	enum class IKKind
	{
		kNone = -1,

		kHitReaction,		// ヒットリアクションIK
		kOnGround,			// 地面に設置させるIK
	};

public:
	HumanoidFootIKSolver(
		IHumanoid& humanoid,
		const std::shared_ptr<Animator>& animator, 
		const std::shared_ptr<Modeler>& modeler, 
		std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, 
		HumanoidLegRayData& ray_data);

	~HumanoidFootIKSolver();

	void Init();
	void Update();


	#pragma region コライダー
	void CreateLeftLegRay	(PhysicalObjBase* physical_obj) const;
	void CreateRightLegRay	(PhysicalObjBase* physical_obj) const;
	void CreateFootRay		(PhysicalObjBase* physical_obj) const;
	void CreateFoeBaseRay	(PhysicalObjBase* physical_obj) const;

	void DeleteLeftLegRay	(PhysicalObjBase* physical_obj) const;
	void DeleteRightLegRay	(PhysicalObjBase* physical_obj) const;

	void CalcLeftLegRayPos() const;
	void CalcRightLegRayPos() const;
	#pragma endregion


	#pragma region IK処理
	/// @brief 左脚IK処理
	/// @brief この関数はIK処理の目的位置を手動で設定する関数です
	void ApplyLeftFootIK (const VECTOR& target_pos, const int frame_end_index, const IKKind ik_kind);
	/// @brief 左脚IK処理
	/// @brief この関数はIK処理の目的位置を手動で設定する関数です
	void ApplyRightFootIK(const VECTOR& target_pos, const int frame_end_index, const IKKind ik_kind);

	/// @brief FootIKの処理を適用する
	void ApplyFootIK();

	/// @brief 左膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyLeftKneelCrouchIK();
	/// @brief 右膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyRightKneelCrouchIK();
	#pragma endregion


	#pragma region ブレンド
	void ChagneArmatureOriginMatrix	(const bool is_set_result_m = false);
	void ChangeLeftLegOriginMatrix	(const bool is_set_result_m = false);
	void ChangeRightLegOriginMatrix	(const bool is_set_result_m = false);

	/// @brief フレーム行列のブレンドを行う
	/// @brief IK処理適用後に呼び出す必要あり
	void BlendFrame();
	#pragma endregion

private:
	#pragma region IK処理
	void ApplyLeftLegIK	();
	void ApplyRightLegIK();
	void ApplyLeftKneelIK();
	void ApplyRightKneelIK();
	#pragma endregion


	void JudgeExecuteIK();

	void CalcToeBaseOffset();
	void DownArmature();
	void UpHips();

private:
	static std::unordered_map<std::string, ModelFrameAngleLimitData> angle_limits;
	float armature_blend_time	= 0.25f;	// 仮　のちに定数化
	float leg_blend_time		= 0.3f;	// 仮　のちに定数化

	IHumanoid&														m_humanoid;
	std::shared_ptr<Animator>										m_animator;
	std::shared_ptr<Modeler>										m_modeler;
	std::unordered_map<ColliderKind, std::shared_ptr<Collider>>&	m_colliders;
	HumanoidLegRayData&												m_ray_data;

	std::unordered_map<TimeKind, bool>								m_can_left_foot_ik;
	std::unordered_map<TimeKind, bool>								m_can_right_foot_ik;

	float															m_left_toe_base_offset;
	float															m_right_toe_base_offset;

	HumanoidLegMatrixData											m_origin_leg_matrices;
	HumanoidLegMatrixData											m_result_leg_matrices;

	float															m_armature_blend_timer;
	float															m_left_leg_blend_timer;
	float															m_right_leg_blend_timer;

	std::unordered_map<TimeKind, IKKind>							m_left_ik_kind;
	std::unordered_map<TimeKind, IKKind>							m_right_ik_kind;
};
