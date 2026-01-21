//#include "../JSON/json_loader.hpp"
//#include "ik_solver.hpp"
//#include "frame_info.hpp"
//#include "../Interface/i_humanoid.hpp"
//#include "../Base/physical_obj_base.hpp"
//#include "../Animator/animator.hpp"
//#include "../Part/collider.hpp"
//#include "../Data/model_frame_angle_limit_data.hpp"
//#include "humanoid_body_ik_solver.hpp"
//
//std::unordered_map<std::string, ModelFrameAngleLimitData> HumanoidBodyIKSolver::angle_limits = {};
//
//HumanoidBodyIKSolver::HumanoidBodyIKSolver(
//	IHumanoid& humanoid,
//	const std::shared_ptr<Animator>& animator, 
//	const std::shared_ptr<Modeler>& modeler, 
//	std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders) :
//	m_humanoid				(humanoid),
//	m_animator				(animator),
//	m_modeler				(modeler),
//	m_colliders				(colliders),
//	m_origin_body_matrices	(),
//	m_result_body_matrices	(),
//	m_left_body_blend_timer	(0.0f),
//	m_right_body_blend_timer(0.0f),
//	m_left_ik_kind			{ {TimeKind::kCurrent, IKKind::kNone}, {TimeKind::kPrev, IKKind::kNone} },
//	m_right_ik_kind			{ {TimeKind::kCurrent, IKKind::kNone}, {TimeKind::kPrev, IKKind::kNone} }
//{
//	if (angle_limits.empty())
//	{
//		nlohmann::json j_data;
//		if (json_loader::Load("Data/JSON/model_body_angle_limits.json", j_data))
//		{
//			for (const auto& [frame_name, angle_limit] : j_data.at("humanoid").items())
//			{
//				angle_limits[frame_name] = angle_limit.get<ModelFrameAngleLimitData>();
//			}
//		}
//	}
//}
//
//HumanoidBodyIKSolver::~HumanoidBodyIKSolver()
//{
//
//}
//
//void HumanoidBodyIKSolver::Init()
//{
//	ChangeLeftBodyOriginMatrix();
//	ChangeRightBodyOriginMatrix();
//}
//
//void HumanoidBodyIKSolver::Update()
//{
//	m_left_ik_kind .at(TimeKind::kPrev) = m_left_ik_kind .at(TimeKind::kCurrent);
//	m_right_ik_kind.at(TimeKind::kPrev) = m_right_ik_kind.at(TimeKind::kCurrent);
//
//	m_left_ik_kind .at(TimeKind::kCurrent) = IKKind::kNone;
//	m_right_ik_kind.at(TimeKind::kCurrent) = IKKind::kNone;
//}
//
//void HumanoidBodyIKSolver::BlendFrame()
//{
//	const auto model_handle		= m_modeler->GetModelHandle();
//	const auto humanoid_frame	= m_humanoid.GetHumanoidFrame();
//
//	// 補間係数tを取得
//	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
//	math::Increase(m_left_body_blend_timer,  delta_time, body_blend_time,	false);
//	math::Increase(m_right_body_blend_timer, delta_time, body_blend_time,	false);
//	const auto left_body_t	= math::GetUnitValue<float, float>(0.0f, body_blend_time, m_left_body_blend_timer);
//	const auto right_body_t	= math::GetUnitValue<float, float>(0.0f, body_blend_time, m_right_body_blend_timer);
//
//	// ブレンド結果を取得
//	m_result_body_matrices.left_body			= math::GetLerpMatrix(m_origin_body_matrices.left_body,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftBodyIndex		(model_handle)), left_body_t);
//	m_result_body_matrices.left_fore_body		= math::GetLerpMatrix(m_origin_body_matrices.left_fore_body,	MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeBodyIndex	(model_handle)), left_body_t);
//	m_result_body_matrices.left_hand			= math::GetLerpMatrix(m_origin_body_matrices.left_hand,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex		(model_handle)), left_body_t);
//																																																	 
//	m_result_body_matrices.right_body			= math::GetLerpMatrix(m_origin_body_matrices.right_body,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightBodyIndex		(model_handle)), right_body_t);
//	m_result_body_matrices.right_fore_body	= math::GetLerpMatrix(m_origin_body_matrices.right_fore_body,	MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightForeBodyIndex	(model_handle)), right_body_t);
//	m_result_body_matrices.right_hand		= math::GetLerpMatrix(m_origin_body_matrices.right_hand,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex		(model_handle)), right_body_t);
//
//	// ブレンド結果をモデルに設定
//	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftBodyIndex		(model_handle), m_result_body_matrices.left_body);
//	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftForeBodyIndex	(model_handle), m_result_body_matrices.left_fore_body);
//	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex		(model_handle), m_result_body_matrices.left_hand);
//	
//	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightBodyIndex		(model_handle), m_result_body_matrices.right_body);
//	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightForeBodyIndex	(model_handle), m_result_body_matrices.right_fore_body);
//	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex		(model_handle), m_result_body_matrices.right_hand);
//}
//
//
//#pragma region IK処理
//
//#pragma endregion
//
//
//#pragma region ブレンドの起点を変更
//void HumanoidBodyIKSolver::ChangeOriginMatrix(const bool is_set_result_m)
//{
//	const auto model_handle					= m_modeler->GetModelHandle();
//	const auto humanoid_frame				= m_humanoid.GetHumanoidFrame();
//
//	m_origin_body_matrices.left_body			= is_set_result_m ? m_result_body_matrices.left_body			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftBodyIndex		(model_handle));
//	m_origin_body_matrices.left_fore_body		= is_set_result_m ? m_result_body_matrices.left_fore_body		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeBodyIndex	(model_handle));
//	m_origin_body_matrices.left_hand			= is_set_result_m ? m_result_body_matrices.left_hand			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex		(model_handle));
//
//	m_left_body_blend_timer = 0.0f;
//}
//#pragma endregion
