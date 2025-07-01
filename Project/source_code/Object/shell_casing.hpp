#pragma once
#include "../Base/physical_obj_base.hpp"

class GunBase;

class ShellCasing final : public PhysicalObjBase
{
public:
	ShellCasing();
	~ShellCasing() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	/// @brief –òä°‚ğ”ro‚·‚é
	void Eject(GunBase& gun);

	[[nodiscard]] bool IsAlive() const { return m_is_alive; }

private:
	void Move();

	/// @brief ¶‘¶‚Ì”»’è
	void JudgeAlive();

	void CalcColliderPos();

private:
	static constexpr float  kDisappearTime			= 3.0f;						// Á‚¦‚é‚Ü‚Å‚ÌŠÔ
	static constexpr float  kInitialVelocity		= 100.0f;					// ‰‘¬
	static constexpr VECTOR kLocalFirstMoveDir		= { 1.0f, 1.0f, 0.0f };		// ‰Šú‚ÌˆÚ“®•ûŒü
	static constexpr float  kMoveSpeed				= 1.5f;						// ˆÚ“®‘¬“x

	static constexpr float kCapsuleRadius			= 0.3f;
	static constexpr float kCapsuleLength			= 2.0f;
	static constexpr float kLandingTriggerRadius	= 0.4f;

	float  m_alive_timer;
	bool   m_is_alive;
	VECTOR m_move_dir;
};
