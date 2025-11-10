#include "houses.hpp"
#include "../Manager/game_manager.hpp"

Houses::Houses() :
	PhysicalObjBase(ObjName.HOUSES, ObjTag.BUILDING),
	m_modeler			(std::make_shared<Modeler>(m_transform, "Data/Model/Building/mesh.mv1",			  kBasicAngle, kBasicScale)),
	m_collision_modeler	(std::make_shared<Modeler>(m_transform, "Data/Model/Building/collision_mesh.mv1", kBasicAngle, kBasicScale))
{	
	mass_kind = MassKind::kStatic;

	SetColliderModelHandle(m_collision_modeler->GetModelHandle());

	m_transform->SetPos (CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();
	m_collision_modeler->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_collision_modeler->GetModelHandle(), this));
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

void Houses::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void Houses::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void Houses::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj	(obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj	(obj_handle);
	ObjManager		::GetInstance()->RemoveObj			(obj_handle);
}

float Houses::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
