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
	HumanoidFootIKSolver(
		IHumanoid& humanoid,
		const std::shared_ptr<Animator>& animator, 
		const std::shared_ptr<Modeler>& modeler, 
		std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, 
		HumanoidLegRayData& ray_data);

	~HumanoidFootIKSolver();

	void Init	();
	void Update	();


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

	/// @brief FootIKの処理を適用する
	void ApplyFootIK();

	/// @brief 左膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyLeftKneelCrouchIK();

	/// @brief 右膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyRightKneelCrouchIK();

	/// @brief フレーム行列のブレンドを行う
	/// @brief IK処理適用後に呼び出す必要あり
	void BlendFrame();

private:
	void JudgeExecuteIK();

	void CalcToeBaseOffset();
	void DownArmature();
	void UpHips();


	#pragma region IK処理
	void ApplyLeftLegIK	();
	void ApplyRightLegIK();
	void ApplyLeftKneelIK();
	void ApplyRightKneelIK();
	#pragma endregion


	#pragma region ブレンドの起点を変更
	void ChagneArmatureOriginMatrix();
	void ChangeLeftLegOriginMatrix();
	void ChangeRightLegOriginMatrix();
	#pragma endregion

private:
	static std::unordered_map<std::string, ModelFrameAngleLimitData> angle_limits;
	float armature_blend_time	= 0.25f;	// 仮　のちに定数化
	float leg_blend_time		= 0.5f;	// 仮　のちに定数化

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

	float															m_armature_blend_timer;
	float															m_left_leg_blend_timer;
	float															m_right_leg_blend_timer;
};
