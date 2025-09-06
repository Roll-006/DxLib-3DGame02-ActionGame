#pragma once
#include "../Base/character_base.hpp"
#include "../Interface/i_enemy.hpp"

#include "../Part/zombie_animator.hpp"

class ZombieStateController;

class Zombie final : public CharacterBase, public IEnemy
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

	[[nodiscard]] bool IsTargetInSight(const VECTOR& target_pos) override;


	#pragma region State
	void Move();
	void TrackMove(const VECTOR& pos);

	void CalcMoveSpeed();
	void CalcMoveSpeedStop();
	void CalcMoveSpeedRun();
	void CalcMoveSpeedRunGrab();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<ZombieStateController> GetStateController() const { return m_state; }
	#pragma endregion

private:
	void OnCollideWithExplosion(const std::shared_ptr<Sphere> sphere);
	void OnCollideWithBullet();

	void CalcLookDir();
	void CalcMoveVelocity();

private:
	static constexpr VECTOR kBasicAngle				= { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale				= 0.25f;

	static constexpr float kFOV						= 80.0f;
	static constexpr float kVisibleDistance			= 300.0f;

	static constexpr float kInvincibleTime			= 0.5f;

	static constexpr float kWalkSpeed				= 30.0f;
	static constexpr float kRunSpeed				= 70.0f;
	static constexpr float kRunGrabSpeed			= 80.0f;

	static constexpr float kCapsuleRadius			= 8.0f;
	static constexpr float kLandingTriggerRadius	= 6.0f;
	static constexpr float kHeadTriggerRadius		= 5.0f;
	static constexpr float kUpBodyTriggerRadius		= 6.0f;
	static constexpr float kDownBodyTriggerRadius	= 6.0f;
	static constexpr float kUpperArmTriggerRadius	= 2.5f;
	static constexpr float kForearmTriggerRadius	= 2.2f;
	static constexpr float kHandTriggerRadius		= 2.0f;
	static constexpr float kUpLegTriggerRadius		= 3.5f;
	static constexpr float kDownLegTriggerRadius	= 2.5f;

private:
	std::shared_ptr<ZombieStateController> m_state;

	float m_move_speed;
};
