#pragma once
#include "../Base/gun_base.hpp"

class AssaultRifle final : public GunBase
{
public:
	AssaultRifle();
	~AssaultRifle() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:
	static constexpr VECTOR kCorrectPos				= { -5.0f, -20.0f, 35.0f };
	static constexpr VECTOR kCorrectAngle			= { -90.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f };
	static constexpr float  kCorrectScale			= 2.0f;
	static constexpr VECTOR kMuzzleCorrectPos		= { 0.0f, 15.0f, 32.5f };
	static constexpr VECTOR kEjectionPortCorrectPos = { 1.5f, 15.0f, -2.5f };
	static constexpr float  kScopeScale				= 2.0f;
	static constexpr float  kRange					= 2000.0f;
};
