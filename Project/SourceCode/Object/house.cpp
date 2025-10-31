#include "house.hpp"

House::House(const std::string& house_id) :
	PhysicalObjBase			(ObjName.HOUSE, ObjTag.BUILDING),
	m_modeler				(nullptr),
	m_collision_modeler		(nullptr),
	m_model_path			(""),
	m_collision_model_path	(""),
	m_pos					(v3d::GetZeroV()),
	m_angle					(v3d::GetZeroV()),
	m_basic_scale			(1.0f),
	m_collision_area_offset	(v3d::GetZeroV()),
	m_collision_area_radius	(0.0f)
{
	mass_kind = MassKind::kStatic;

	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/houses.json", data))
	{
		m_model_path			= data.at("houses").at(house_id).at("model_path");
		m_collision_model_path	= data.at("houses").at(house_id).at("collision_model_path");
		m_pos					= data.at("houses").at(house_id).at("position").get<VECTOR>();
		m_angle					= data.at("houses").at(house_id).at("angle").get<VECTOR>();
		m_basic_scale			= data.at("houses").at(house_id).at("basic_scale");
		m_collision_area_offset = data.at("houses").at(house_id).at("collision_area_offset").get<VECTOR>();
		m_collision_area_radius = data.at("houses").at(house_id).at("collision_area_radius");
	}

	m_modeler			= std::make_shared<Modeler>(m_transform, m_model_path,			 v3d::GetZeroV(), m_basic_scale);
	m_collision_modeler = std::make_shared<Modeler>(m_transform, m_collision_model_path, v3d::GetZeroV(), m_basic_scale);

	SetColliderModelHandle(m_collision_modeler->GetModelHandle());

	m_transform->SetPos		(CoordinateKind::kWorld, m_pos);
	m_transform->SetRot		(CoordinateKind::kWorld, math::ConvertEulerAnglesToXYZRotMatrix(m_angle));
	m_modeler			->ApplyMatrix();
	m_collision_modeler	->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(m_pos + m_collision_area_offset, m_collision_area_radius), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,				m_collision_modeler->GetModelHandle(), this));

	// ‰¼‚Åƒf[ƒ^‚ð‘‚«ž‚Þ
}

House::~House()
{

}

void House::Init()
{

}

void House::Update()
{
	if (!IsActive()) { return; }
}

void House::LateUpdate()
{
	if (!IsActive()) { return; }
}

void House::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawColliders();
}

void House::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void House::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void House::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

float House::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
