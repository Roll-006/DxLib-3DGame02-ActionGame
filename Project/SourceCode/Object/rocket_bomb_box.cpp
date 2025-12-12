#include "rocket_bomb_box.hpp"

RocketBombBox::RocketBombBox() : 
	PhysicalObjBase			(ObjName.AMMO_BOX_ROCKET_BOMB, ObjTag.AMMO_BOX),
	rifle_cartridge_box_data(),
	m_have_num				(0),
	m_item_effect_transform	(std::make_shared<Transform>()),
	m_modeler				(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/ammo_box.json", j_data))
	{
		rifle_cartridge_box_data = j_data.at("ammo_box").at("rocket_bomb_box").at("rifle_cartridge_box_data").get<RifleCartridgeBoxData>();
		
		m_modeler = std::make_shared<Modeler>(m_transform, rifle_cartridge_box_data.model_path, rifle_cartridge_box_data.basic_angle, rifle_cartridge_box_data.basic_scale);
		AddCollider(std::make_shared<Collider>(ColliderKind::kProjectRay,		std::make_shared<Segment>	(m_transform->GetPos(CoordinateKind::kWorld), -axis::GetWorldYAxis(), rifle_cartridge_box_data.project_ray_length), this));
		AddCollider(std::make_shared<Collider>(ColliderKind::kVisibleTrigger,	std::make_shared<Point>		(m_transform->GetPos(CoordinateKind::kWorld)), this));
	}
}

RocketBombBox::RocketBombBox(const int ammo_num) :
	PhysicalObjBase			(ObjName.AMMO_BOX_ROCKET_BOMB, ObjTag.AMMO_BOX),
	rifle_cartridge_box_data(),
	m_have_num				(ammo_num),
	m_item_effect_transform	(std::make_shared<Transform>()),
	m_modeler				(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/ammo_box.json", j_data))
	{
		rifle_cartridge_box_data = j_data.at("ammo_box").at("rocket_bomb_box").at("rifle_cartridge_box_data").get<RifleCartridgeBoxData>();
		
		m_modeler = std::make_shared<Modeler>(m_transform, rifle_cartridge_box_data.model_path, rifle_cartridge_box_data.basic_angle, rifle_cartridge_box_data.basic_scale);
		AddCollider(std::make_shared<Collider>(ColliderKind::kProjectRay,		std::make_shared<Segment>	(m_transform->GetPos(CoordinateKind::kWorld), -axis::GetWorldYAxis(), rifle_cartridge_box_data.project_ray_length), this));
		AddCollider(std::make_shared<Collider>(ColliderKind::kVisibleTrigger,	std::make_shared<Point>		(m_transform->GetPos(CoordinateKind::kWorld)), this));
	}

	if (m_have_num > GetMaxHaveNum())
	{
		m_have_num = GetMaxHaveNum();
	}
}

RocketBombBox::~RocketBombBox()
{

}

void RocketBombBox::Init()
{

}

void RocketBombBox::Update()
{
	if (!IsActive()) { return; }

	const auto project_ray = GetCollider(ColliderKind::kProjectRay);
	if (project_ray)
	{
		const auto segment = std::static_pointer_cast<Segment>(project_ray->GetShape());
		segment->SetBeginPos(m_transform->GetPos(CoordinateKind::kWorld), false);
	}
}

void RocketBombBox::LateUpdate()
{
	if (!IsActive()) { return; }

	m_modeler->ApplyMatrix();
}

void RocketBombBox::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void RocketBombBox::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj				(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddIgnoreObjGravity(GetObjHandle());
}

void RocketBombBox::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj		(obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj		(obj_handle);
	PhysicsManager	::GetInstance()->RemoveIgnoreObjGravity	(obj_handle);
	ObjManager		::GetInstance()->RemoveObj				(obj_handle);
}

void RocketBombBox::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kProjectRay:
		if (hit_collider_pair.intersection) { m_current_project_pos = hit_collider_pair.intersection; }
		break;

	default:
		break;
	}
}

void RocketBombBox::OnProjectPos()
{
	if (!IsActive()) { return; }
	if (IsOnGround()) { return; }

	const auto project_pos = GetCurrentProjectPos();
	if (!project_pos) { return; }

	const auto hit_triangle = GetCollider(ColliderKind::kProjectRay)->GetHitTriangles();
	if (hit_triangle.size() <= 0) { return; }

	// 角度・位置を固定
	const auto transform	= GetTransform();
	const auto current_axis = transform->GetAxis(CoordinateKind::kWorld);
	const auto cross_x		= math::GetNormalVector(hit_triangle.front().GetNormalVector(), axis::GetWorldYAxis());
	const auto cross_z		= math::GetNormalVector(hit_triangle.front().GetNormalVector(), cross_x);
	const auto new_axis		= math::GetForwardSyncedAxis(current_axis, cross_z);

	m_item_effect_transform	->SetPos(CoordinateKind::kWorld, *project_pos);
	transform				->SetPos(CoordinateKind::kWorld, *project_pos);
	transform				->SetRot(CoordinateKind::kWorld, new_axis);

	// コライダーの位置更新
	const auto pos = transform->GetPos(CoordinateKind::kWorld);
	std::static_pointer_cast<Point>(m_colliders.at(ColliderKind::kVisibleTrigger)->GetShape())->SetPos(pos);
}

void RocketBombBox::Synthesize(const std::shared_ptr<IAmmoBox> ammo_box)
{
	if (IsMax()) { return; }

	const auto partner_num	= ammo_box->GetCurrentHaveNum();	// 現在の相手の数
	const auto margin_num	= GetMaxHaveNum() - m_have_num;		// 空き容量
	const auto sub_num		= partner_num - margin_num;			// あふれた数

	AddHaveNum(partner_num);

	if (sub_num > 0)
	{
		ammo_box->SetHaveNum(sub_num);
	}
}

void RocketBombBox::SetHaveNum(const int set_num)
{
	m_have_num = set_num;
	if (m_have_num > GetMaxHaveNum())
	{
		m_have_num = GetMaxHaveNum();
	}
}

void RocketBombBox::AddHaveNum(const int add_num)
{
	m_have_num += add_num;
	if (m_have_num > GetMaxHaveNum())
	{
		m_have_num = GetMaxHaveNum();
	}
}

float RocketBombBox::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
