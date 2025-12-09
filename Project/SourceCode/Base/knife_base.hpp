#pragma once
#include "../Base/weapon_base.hpp"

class KnifeBase abstract : public WeaponBase
{
public:
	KnifeBase(const std::string& name, const HolsterKind holster_kind);
	virtual ~KnifeBase() = default;

	/// @brief 持ち主の手を追跡する
	void TrackOwnerHand() override;
	/// @brief 持ち主のホルスター(武器収納位置)を追尾する
	void TrackOwnerHolster()override;

protected:
	void AddAttackTrigger();
	void RemoveAttackTrigger();
	void CalcAttackTriggerPos();

protected:
	VECTOR trigger_offset_pos;
	float  trigger_radius;

	friend void from_json(const nlohmann::json& data, KnifeBase& knife_base);
	friend void to_json  (nlohmann::json& data, const KnifeBase& knife_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, KnifeBase& knife_base)
{
	from_json(data, static_cast<WeaponBase&>(knife_base));

	data.at("trigger_offset_pos")	.get_to(knife_base.trigger_offset_pos);
	data.at("trigger_radius")		.get_to(knife_base.trigger_radius);
}

inline void to_json(nlohmann::json& data, const KnifeBase& knife_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const WeaponBase&>(knife_base));

	nlohmann::json derived_json =
	{
		{ "trigger_offset_pos",	knife_base.trigger_offset_pos },
		{ "trigger_radius",		knife_base.trigger_radius },
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
