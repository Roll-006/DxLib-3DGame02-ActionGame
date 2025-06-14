#include "skydome.hpp"

Skydome::Skydome(std::shared_ptr<Camera> camera) :
	ObjBase				(ObjName.NIGHT_SKY, ObjTag.SKYDOME),
	m_modeler			(std::make_shared<Modeler>(m_transform, ModelPath.NIGHT_SKYDOME_01, true)),
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
	const VECTOR pos = m_camera_transform->GetPos(CoordinateKind::kWorld);
	m_transform->SetPos(CoordinateKind::kWorld, pos);
}

void Skydome::Draw() const
{
	m_modeler->Draw();
}
