#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class GunBase;

class Bullet final : public PhysicalObjBase
{
public:
	Bullet();
	~Bullet() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	
	/// @brief ’eŠÛ‚ª”­Ë‚³‚ê‚½
	void OnShot(const GunBase& gun);

	[[nodiscard]] bool IsAlive() const { return m_Is_alive; }

private:
	void Move();
	void CalcRayPos();

	/// @brief ’eŠÛ‚Ì¶‘¶‚ğ”»’è
	void JudgeAlive();

private:
	VECTOR m_dir;
	VECTOR m_prev_pos;
	VECTOR m_first_pos;		// ‰Šú”­ËˆÊ’u
	float  m_move_speed;	// ˆÚ“®‘¬“x
	float  m_deceleration;	// Œ¸‘¬“x
	float  m_range;			// Ë’ö
	bool   m_Is_alive;		// ¶‘¶”»’è
};
