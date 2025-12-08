#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_poolable.hpp"
#include "../Interface/i_bullet.hpp"

#include "../Manager/obj_manager.hpp"
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
	
	/// @brief íeä€Ç™î≠éÀÇ≥ÇÍÇΩ
	void OnShot(GunBase& gun) override;

	[[nodiscard]] float			GetDeltaTime()		const	override;
	[[nodiscard]] float			GetPower()			const	override { return m_power; }
	[[nodiscard]] std::string	GetShotOwnerName()	const	override { return m_shot_owner_name; }
	[[nodiscard]] bool			IsReturnPool()				override;

private:
	void Move();
	void CalcRayCastPos() override;

private:
	static constexpr float kCollisionAreaRadius	= 5.0f;

	std::string m_shot_owner_name;

	VECTOR m_move_dir;
	VECTOR m_prev_pos;
	VECTOR m_first_pos;		// èâä˙î≠éÀà íu
	float  m_move_speed;	// à⁄ìÆë¨ìx
	float  m_deceleration;	// å∏ë¨ìx
	float  m_range;			// éÀíˆ
	float  m_power;
};
