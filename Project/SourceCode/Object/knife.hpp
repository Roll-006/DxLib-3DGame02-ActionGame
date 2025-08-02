#pragma once
#include "../Base/knife_base.hpp"

class Knife final : public KnifeBase
{
public:
	Knife();
	~Knife() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:
	static constexpr VECTOR kModelBasicAngle	= { 0.0f, 270.0f * math::kDegreesToRadian, 0.0f };
	static constexpr VECTOR kOffsetPos			= { -1.0f, -6.0f, 13.0f };
	static constexpr VECTOR kOffsetAngle		= { 0.0f, 270.0f * math::kDegreesToRadian, DX_PI_F };
	static constexpr float  kOffsetScale		= 0.1f;
};
