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
	void CalcMoveSpeedRun();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] 
	[[nodiscard]] std::shared_ptr<ZombieStateController> GetStateController() const { return m_state; }
	#pragma endregion

private:
	void OnCollideWithExpolsion(const std::shared_ptr<Sphere> sphere);

	void CalcLookDir();
	void CalcMoveVelocity();

private:
	static constexpr VECTOR kBasicAngle				= { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale				= 0.25f;

	static constexpr float kInvincibleTime			= 0.5f;

	static constexpr float kWalkSpeed			= 40.0f;
	static constexpr float kRunSpeed 			= 70.0f;

	static constexpr float kCapsuleRadius			= 8.0f;
	static constexpr float kHeadTriggerRadius		= 4.5f;
	static constexpr float kBodyTriggerRadius		= 6.0f;
	static constexpr float kLandingTriggerRadius	= 6.0f;
	static constexpr float kUpLegTriggerRadius		= 3.5f;
	static constexpr float kDownLegTriggerRadius	= 2.5f;

private:

	std::shared_ptr<ZombieStateController> m_state;

	VECTOR m_move_dir;
	VECTOR m_look_dir;

	float m_move_speed;
};
