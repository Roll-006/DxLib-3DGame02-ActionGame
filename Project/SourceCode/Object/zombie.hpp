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


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime() const override;
	#pragma endregion

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
	std::shared_ptr<ZombieStateController> m_state;

	VECTOR m_move_dir;
	VECTOR m_look_dir;
};
