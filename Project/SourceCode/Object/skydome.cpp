#include "skydome.hpp"

Skydome::Skydome(std::shared_ptr<MainCamera> camera) :
	ObjBase				(ObjName.NIGHT_SKY, ObjTag.SKYDOME),
	m_modeler			(std::make_shared<Modeler>(m_transform, ModelPath.NIGHT_SKYDOME, kBasicAngle, kBasicScale)),
	m_camera_transform	(camera->GetTransform())
{

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
}

void Skydome::LateUpdate()
{
	if (!IsActive()) { return; }

	const VECTOR pos = m_camera_transform->GetPos(CoordinateKind::kWorld);
	m_transform->SetPos(CoordinateKind::kWorld, pos);
}

void Skydome::DrawToShadowMap() const
{
	// ˆ—‚È‚µ
}

void Skydome::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}
