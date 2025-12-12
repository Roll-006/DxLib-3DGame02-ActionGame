#pragma once
#include "../Base/gun_base.hpp"
#include "non_collide_rocket_bomb.hpp"
#include "rocket_bomb_box.hpp"
#include "../JSON/json_loader.hpp"

class RocketLauncher final : public GunBase
{
public:
	RocketLauncher();
	~RocketLauncher() override;


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

	[[nodiscard]] std::shared_ptr<Transform> GetExhaustVentTransform() const { return m_exhaust_vent_transform; }

private:
	float  cross_hair_max_radius;
	VECTOR muzzle_offset;
	VECTOR exhaust_vent_offset;
	VECTOR load_port_offset;

	std::shared_ptr<Transform> m_exhaust_vent_transform;
	bool m_is_draw_magazine;

	friend void from_json(const nlohmann::json& j_data, RocketLauncher& rocket_launcher);
	friend void to_json  (nlohmann::json& j_data, const RocketLauncher& rocket_launcher);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, RocketLauncher& rocket_launcher)
{
	from_json(j_data, static_cast<GunBase&>(rocket_launcher));

	j_data.at("cross_hair_max_radius").get_to(rocket_launcher.cross_hair_max_radius);
}

inline void to_json(nlohmann::json& j_data, const RocketLauncher& rocket_launcher)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const GunBase&>(rocket_launcher));

	nlohmann::json derived_json =
	{
		{ "cross_hair_max_radius", rocket_launcher.cross_hair_max_radius },
	};

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
