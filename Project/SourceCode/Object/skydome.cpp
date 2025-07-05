#include "skydome.hpp"

Skydome::Skydome(std::shared_ptr<Camera> camera) :
	ObjBase				(ObjName.NIGHT_SKY, ObjTag.SKYDOME),
	m_modeler			(std::make_shared<Modeler>(m_transform, ModelPath.NIGHT_SKYDOME_01, VGet(0.0f, DX_PI_F, 0.0f))),
	m_camera_transform	(camera->GetTransform())
{
	m_transform->SetScale(CoordinateKind::kWorld, kScale);
	sample_v = VGet(0, 100, 0);
}

Skydome::~Skydome()
{

}

void Skydome::Init()
{

}

void Skydome::Update()
{
	if (!IsActive()) { return; }

	const VECTOR pos = m_camera_transform->GetPos(CoordinateKind::kWorld);
	m_transform->SetPos(CoordinateKind::kWorld, pos);

	sample_v = math::GetRotatedPos(sample_v, quat::CreateQuaternion(axis::GetWorldZAxis(), 1 * FPS::GetDeltaTime()));
}

void Skydome::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Skydome::Draw() const
{
	if (!IsActive()) { return; }

	//m_modeler->Draw();
	DrawLine3D(v3d::GetZeroV(), sample_v, 0xffffff);
}
