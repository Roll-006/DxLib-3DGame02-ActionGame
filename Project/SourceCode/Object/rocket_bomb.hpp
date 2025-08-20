#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_poolable.hpp"
#include "../Interface/i_bullet.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"
#include "../VirtualCamera/camera_brain.hpp"

#include "../Part/modeler.hpp"
#include "../Part/subject.hpp"

class RocketBomb final : public PhysicalObjBase, public IPoolable, public IBullet
{
public:
	RocketBomb();
	~RocketBomb() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;

	/// @brief 弾丸が発射された
	void OnShot(GunBase& gun)		override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] bool  IsReturnPool() override;
	[[nodiscard]] std::string GetShotOwnerName() const override { return m_shot_owner_name; }

private:
	/// @brief 移動方向を姿勢に適用する
	void ApplyMoveDirToRot();
	void Move();
	void CalcRayPos() override;

private:
	static constexpr VECTOR kBasicAngle		= { 0.0f, 90.0f * math::kDegToRad, 0.0f };
	static constexpr float  kBasicScale		= 0.3f;	// TODO : 後に武器所持者のスケールに依存するよう変更
	static constexpr float  kGoStraightTime = 0.01f;
	static constexpr float  kBlendTime		= 0.02f;

	std::shared_ptr<Modeler>				m_modeler;
	std::shared_ptr<Subject<RocketBomb>>	m_subject;
	std::string								m_shot_owner_name;

	VECTOR m_destination_dir;

	float  m_blend_timer;
	float  m_go_straight_timer;

	VECTOR m_move_dir;
	VECTOR m_prev_pos;
	VECTOR m_first_pos;			// 初期発射位置
	float  m_move_speed;		// 移動速度
	float  m_deceleration;		// 減速度
	float  m_range;				// 射程
};
