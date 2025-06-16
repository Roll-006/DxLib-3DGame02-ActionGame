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
	//m_modeler->Draw();

	const VECTOR pos1 = VGet( 1000, 0, -1000);
	const VECTOR pos2 = VGet( 1000, 0,  1000);
	const VECTOR pos3 = VGet(-1000, 0,  1000);
	const VECTOR pos4 = VGet(-1000, 0, -1000);
	Square square(pos1, pos2, pos3, pos4);
	square.Draw(false, 255, 0xffffff);
}
