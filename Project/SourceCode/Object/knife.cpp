#include "knife.hpp"

Knife::Knife() : 
	KnifeBase(ObjName.KNIFE, HolsterKind::kKnife)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/knife_data.json", j_data))
	{
		const auto knife = j_data.at("knife_data").at("knife");

		mass_kind			= knife.at("mass_kind");
		basic_angle			= knife.at("basic_angle").get<VECTOR>() * math::kDegToRad;
		basic_scale			= knife.at("basic_scale");
		power				= knife.at("power");
		hold_offset_pos		= knife.at("hold_offset_pos");
		hold_offset_angle	= knife.at("hold_offset_angle").get<VECTOR>() * math::kDegToRad;
		hold_offset_scale	= knife.at("hold_offset_scale");
		attach_offset_pos	= knife.at("attach_offset_pos");
		attach_offset_angle	= knife.at("attach_offset_angle").get<VECTOR>() * math::kDegToRad;
		attach_offset_scale	= knife.at("attach_offset_scale");
		weapon_kind			= knife.at("weapon_kind");
		holster_kind		= knife.at("holster_kind");
		trigger_offset_pos	= knife.at("trigger_offset_pos");
		trigger_radius		= knife.at("trigger_radius");

		m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.KNIFE, basic_angle, basic_scale);
		SetColliderModelHandle(m_modeler->GetModelHandle());
	}
}

Knife::~Knife()
{

}

void Knife::Init()
{

}

void Knife::Update()
{
	if (!IsActive()) { return; }
}

void Knife::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAttackTriggerPos();
}

void Knife::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//DrawColliders();
}

void Knife::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
