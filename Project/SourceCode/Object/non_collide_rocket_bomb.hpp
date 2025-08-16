#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_bullet.hpp"
#include "../Interface/i_poolable.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

#include "../Part/modeler.hpp"

class NonCollildeRocketBomb final : public PhysicalObjBase, public IPoolable, public IBullet
{
public:
	NonCollildeRocketBomb();
	~NonCollildeRocketBomb() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;

	/// @brief 弾丸が発射された
	void OnShot(GunBase& gun)		override;

	/// @brief リロードさせた
	void OnReload(const std::shared_ptr<Modeler> owner_modeler);

	/// @brief リロードが完了した
	void CompletedReload();

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] bool  IsReturnPool() override;

private:
	/// @brief 移動方向を姿勢に適用する
	void ApplyMoveDirToRot();
	void Move();
	void CalcRayPos() override;

	void TrackOwnerHand();
	void TrackMuzzle();

private:
	static constexpr VECTOR kBasicAngle = { 0.0f, 90.0f * math::kDegToRad, 0.0f };
	static constexpr float  kBasicScale = 1.2f * 0.3f;

	static constexpr VECTOR kHoldOffsetAngle	= {};			// 手に持たれる際のオフセット角度
	static constexpr VECTOR kHoldOffsetPos		= {};			// 手に持たれる際のオフセット座標
	static constexpr VECTOR kHoldOffsetScale	= {};			// 手に持たれる際のオフセットスケール
	static constexpr VECTOR kMuzzleOffsetPos	= {};			// 銃口のオフセット座標
	static constexpr VECTOR kMuzzleOffsetAngle	= {};			// 銃口のオフセット角度
	static constexpr VECTOR kMuzzleOffsetScale	= {};			// 銃口のオフセットスケール

	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Modeler>	m_owner_modeler;
	std::string					m_time_scale_owner_name;

	VECTOR m_move_dir;
	VECTOR m_first_pos;					// 初期発射位置
	float  m_move_speed;				// 移動速度
	float  m_deceleration;				// 減速度
};
