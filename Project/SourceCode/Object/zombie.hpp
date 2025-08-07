#pragma once
#include "../Base/enemy_base.hpp"

class Zombie final : public EnemyBase
{
public:
	Zombie();
	~Zombie() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:
	void Move();

private:
	static constexpr VECTOR kBasicAngle				= { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale				= 0.25f;

	static constexpr float kCapsuleRadius			= 8.0f;
	static constexpr float kHeadTriggerRadius		= 4.5f;
	static constexpr float kBodyTriggerRadius		= 6.0f;
	static constexpr float kLandingTriggerRadius	= 6.0f;
	static constexpr float kUpLegTriggerRadius		= 3.5f;
	static constexpr float kDownLegTriggerRadius	= 2.5f;

private:

};