#pragma once
#include "../Base/gun_base.hpp"
#include "assault_rifle_magazine.hpp"
#include "assault_rifle_ammo_box.hpp"
#include "../JSON/json_loader.hpp"

class AssaultRifle final : public GunBase
{
public:
	AssaultRifle();
	~AssaultRifle() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void InitCrossHairRange() override;
	void CalcCrossHairRange(const VECTOR& owner_move_velocity) override;
	void CalcCrossHairRangeShot()	override;
	void CalcCrossHairPos()			override;
	void CalcTargetPos()			override;
	void CalcTransforms()			override;

private:
	float  cross_hair_max_radius;
	float  cross_hair_middle_radius;
	float  cross_hair_min_radius;
	VECTOR muzzle_offset;
	VECTOR ejection_port_offset;
	VECTOR load_port_offset;

	float  m_cross_hair_radius;

	friend void from_json(const nlohmann::json& j_data, AssaultRifle& assaultRifle);
	friend void to_json  (nlohmann::json& j_data, const AssaultRifle& assaultRifle);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, AssaultRifle& assaultRifle)
{
	from_json(j_data, static_cast<GunBase&>(assaultRifle));

	j_data.at("cross_hair_max_radius")	.get_to(assaultRifle.cross_hair_max_radius);
	j_data.at("cross_hair_middle_radius")	.get_to(assaultRifle.cross_hair_middle_radius);
	j_data.at("cross_hair_min_radius")	.get_to(assaultRifle.cross_hair_min_radius);
	j_data.at("muzzle_offset")			.get_to(assaultRifle.muzzle_offset);
	j_data.at("ejection_port_offset")		.get_to(assaultRifle.ejection_port_offset);
	j_data.at("load_port_offset")			.get_to(assaultRifle.load_port_offset);
}

inline void to_json(nlohmann::json& j_data, const AssaultRifle& assaultRifle)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const GunBase&>(assaultRifle));

	nlohmann::json derived_json =
	{
		{ "cross_hair_max_radius",		assaultRifle.cross_hair_max_radius },
		{ "cross_hair_middle_radius",	assaultRifle.cross_hair_middle_radius },
		{ "cross_hair_min_radius",		assaultRifle.cross_hair_min_radius },
		{ "muzzle_offset",				assaultRifle.muzzle_offset },
		{ "ejection_port_offset",		assaultRifle.ejection_port_offset },
		{ "load_port_offset",			assaultRifle.load_port_offset },
	};

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
