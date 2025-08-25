#include "ground.hpp"

Ground::Ground() : 
	PhysicalObjBase	(ObjName.GROUND, ObjTag.GROUND, MassKind::kStatic),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.GROUND_01, kBasicAngle, kBasicScale))
{	
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos (CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));

	//t1 = std::make_shared<Triangle>(VGet(200, 0, 0), VGet(0, 0, 200), VGet(-1000, 0, -500));
	t2 = std::make_shared<Triangle>(VGet(100, 0, 0), VGet(-50, 50, 200), VGet(-1000, 200, -500));
	//t3 = std::make_shared<Triangle>(VGet(, , ), VGet(, , ), VGet(, , ));
	//t4 = std::make_shared<Triangle>(VGet(, , ), VGet(, , ), VGet(, , ));
	//AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, t1, this));
	//AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, t2, this));
	//AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, t3, this));
	//AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, t4, this));
}

Ground::~Ground()
{

}

void Ground::Init()
{

}

void Ground::Update()
{
	if (!IsActive()) { return; }
}

void Ground::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Ground::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void Ground::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//for (const auto& collider : m_collider)
	//{
	//	const auto shape = collider->GetShape();
	//	if (shape != nullptr)
	//	{
	//		shape->Draw(true, 200, 0xffffff);
	//	}
	//}
}

void Ground::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void Ground::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

float Ground::GetDeltaTime() const
{
const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleController::LayerKind::kWorld);
}
