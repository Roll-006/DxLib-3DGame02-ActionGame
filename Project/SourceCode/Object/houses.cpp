#include "houses.hpp"
#include "../Manager/game_manager.hpp"

Houses::Houses() :
	ObjBase	(ObjName.HOUSES, ObjTag.BUILDING),
	m_modeler(std::make_shared<Modeler>(m_transform, "Data/Model/Building/mesh.mv1", kBasicAngle, kBasicScale))
{	
	m_transform->SetPos (CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();
}

Houses::~Houses()
{

}

void Houses::Init()
{

}

void Houses::Update()
{
	if (!IsActive()) { return; }
}

void Houses::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Houses::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Houses::AddToObjManager()
{
	ObjManager::GetInstance()->AddObj(shared_from_this());
}

void Houses::RemoveToObjManager()
{
	ObjManager::GetInstance()->RemoveObj(GetObjHandle());
}

float Houses::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
