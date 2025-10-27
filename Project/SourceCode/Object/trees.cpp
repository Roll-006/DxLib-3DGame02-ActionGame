#include "trees.hpp"
#include "../Manager/game_manager.hpp"

Trees::Trees() :
	ObjBase	(ObjName.TREES, ObjTag.TREES),
	m_modeler(std::make_shared<Modeler>(m_transform, ModelPath.TREES, kBasicAngle, kBasicScale))
{	
	m_transform->SetPos (CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();
}

Trees::~Trees()
{

}

void Trees::Init()
{

}

void Trees::Update()
{
	if (!IsActive()) { return; }
}

void Trees::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Trees::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Trees::AddToObjManager()
{
	ObjManager::GetInstance()->AddObj(shared_from_this());
}

void Trees::RemoveToObjManager()
{
	ObjManager::GetInstance()->RemoveObj(GetObjHandle());
}

float Trees::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
