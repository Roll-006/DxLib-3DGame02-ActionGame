#pragma once
#include "../Base/weapon_base.hpp"

class Knife: public WeaponBase
{
public:
	Knife();
	~Knife() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	[[nodiscard]] ItemKind GetItemKind() const override { return m_item_kind; }

private:
	static constexpr VECTOR kOffsetPos		= { -5.0f, -20.0f, 35.0f };
	static constexpr VECTOR kOffsetAngle	= { -90.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kOffsetScale	= 2.0f;

	ItemKind m_item_kind;
};
