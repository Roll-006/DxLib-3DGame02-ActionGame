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
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() { return m_modeler; }

private:
	void Move();

	/// @brief ¶‘¶‚Ì”»’è
	void JudgeAlive();

	void CalcColliderPos();

private:
	static constexpr float  kDisappearTime				= 3.0f;						// Á‚¦‚é‚Ü‚Å‚ÌŠÔ
	static constexpr VECTOR kLocalFirstMoveDir			= { 1.0f, 1.0f, 0.0f };		// ‰Šú‚ÌˆÚ“®•ûŒü
	static constexpr float  kInitialVelocity			= 1.5f;						// ‰‘¬
	static constexpr float  kDeceleration				= 0.5f;						// Œ¸‘¬“x

	static constexpr float  kCapsuleLength				= 2.0f;
	static constexpr float  kCapsuleRadius				= 0.5f;
	static constexpr float  kLandingTriggerRadius		= 0.25f;
	static constexpr VECTOR kLandingTriggerCorrectPos	= { 0.0f, -0.35f, 0.0f };

private:
	std::shared_ptr<Modeler> m_modeler;

	VECTOR m_move_dir;
	float  m_alive_timer;
	float  m_move_speed;
	bool   m_is_alive;
};
