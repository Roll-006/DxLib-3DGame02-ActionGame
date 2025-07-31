#include "enemy_base.hpp"

EnemyBase::EnemyBase(const std::string& name, const std::string& file_path, const MassKind mass_level_kind) : 
	CharacterBase	(name, ObjTag.ENEMY, file_path, mass_level_kind),
	m_move_dir	(v3d::GetZeroV()),
	m_look_dir	(v3d::GetZeroV())
{
	// 処理なし
}

void EnemyBase::CreateLegTrigger (const float up_leg_capsule_radius, const float down_leg_capsule_radius)
{
	const auto left_up_leg_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	const auto left_down_leg_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);
	const auto right_up_leg_trigger	 	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	const auto right_down_leg_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);

	// 登録
	AddCollider(std::make_shared<Collider>(ColliderKind::kLeftUpLegTrigger,		left_up_leg_trigger,	this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLeftDownLegTrigger,	left_down_leg_trigger,	this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kRightUpLegTrigger,	right_up_leg_trigger,	this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kRightDownLegTrigger,	right_down_leg_trigger,	this));

	CalcLegTriggerPos();
}

void EnemyBase::CreateHeadTrigger(const float sphere_radius)
{
	MATRIX head_mat = MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.HEAD));
	const VECTOR head_pos = MGetTranslateElem(head_mat);

	AddCollider(std::make_shared<Collider>(ColliderKind::kHeadTrigger, std::make_shared<Sphere>(head_pos, sphere_radius), this));
}

void EnemyBase::CreateBodyTrigger(const float capsule_radius)
{
	AddCollider(std::make_shared<Collider>(ColliderKind::kBodyTrigger, std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), capsule_radius), this));

	CalcBodyTriggerPos();
}

void EnemyBase::CalcLegTriggerPos()
{
	const int model_handle = m_modeler->GetModelHandle();

	// フレームの行列情報を取得
	MATRIX left_up_leg_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_UP_LEG));
	MATRIX left_leg_mat				= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_LEG));
	MATRIX left_foot_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_FOOT));
	MATRIX right_up_leg_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_UP_LEG));
	MATRIX right_leg_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_LEG));
	MATRIX right_foot_mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_FOOT));

	// 位置を取得
	const VECTOR left_up_leg_pos	= MGetTranslateElem(left_up_leg_mat);
	const VECTOR left_leg_pos		= MGetTranslateElem(left_leg_mat);
	const VECTOR left_foot_pos		= MGetTranslateElem(left_foot_mat);
	const VECTOR right_up_leg_pos	= MGetTranslateElem(right_up_leg_mat);
	const VECTOR right_leg_pos		= MGetTranslateElem(right_leg_mat);
	const VECTOR right_foot_pos		= MGetTranslateElem(right_foot_mat);

	// 位置を適用
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kLeftUpLegTrigger)		->GetShape())->SetSegmentBeginPos(left_leg_pos,		true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kLeftUpLegTrigger)		->GetShape())->SetSegmentEndPos	(left_up_leg_pos,	true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kLeftDownLegTrigger)	->GetShape())->SetSegmentBeginPos(left_foot_pos,	true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kLeftDownLegTrigger)	->GetShape())->SetSegmentEndPos	(left_leg_pos,		true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kRightUpLegTrigger)	->GetShape())->SetSegmentBeginPos(right_leg_pos,	true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kRightUpLegTrigger)	->GetShape())->SetSegmentEndPos	(right_up_leg_pos,	true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kRightDownLegTrigger)	->GetShape())->SetSegmentBeginPos(right_foot_pos,	true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kRightDownLegTrigger)	->GetShape())->SetSegmentEndPos	(right_leg_pos,		true);
}

void EnemyBase::CalcBodyTriggerPos()
{
	const int model_handle = m_modeler->GetModelHandle();

	// フレームの行列情報を取得
	MATRIX spine2_mat	= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.SPINE_2));
	MATRIX hips_mat		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HIPS));

	// 位置を取得
	const VECTOR spine2_pos	= MGetTranslateElem(spine2_mat);
	const VECTOR hips_pos	= MGetTranslateElem(hips_mat);

	// 位置を適用
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kBodyTrigger)->GetShape())->SetSegmentBeginPos	(spine2_pos, true);
	std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kBodyTrigger)->GetShape())->SetSegmentEndPos	(hips_pos,	 true);
}
