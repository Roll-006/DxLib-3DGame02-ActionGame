#pragma once
#include "../Base/gun_base.hpp"

class RocketLauncher final : public GunBase
{
public:
	RocketLauncher();
	~RocketLauncher() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void CalcDiffusionRange() override;
	void CalcTargetPos()	  override;

	[[nodiscard]] VECTOR GetExhaustVentMatrix();

private:
	static constexpr VECTOR kBasicAngle				= { 0.0f, 90.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kBasicScale				= 1.2f;

	static constexpr VECTOR kHoldOffsetPos			= { -3.0f, 11.0f, -22.5f };
	static constexpr VECTOR kHoldOffsetAngle		= { 270.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kHoldOffsetScale		= 1.0f;
	static constexpr VECTOR kAttachOffsetPos		= { -17.0f, -7.0f, 0.0f };
	static constexpr VECTOR kAttachOffsetAngle		= { 270.0f * math::kDegreesToRadian, 90.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kAttachOffsetScale		= 1.0f;

	static constexpr VECTOR kMuzzleOffsetPos		= { 0.0f, 0.0f, 65.0f };
	static constexpr VECTOR kExhaustVentOffsetPos	= { 0.0f, 0.0f, -100.0f };
	static constexpr float  kScopeScale				= 2.0f;
	static constexpr float  kRange					= 5000.0f;
	static constexpr float  kInitialVelocity		= 40.0f;
	static constexpr float  kDeceleration			= 10.0f;
	static constexpr float  kShotIntervalTime		= 1.5f;

	static constexpr float  kDiffusionRadius		= 10.0f;		// ŠgŽU”ÍˆÍ‚Ì”¼Œa
};
