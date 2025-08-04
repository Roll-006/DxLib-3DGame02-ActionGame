#include "skydome.hpp"

Skydome::Skydome(std::shared_ptr<MainCamera> camera) :
	ObjBase				(ObjName.NIGHT_SKY, ObjTag.SKYDOME),
	m_modeler			(std::make_shared<Modeler>(m_transform, ModelPath.NIGHT_SKYDOME, VGet(0.0f, DX_PI_F, 0.0f))),
	m_camera_transform	(camera->GetTransform())
{
	m_transform->SetScale(CoordinateKind::kWorld, kScale);
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
}

void Skydome::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Skydome::DrawToShadowMap() const
{

}

void Skydome::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}
