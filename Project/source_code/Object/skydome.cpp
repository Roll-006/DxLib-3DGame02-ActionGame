#include "skydome.hpp"

Skydome::Skydome(std::shared_ptr<Camera> camera) :
	ObjBase				(ObjName.NIGHT_SKY, ObjTag.SKYDOME),
	m_camera_transform	(camera->GetTransform())
{
	MakeModel(ModelPath.NIGHT_SKYDOME_01, VGet(0.0f, DX_PI_F, 0.0f));

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
	//m_modeler->Draw();
}
