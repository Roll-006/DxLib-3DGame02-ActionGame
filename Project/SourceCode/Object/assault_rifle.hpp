#pragma once
#include "../Base/gun_base.hpp"

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

private:
	void CalcDiffusionRange() override;
	void CalcTargetPos()	  override;

private:
	static constexpr VECTOR kModelBasicAngle		= { 0.0f, DX_PI_F, 0.0f };
	static constexpr VECTOR kHoldOffsetPos			= { -5.0f, -16.0f, 30.0f };
	static constexpr VECTOR kHoldOffsetAngle		= { 270.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kHoldOffsetScale		= 1.85f;
	static constexpr VECTOR kAttachOffsetPos		= { 15.0f, -30.0f, 0.0f };
	static constexpr VECTOR kAttachOffsetAngle		= { 45.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kAttachOffsetScale		= 1.7f;

	static constexpr VECTOR kMuzzleOffsetPos		= { 0.0f, 15.0f, 32.5f };
	static constexpr VECTOR kEjectionPortOffsetPos  = { 1.5f, 15.5f, -2.5f };
	static constexpr float  kScopeScale				= 2.0f;
	static constexpr float  kRange					= 2000.0f;
	static constexpr float  kInitialVelocity		= 140.0f;
	static constexpr float  kDeceleration			= 100.0f;
	static constexpr float  kShotIntervalTime		= 0.1f;

	static constexpr float  kDiffusionRadius		= 50.0f;		// ŠgŽU”ÍˆÍ‚Ì”¼Œa
};
