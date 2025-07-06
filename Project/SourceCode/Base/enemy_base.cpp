#include "enemy_base.hpp"

EnemyBase::EnemyBase(const std::string& name, const std::string& file_path, const MassKind mass_level_kind) : 
	CharaBase	(name, ObjTag.ENEMY, file_path, mass_level_kind),
	m_move_dir	(v3d::GetZeroV()),
	m_look_dir	(v3d::GetZeroV())
{

}

void EnemyBase::CreateLegTrigger (const float up_leg_capsule_radius, const float down_leg_capsule_radius)
{
	m_leg_trigger[ColliderKind::kLeftUpLegTrigger]		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	m_leg_trigger[ColliderKind::kLeftDownLegTrigger]	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);
	m_leg_trigger[ColliderKind::kRightUpLegTrigger]		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	m_leg_trigger[ColliderKind::kRightDownLegTrigger]	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);

	// 登録
	AddCollider(std::make_shared<Collider>(ColliderKind::kLeftUpLegTrigger,		m_leg_trigger.at(ColliderKind::kLeftUpLegTrigger),		this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLeftDownLegTrigger,	m_leg_trigger.at(ColliderKind::kLeftDownLegTrigger),	this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kRightUpLegTrigger,	m_leg_trigger.at(ColliderKind::kRightUpLegTrigger),		this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kRightDownLegTrigger,	m_leg_trigger.at(ColliderKind::kRightDownLegTrigger),	this));

	CalcLegTriggerPos();
}

void EnemyBase::CreateHeadTrigger(const float sphere_radius)
{
	MATRIX head_mat = MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.HEAD));
	const VECTOR head_pos = MGetTranslateElem(head_mat);

	AddCollider(std::make_shared<Collider>(ColliderKind::kHeadTrigger, std::make_shared<Sphere>(head_pos, sphere_radius), this));
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
	m_leg_trigger.at(ColliderKind::kLeftUpLegTrigger)	->SetSegmentBeginPos(left_leg_pos,		true);
	m_leg_trigger.at(ColliderKind::kLeftUpLegTrigger)	->SetSegmentEndPos	(left_up_leg_pos,	true);
	m_leg_trigger.at(ColliderKind::kLeftDownLegTrigger)	->SetSegmentBeginPos(left_foot_pos,		true);
	m_leg_trigger.at(ColliderKind::kLeftDownLegTrigger)	->SetSegmentEndPos	(left_leg_pos,		true);
	m_leg_trigger.at(ColliderKind::kRightUpLegTrigger)  ->SetSegmentBeginPos(right_leg_pos,		true);
	m_leg_trigger.at(ColliderKind::kRightUpLegTrigger)  ->SetSegmentEndPos	(right_up_leg_pos,	true);
	m_leg_trigger.at(ColliderKind::kRightDownLegTrigger)->SetSegmentBeginPos(right_foot_pos,	true);
	m_leg_trigger.at(ColliderKind::kRightDownLegTrigger)->SetSegmentEndPos	(right_leg_pos,		true);
}
