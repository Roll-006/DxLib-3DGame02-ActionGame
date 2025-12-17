#pragma once
#include "../JSON/json_loader.hpp"
#include "ik_solver.hpp"

#include "../Interface/i_humanoid.hpp"
#include "../Base/physical_obj_base.hpp"
#include "../Base/animator_base.hpp"
#include "../Part/collider.hpp"
#include "../Data/humanoid_arm_ray_data.hpp"
#include "../Data/humanoid_arm_matrix_data.hpp"

class HumanoidArmIKSolver final
{
public:
	enum class IKKind
	{
		kNone = -1,

		kHandOnKnees,		// 膝に手を置くIK
		kForeArmOnKnees,	// 膝に肘を置くIK
		kHandOnThigh,		// ふとももに手を置くIK
		kHandOnGround,		// 地面に手を置くIK
	};

	enum class ResultKind
	{
		kSuccess,
		kLimitedSuccess,
		kFailure,
	};

public:
	HumanoidArmIKSolver(
		const std::shared_ptr<AnimatorBase>& animator, 
		const std::shared_ptr<Modeler>& modeler, 
		std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, 
		HumanoidArmRayData& ray_data);
	~HumanoidArmIKSolver();

	void Init	(const std::shared_ptr<IHumanoid>& humanoid);
	void Update	(const std::shared_ptr<IHumanoid>& humanoid);


	#pragma region コライダー
	void CreateLeftHandRay	(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid);
	void CreateRightHandRay	(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid);
	void DeleteLeftHandRay	(PhysicalObjBase* physical_obj);
	void DeleteRightHandRay	(PhysicalObjBase* physical_obj);

	void CalcLeftHandRayPos	(const std::shared_ptr<IHumanoid>& humanoid);
	void CalcRightHandRayPos(const std::shared_ptr<IHumanoid>& humanoid);
	#pragma endregion


	/// @brief 左膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyLeftKneelCrouchIK	(const std::shared_ptr<IHumanoid>& humanoid);
	/// @brief 右膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyRightKneelCrouchIK(const std::shared_ptr<IHumanoid>& humanoid);

	/// @brief フレーム行列のブレンドを行う
	/// @brief IK処理適用後に呼び出す必要あり
	void BlendFrame(const std::shared_ptr<IHumanoid>& humanoid);

private:
	#pragma region IK処理
	/// @brief 左膝の上に左手を置くIK処理
	ResultKind ApplyLeftHandIKOnKnees	(const std::shared_ptr<IHumanoid>& humanoid);
	/// @brief 右膝の上に右手を置くIK処理
	ResultKind ApplyRightHandIKOnKnees	(const std::shared_ptr<IHumanoid>& humanoid);

	/// @brief 左太ももの上に左手を置くIK処理
	ResultKind ApplyLeftHandIKOnThigh	(const std::shared_ptr<IHumanoid>& humanoid);
	/// @brief 右太ももの上に右手を置くIK処理
	ResultKind ApplyRightHandIKOnThigh	(const std::shared_ptr<IHumanoid>& humanoid);

	/// @brief 地面に左手を置くIK処理
	ResultKind ApplyLeftArmIKOnGround	(const std::shared_ptr<IHumanoid>& humanoid);
	/// @brief 地面に右手を置くIK処理
	ResultKind ApplyRightArmIKOnGround	(const std::shared_ptr<IHumanoid>& humanoid);
	#pragma endregion


	#pragma region ブレンドの起点を変更
	void ChangeLeftArmOriginMatrix	(const std::shared_ptr<IHumanoid>& humanoid, const bool is_set_result_m = false);
	void ChangeRightArmOriginMatrix	(const std::shared_ptr<IHumanoid>& humanoid, const bool is_set_result_m = false);
	#pragma endregion

private:
	static std::unordered_map<std::string, ModelFrameAngleLimitData> angle_limits;
	float arm_blend_time		= 0.25f;	// 仮　のちに定数化

	std::shared_ptr<AnimatorBase>									m_animator;
	std::shared_ptr<Modeler>										m_modeler;
	std::unordered_map<ColliderKind, std::shared_ptr<Collider>>&	m_colliders;
	HumanoidArmRayData&												m_ray_data;

	HumanoidArmMatrixData											m_origin_arm_matrices;
	HumanoidArmMatrixData											m_result_arm_matrices;

	float															m_left_arm_blend_timer;
	float															m_right_arm_blend_timer;

	std::unordered_map<TimeKind, IKKind>							m_left_ik_kind;
	std::unordered_map<TimeKind, IKKind>							m_right_ik_kind;
};
