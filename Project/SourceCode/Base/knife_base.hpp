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
	void CreateAttackTrigger(const VECTOR& trigger_offset_pos, const float trigger_radius);
	void CalcAttackTriggerPos();

private:
	VECTOR m_trigger_offset_pos;
	float  m_trigger_radius;
};
