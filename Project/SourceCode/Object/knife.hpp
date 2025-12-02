#pragma once
#include "../Base/knife_base.hpp"

class Knife final : public KnifeBase
{
public:
	Knife();
	~Knife() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:
	static constexpr VECTOR kBasicAngle			= { 0.0f, 270.0f * math::kDegToRad, 0.0f };
	static constexpr float  kBasicScale			= 0.04f;

	static constexpr VECTOR kHoldOffsetPos		= { -1.0f, -6.0f, 13.0f };
	static constexpr VECTOR kHoldOffsetAngle	= { 0.0f, 270.0f * math::kDegToRad, DX_PI_F };
	static constexpr float  kHoldOffsetScale	= 1.0f;
	static constexpr VECTOR kAttachOffsetPos	= { 18.0f, -8.0f, 0.0f };
	static constexpr VECTOR kAttachOffsetAngle	= { 270.0f * math::kDegToRad, 90.0f * math::kDegToRad, 0.0f };
	static constexpr float  kAttachOffsetScale	= 0.6f;

	static constexpr float  kPower				= 30.0f;

	static constexpr VECTOR kTriggerOffsetPos	= { 0.0f, 0.0f, 15.0f };
	static constexpr float  kTriggerRadius		= 5.0f;
};
