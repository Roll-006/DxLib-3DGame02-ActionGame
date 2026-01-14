#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_poolable.hpp"
#include "../Interface/i_bullet.hpp"

#include "../Accessor/obj_accessor.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class Bullet final : public PhysicalObjBase, public IPoolable, public IBullet
{
public:
	Bullet();
	~Bullet() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;
	
	/// @brief 弾丸が発射された
	void OnShot(GunBase& gun) override;

	[[nodiscard]] const float		GetDeltaTime()		const	override;
	[[nodiscard]] const float		GetPower()			const	override { return m_power; }
	[[nodiscard]] const std::string	GetShotOwnerName()	const	override { return m_shot_owner_name; }
	[[nodiscard]] const bool		IsReturnPool()				override;

private:
	void Move();
	void CalcRayCastPos() override;

private:
	static constexpr float kCollisionAreaRadius	= 5.0f;

	std::string m_shot_owner_name;

	VECTOR m_move_dir;
	VECTOR m_prev_pos;
	VECTOR m_first_pos;		// 初期発射位置
	float  m_move_speed;	// 移動速度
	float  m_deceleration;	// 減速度
	float  m_range;			// 射程
	float  m_power;
};
