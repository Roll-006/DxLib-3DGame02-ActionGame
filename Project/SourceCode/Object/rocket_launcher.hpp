#pragma once
#include "../Base/gun_base.hpp"
#include "non_collide_rocket_bomb.hpp"
#include "rocket_bomb_box.hpp"

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
	static constexpr VECTOR kBasicAngle				= { 0.0f, 90.0f * math::kDegToRad, 0.0f };
	static constexpr float  kBasicScale				= 0.45f;

	static constexpr VECTOR kHoldOffsetPos			= { -3.0f, 11.0f, -22.5f };
	static constexpr VECTOR kHoldOffsetAngle		= { 270.0f * math::kDegToRad, 270.0f * math::kDegToRad, 0.0f };
	static constexpr float  kHoldOffsetScale		= 1.0f;
	static constexpr VECTOR kAttachOffsetPos		= { -17.0f, -7.0f, 0.0f };
	static constexpr VECTOR kAttachOffsetAngle		= { 270.0f * math::kDegToRad,  90.0f * math::kDegToRad, 0.0f };
	static constexpr float  kAttachOffsetScale		= 1.0f;

	static constexpr int    kMaxRemainingBulletNum	= 1;
	static constexpr float  kPower					= 3000.0f;

	static constexpr VECTOR kMuzzleOffsetPos		= { 0.0f, 0.0f,  33.5f };
	static constexpr VECTOR kLoadPortOffsetPos		= { 0.0f, 0.0f,  33.5f };
	static constexpr VECTOR kExhaustVentOffsetPos	= { 0.0f, 0.0f, -30.0f };
	static constexpr float  kScopeScale				= 2.0f;
	static constexpr float  kRange					= 5000.0f;
	static constexpr float  kInitialVelocity		= 1500.0f;
	static constexpr float  kDeceleration			= 0.2f;
	static constexpr float  kShotIntervalTime		= 1.5f;

	static constexpr float  kCrossHairMaxRadius		= 15.0f;		// ŠgŽU”ÍˆÍ‚Ì”¼Œa

	std::shared_ptr<Transform> m_exhaust_vent_transform;
	bool m_is_draw_magazine;
};
