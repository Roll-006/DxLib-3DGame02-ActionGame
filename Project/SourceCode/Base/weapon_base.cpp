#include "weapon_base.hpp"

WeaponBase::WeaponBase(const std::string& name, const std::string& file_path) :
	PhysicalObjBase	(name, ObjTag.WEAPON, MassKind::kLight),
	m_modeler		(std::make_shared<Modeler>(m_transform, file_path, VGet(0.0f, DX_PI_F, 0.0f))),
	m_owner_modeler	(nullptr),
	m_correct_pos	(v3d::GetZeroV()),
	m_correct_angle	(v3d::GetZeroV()),
	m_correct_scale	(v3d::GetZeroV())
{
	SetModelHandle(m_modeler->GetModelHandle());
}

void WeaponBase::TrackOwner()
{
	if (!m_owner_modeler) { return; }

	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const int    owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_HAND);
	const MATRIX owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	MATRIX correct_angle_mat = MGetIdent();
	CreateRotationXYZMatrix(&correct_angle_mat, m_correct_angle.x, m_correct_angle.y, m_correct_angle.z);
	const MATRIX result_mat = correct_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos   (CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(m_correct_pos, result_mat));
	m_transform->SetScale (CoordinateKind::kWorld, m_correct_scale);
}
