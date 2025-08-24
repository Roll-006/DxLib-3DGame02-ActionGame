#include "ground.hpp"

Ground::Ground() : 
	PhysicalObjBase	(ObjName.GROUND, ObjTag.GROUND, MassKind::kStatic),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.GROUND_01, kBasicAngle, kBasicScale))
{	
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos (CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));

	t = std::make_shared<Triangle>(VGet(300, 0, 0), VGet(0, 0, 300), VGet(-300, 0, 0));
	//AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, t, this));
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
	t->Draw(true, 200, 0xffffff);
}

void Ground::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	t->Draw(true, 200, 0xffffff);
	DrawSphere3D(t->GetPos(0), 3, 8, 0xffffff, 0xffffff, TRUE);
	DrawSphere3D(t->GetPos(1), 3, 8, 0xff0000, 0xff0000, TRUE);
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
