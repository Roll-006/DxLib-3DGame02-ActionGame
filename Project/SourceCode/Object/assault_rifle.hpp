#pragma once
#include "../Base/gun_base.hpp"
#include "assault_rifle_magazine.hpp"

class AssaultRifle final : public GunBase
{
public:
	AssaultRifle();
	~AssaultRifle() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void CalcDiffusionRange() override;
	void CalcTargetPos()	  override;

private:
	static constexpr VECTOR kBasicAngle				= { 0.0f, 0.0f, 0.0f };
	static constexpr float  kBasicScale				= 5.2f;

	static constexpr VECTOR kHoldOffsetPos			= { -3.0f, 3.0f, 23.0f };
	static constexpr VECTOR kHoldOffsetAngle		= { 270.0f * math::kDegToRad, 270.0f * math::kDegToRad, 0.0f };
	static constexpr float  kHoldOffsetScale		= 1.0f;
	static constexpr VECTOR kAttachOffsetPos		= { 15.0f, -5.0f, -5.0f };
	static constexpr VECTOR kAttachOffsetAngle		= { 45.0f * math::kDegToRad, 270.0f * math::kDegToRad, 0.0f };
	static constexpr float  kAttachOffsetScale		= 1.0f;

	static constexpr VECTOR kMuzzleOffsetPos		= { 0.0f, 10.0f, 48.0f };
	static constexpr VECTOR kEjectionPortOffsetPos  = { 2.0f, 10.0f, 0.15f };
	static constexpr VECTOR kLoadPortOffsetPos		= { 0.0f, -10.0f, 0.0f };
	static constexpr float  kScopeScale				= 2.0f;
	static constexpr float  kRange					= 2000.0f;
	static constexpr float  kInitialVelocity		= 2500.0f;
	static constexpr float  kDeceleration			= 0.1f;
	static constexpr float  kShotIntervalTime		= 0.1f;

	static constexpr float  kDiffusionRadius		= 50.0f;		// ŠgŽU”ÍˆÍ‚Ì”¼Œa
};
