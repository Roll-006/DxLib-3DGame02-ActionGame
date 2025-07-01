#include "weapon_base.hpp"

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
