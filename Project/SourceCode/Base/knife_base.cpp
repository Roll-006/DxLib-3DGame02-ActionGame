#include "knife_base.hpp"

KnifeBase::KnifeBase(const std::string& name, const HolsterKind holster_kind) :
	WeaponBase(name, WeaponKind::kKnife, holster_kind)
{

}

void KnifeBase::TrackOwnerHand()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const auto owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.RIGHT_HAND);
	const auto owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(hold_offset_angle);
	const auto result_mat = offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(hold_offset_pos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, hold_offset_scale);
}

void KnifeBase::TrackOwnerHolster()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	int owner_attach_frame_num = -1;

	switch (holster_kind)
	{
	case HolsterKind::kKnife:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.SPINE_2);
		break;

	case HolsterKind::kHandgun:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.RIGHT_UP_LEG);
		break;

	case HolsterKind::kRifle:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.SPINE_2);
		break;

	case HolsterKind::kGrenade:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.RIGHT_UP_LEG);
		break;
	}


	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto owner_attach_frame_mat	= MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);
	const auto offset_angle_mat			= math::ConvertEulerAnglesToXYZRotMatrix(attach_offset_angle);
	const auto result_mat				= offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(attach_offset_pos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, attach_offset_scale);
}

void KnifeBase::AddAttackTrigger()
{
	AddCollider(std::make_shared<Collider>(ColliderKind::kAttackTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), trigger_radius), this));
}

void KnifeBase::RemoveAttackTrigger()
{
	RemoveCollider(ColliderKind::kAttackTrigger);
}

void KnifeBase::CalcAttackTriggerPos()
{
	if (!m_colliders.contains(ColliderKind::kAttackTrigger)) { return; }

	const auto m	= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto pos	= VTransform(trigger_offset_pos, m);

	std::dynamic_pointer_cast<Sphere>(GetCollider(ColliderKind::kAttackTrigger)->GetShape())->SetPos(pos);
}
