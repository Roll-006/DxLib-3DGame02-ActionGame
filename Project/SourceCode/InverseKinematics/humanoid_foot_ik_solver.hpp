#pragma once
#include "../JSON/json_loader.hpp"

#include "ik_solver.hpp"

#include "../Interface/i_humanoid.hpp"
#include "../Base/physical_obj_base.hpp"
#include "../Base/animator_base.hpp"
#include "../Part/collider.hpp"
#include "../Data/humanoid_leg_ray_data.hpp"
#include "../Data/humanoid_leg_matrix_data.hpp"

class HumanoidFootIKSolver final
{
public:
	HumanoidFootIKSolver(
		const std::shared_ptr<AnimatorBase>& animator, 
		const std::shared_ptr<Modeler>& modeler, 
		std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, 
		HumanoidLegRayData& ray_data);
	~HumanoidFootIKSolver();

	void Init	(const std::shared_ptr<IHumanoid>& humanoid);
	void Update	(const std::shared_ptr<IHumanoid>& humanoid);


	#pragma region コライダー
	void CreateLeftLegRay	(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid);
	void CreateRightLegRay	(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid);
	void CreateFootRay		(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid);
	void CreateFoeBaseRay	(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid);

	void DeleteLeftLegRay	(PhysicalObjBase* physical_obj);
	void DeleteRightLegRay	(PhysicalObjBase* physical_obj);

	void CalcLeftLegRayPos	(const std::shared_ptr<IHumanoid>& humanoid);
	void CalcRightLegRayPos	(const std::shared_ptr<IHumanoid>& humanoid);
	#pragma endregion

	/// @brief FootIKの処理を適用する
	void ApplyFootIK			(const std::shared_ptr<IHumanoid>& humanoid);

	/// @brief 左膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyLeftKneelCrouchIK	(const std::shared_ptr<IHumanoid>& humanoid);

	/// @brief 右膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyRightKneelCrouchIK(const std::shared_ptr<IHumanoid>& humanoid);

	/// @brief フレーム行列のブレンドを行う
	/// @brief IK処理適用後に呼び出す必要あり
	void BlendFrame(const std::shared_ptr<IHumanoid>& humanoid);

private:
	void JudgeExecuteIK		(const std::shared_ptr<IHumanoid>& humanoid);

	void CalcToeBaseOffset	(const std::shared_ptr<IHumanoid>& humanoid);
	void DownArmature		(const std::shared_ptr<IHumanoid>& humanoid);
	void UpHips				(const std::shared_ptr<IHumanoid>& humanoid);


	#pragma region IK処理
	void ApplyLeftLegIK		(const std::shared_ptr<IHumanoid>& humanoid);
	void ApplyRightLegIK	(const std::shared_ptr<IHumanoid>& humanoid);
	void ApplyLeftKneelIK	(const std::shared_ptr<IHumanoid>& humanoid);
	void ApplyRightKneelIK	(const std::shared_ptr<IHumanoid>& humanoid);
	#pragma endregion


	#pragma region ブレンドの起点を変更
	void ChagneArmatureOriginMatrix	(const std::shared_ptr<IHumanoid>& humanoid);
	void ChangeLeftLegOriginMatrix	(const std::shared_ptr<IHumanoid>& humanoid);
	void ChangeRightLegOriginMatrix	(const std::shared_ptr<IHumanoid>& humanoid);
	#pragma endregion

private:
	static std::unordered_map<std::string, ModelFrameAngleLimitData> angle_limits;
	float armature_blend_time	= 0.25f;	// 仮　のちに定数化
	float leg_blend_time		= 0.5f;	// 仮　のちに定数化

	std::shared_ptr<AnimatorBase>									m_animator;
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
