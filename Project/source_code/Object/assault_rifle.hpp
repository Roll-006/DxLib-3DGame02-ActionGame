#pragma once
#include "../Base/gun_base.hpp"

class AssaultRifle final : public GunBase
{
public:
	AssaultRifle();
	~AssaultRifle();

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void TrackOwner() override;

private:
	static constexpr VECTOR angle  = VECTOR(-90.0f * math::kDegreesToRadian, 270.0f * math::kDegreesToRadian, 0.0f);
	static constexpr VECTOR offset = VECTOR(-5.0f, -20.0f, 35.0f);
};
