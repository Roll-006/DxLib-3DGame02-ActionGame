#pragma once
#include "../Data/humanoid_body_matrix_data.hpp"

class IHumanoid;
class Animator;
class Modeler;
class PhysicalObjBase;
struct ModelFrameAngleLimitData;

class HumanoidBodyIKSolver final
{
public:
	enum class IKKind
	{
		kNone = -1,

		kHitReaction,		// ヒットリアクションIK
	};

	enum class ResultKind
	{
		kSuccess,
		kLimitedSuccess,
		kFailure,
	};

public:
	HumanoidBodyIKSolver(
		IHumanoid& humanoid,
		const std::shared_ptr<Animator>& animator, 
		const std::shared_ptr<Modeler>& modeler);

	~HumanoidBodyIKSolver();

	void Init();
	void Update();


	#pragma region IK処理
	/// @brief 胴体IK処理
	/// @brief この関数はIK処理の目的位置を手動で設定する関数です
	void ApplyBodyOneIK(const VECTOR& target_pos, const int frame_index, const IKKind ik_kind);

	/// @brief 胴体IK処理
	/// @brief この関数はIK処理の目的位置を手動で設定する関数です
	void ApplyBodyTowIK(const VECTOR& target_pos, const int frame_end_index, const IKKind ik_kind);
	#pragma endregion


	#pragma region ブレンド
	void ChangeOriginMatrix(const bool is_set_result_m = false);

	/// @brief フレーム行列のブレンドを行う
	/// @brief IK処理適用後に呼び出す必要あり
	void BlendFrame();
	#pragma endregion

private:
	static std::unordered_map<std::string, ModelFrameAngleLimitData> angle_limits;
	float body_blend_time		= 0.25f;	// 仮　のちに定数化

	IHumanoid&								m_humanoid;
	std::shared_ptr<Animator>				m_animator;
	std::shared_ptr<Modeler>				m_modeler;

	HumanoidBodyMatrixData					m_origin_body_matrices;
	HumanoidBodyMatrixData					m_result_body_matrices;

	float									m_blend_timer;

	std::unordered_map<TimeKind, IKKind>	m_ik_kind;
};
