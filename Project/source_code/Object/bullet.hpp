#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

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
	/// @param pos ‰ŠúÀ•W
	/// @param dir ”­Ë•ûŒü
	/// @param initial_velocity ‰‘¬
	void OnShot(const VECTOR& pos, const VECTOR& dir, const float initial_velocity);

private:
	void Move();
	void CalcRayPos();

private:
	VECTOR m_dir;
	VECTOR m_prev_pos;
	float  m_move_speed;
};
