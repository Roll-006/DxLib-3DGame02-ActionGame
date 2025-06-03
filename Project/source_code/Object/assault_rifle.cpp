#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, ModelPath.ASSAULT_RIFLE_01)
{

}

AssaultRifle::~AssaultRifle()
{

}

void AssaultRifle::Init()
{

}

void AssaultRifle::Update()
{

}

void AssaultRifle::Draw() const
{
	m_modeler->Draw();
}

void AssaultRifle::TrackOwner()
{
	if (!m_owner_modeler) { return; }

	// アタッチする部位の行列情報を取り出す
	const int    owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_HAND);
	const MATRIX owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	MATRIX mat = MGetIdent();
	CreateRotationXYZMatrix(&mat, angle.x, angle.y, angle.z);
	const MATRIX result_mat = mat * owner_attach_frame_mat;

	m_transform->SetMatrix(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos	  (CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(offset, result_mat));
}
