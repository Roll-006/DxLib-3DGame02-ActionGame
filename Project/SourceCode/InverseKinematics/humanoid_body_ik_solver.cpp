#include "../JSON/json_loader.hpp"
#include "ik_solver.hpp"
#include "frame_info.hpp"
#include "../Interface/i_humanoid.hpp"
#include "../Base/physical_obj_base.hpp"
#include "../Animator/animator.hpp"
#include "../Part/collider.hpp"
#include "../Data/model_frame_angle_limit_data.hpp"
#include "humanoid_body_ik_solver.hpp"

std::unordered_map<std::string, ModelFrameAngleLimitData> HumanoidBodyIKSolver::angle_limits = {};

HumanoidBodyIKSolver::HumanoidBodyIKSolver(
	IHumanoid& humanoid,
	const std::shared_ptr<Animator>& animator, 
	const std::shared_ptr<Modeler>& modeler) :
	m_humanoid				(humanoid),
	m_animator				(animator),
	m_modeler				(modeler),
	m_origin_body_matrices	(),
	m_result_body_matrices	(),
	m_blend_timer			(0.0f),
	m_ik_kind				{ {TimeKind::kCurrent, IKKind::kNone}, {TimeKind::kPrev, IKKind::kNone} }
{
	//if (angle_limits.empty())
	//{
	//	nlohmann::json j_data;
	//	if (json_loader::Load("Data/JSON/model_body_angle_limits.json", j_data))
	//	{
	//		for (const auto& [frame_name, angle_limit] : j_data.at("humanoid").items())
	//		{
	//			angle_limits[frame_name] = angle_limit.get<ModelFrameAngleLimitData>();
	//		}
	//	}
	//}
}

HumanoidBodyIKSolver::~HumanoidBodyIKSolver()
{

}

void HumanoidBodyIKSolver::Init()
{
	ChangeOriginMatrix();
}

void HumanoidBodyIKSolver::Update()
{
	m_ik_kind.at(TimeKind::kPrev)		= m_ik_kind.at(TimeKind::kCurrent);
	m_ik_kind.at(TimeKind::kCurrent)	= IKKind::kNone;
}

void HumanoidBodyIKSolver::BlendFrame()
{
	const auto model_handle		= m_modeler->GetModelHandle();
	const auto humanoid_frame	= m_humanoid.GetHumanoidFrame();

	// 補間係数tを取得
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
	math::Increase(m_blend_timer, delta_time, body_blend_time, false);
	const auto t = math::GetUnitValue<float, float>(0.0f, body_blend_time, m_blend_timer);

	// ブレンド結果を取得
	m_result_body_matrices.spine		= math::GetLerpMatrix(m_origin_body_matrices.spine,			MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetSpineIndex		(model_handle)), t);
	m_result_body_matrices.spine1		= math::GetLerpMatrix(m_origin_body_matrices.spine1,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetSpine1Index		(model_handle)), t);
	m_result_body_matrices.spine2		= math::GetLerpMatrix(m_origin_body_matrices.spine2,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetSpine2Index		(model_handle)), t);
	m_result_body_matrices.neck			= math::GetLerpMatrix(m_origin_body_matrices.neck,			MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetNeckIndex		(model_handle)), t);
	m_result_body_matrices.head			= math::GetLerpMatrix(m_origin_body_matrices.head,			MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetHeadIndex		(model_handle)), t);
	m_result_body_matrices.head_top_end = math::GetLerpMatrix(m_origin_body_matrices.head_top_end,	MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetHeadTopEndIndex	(model_handle)), t);
	m_result_body_matrices.left_eye		= math::GetLerpMatrix(m_origin_body_matrices.left_eye,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftEyeIndex	(model_handle)), t);
	m_result_body_matrices.right_eye	= math::GetLerpMatrix(m_origin_body_matrices.right_eye,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightEyeIndex	(model_handle)), t);

	// ブレンド結果をモデルに設定
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetSpineIndex		(model_handle), m_result_body_matrices.spine);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetSpine1Index		(model_handle), m_result_body_matrices.spine1);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetSpine2Index		(model_handle), m_result_body_matrices.spine2);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetNeckIndex		(model_handle), m_result_body_matrices.neck);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetHeadIndex		(model_handle), m_result_body_matrices.head);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetHeadTopEndIndex	(model_handle), m_result_body_matrices.head_top_end);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftEyeIndex	(model_handle), m_result_body_matrices.left_eye);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightEyeIndex	(model_handle), m_result_body_matrices.right_eye);
}


#pragma region IK処理
void HumanoidBodyIKSolver::ApplyBodyOneIK(const VECTOR& target_pos, const int frame_index, const IKKind ik_kind)
{
	m_ik_kind.at(TimeKind::kCurrent) = ik_kind;

	const auto model_handle		= m_modeler->GetModelHandle();
	const auto humanoid_frame	= m_humanoid.GetHumanoidFrame();

	const auto hips_frame		= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetHipsIndex(model_handle));
	const auto hips_world_axis	= math::ConvertRotMatrixToAxis(hips_frame.world_rot_m);

	ik_solver::OneBoneIK(model_handle, target_pos, frame_index, std::make_optional<AxisData>(-hips_world_axis.x_axis, AxisKind::kRight));
}

void HumanoidBodyIKSolver::ApplyBodyTowIK(const VECTOR& target_pos, const int frame_end_index, const HumanoidBodyIKSolver::IKKind ik_kind)
{
	m_ik_kind.at(TimeKind::kCurrent) = ik_kind;

	const auto model_handle			= m_modeler->GetModelHandle();
	const auto humanoid_frame		= m_humanoid.GetHumanoidFrame();
	auto	   begin_angle_limit	= ModelFrameAngleLimitData();
	auto	   middle_angle_limit	= ModelFrameAngleLimitData();

	const auto hips_frame			= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetHipsIndex(model_handle));
	const auto hips_world_axis		= math::ConvertRotMatrixToAxis(hips_frame.world_rot_m);

	// ブレンドの起点行列を設定する
	if (m_ik_kind.at(TimeKind::kCurrent) != m_ik_kind.at(TimeKind::kPrev))
	{
		ChangeOriginMatrix(true);
	}

	// 胴体IK処理
	ik_solver::TwoBoneIK(
		model_handle, target_pos, frame_end_index,
		begin_angle_limit, middle_angle_limit,
		ik_solver::RotDirKind::kRight, false, std::make_optional<AxisData>(-hips_world_axis.x_axis, AxisKind::kRight));
}
#pragma endregion


#pragma region ブレンドの起点を変更
void HumanoidBodyIKSolver::ChangeOriginMatrix(const bool is_set_result_m)
{
	const auto model_handle	  = m_modeler->GetModelHandle();
	const auto humanoid_frame = m_humanoid.GetHumanoidFrame();

	m_origin_body_matrices.spine		= is_set_result_m ? m_result_body_matrices.spine		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetSpineIndex		(model_handle));
	m_origin_body_matrices.spine1		= is_set_result_m ? m_result_body_matrices.spine1		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetSpine1Index		(model_handle));
	m_origin_body_matrices.spine2		= is_set_result_m ? m_result_body_matrices.spine2		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetSpine2Index		(model_handle));
	m_origin_body_matrices.neck			= is_set_result_m ? m_result_body_matrices.neck			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetNeckIndex			(model_handle));
	m_origin_body_matrices.head			= is_set_result_m ? m_result_body_matrices.head			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetHeadIndex			(model_handle));
	m_origin_body_matrices.head_top_end = is_set_result_m ? m_result_body_matrices.head_top_end : MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetHeadTopEndIndex	(model_handle));
	m_origin_body_matrices.left_eye		= is_set_result_m ? m_result_body_matrices.left_eye		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftEyeIndex		(model_handle));
	m_origin_body_matrices.right_eye	= is_set_result_m ? m_result_body_matrices.right_eye	: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightEyeIndex		(model_handle));

	m_blend_timer = 0.0f;
}
#pragma endregion
