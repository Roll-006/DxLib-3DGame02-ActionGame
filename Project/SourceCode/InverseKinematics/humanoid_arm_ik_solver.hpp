#pragma once
#include "../Data/humanoid_arm_matrix_data.hpp"

class IHumanoid;
class Animator;
class Modeler;
class PhysicalObjBase;
struct HumanoidArmRayData;
struct ModelFrameAngleLimitData;

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
		IHumanoid& humanoid,
		const std::shared_ptr<Animator>& animator, 
		const std::shared_ptr<Modeler>& modeler, 
		std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, 
		HumanoidArmRayData& ray_data);
	~HumanoidArmIKSolver();

	void Init	();
	void Update	();


	#pragma region コライダー
	void CreateLeftHandRay	(PhysicalObjBase* physical_obj) const;
	void CreateRightHandRay	(PhysicalObjBase* physical_obj) const;
	void DeleteLeftHandRay	(PhysicalObjBase* physical_obj) const;
	void DeleteRightHandRay	(PhysicalObjBase* physical_obj) const;

	void CalcLeftHandRayPos	() const;
	void CalcRightHandRayPos() const;
	#pragma endregion


	/// @brief 左膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyLeftKneelCrouchIK();
	/// @brief 右膝を地面につけるしゃがみアニメーションにIK処理を適用する
	void ApplyRightKneelCrouchIK();

	/// @brief フレーム行列のブレンドを行う
	/// @brief IK処理適用後に呼び出す必要あり
	void BlendFrame();

private:
	#pragma region IK処理
	/// @brief 左膝の上に左手を置くIK処理
	const ResultKind ApplyLeftHandIKOnKnees();
	/// @brief 右膝の上に右手を置くIK処理
	const ResultKind ApplyRightHandIKOnKnees();

	/// @brief 左太ももの上に左手を置くIK処理
	const ResultKind ApplyLeftHandIKOnThigh();
	/// @brief 右太ももの上に右手を置くIK処理
	const ResultKind ApplyRightHandIKOnThigh();

	/// @brief 地面に左手を置くIK処理
	const ResultKind ApplyLeftArmIKOnGround();
	/// @brief 地面に右手を置くIK処理
	const ResultKind ApplyRightArmIKOnGround();
	#pragma endregion


	#pragma region ブレンドの起点を変更
	void ChangeLeftArmOriginMatrix	(const bool is_set_result_m = false);
	void ChangeRightArmOriginMatrix	(const bool is_set_result_m = false);
	#pragma endregion

private:
	static std::unordered_map<std::string, ModelFrameAngleLimitData> angle_limits;
	float arm_blend_time		= 0.25f;	// 仮　のちに定数化

	IHumanoid&														m_humanoid;
	std::shared_ptr<Animator>										m_animator;
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
