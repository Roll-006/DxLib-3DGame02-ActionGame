#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_poolable.hpp"

#include "../Part/modeler.hpp"

class GunBase;

class ShellCasing final : public PhysicalObjBase, public IPoolable
{
public:
	ShellCasing(const std::string& file_path);
	~ShellCasing() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	/// @brief ñÚ‰∞ÇîrèoÇ∑ÇÈ
	void Eject(GunBase& gun);

	[[nodiscard]] bool IsReturnPool() override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	void Move();

	void CalcColliderPos();
	void CalcProjectRayPos();

	[[nodiscard]] float	GetDeltaTime() const override;

private:
	static constexpr VECTOR kBasicAngle					= { 90.0f * math::kDegToRad, 0.0f, 0.0f };
	static constexpr float  kBasicScale					= 0.4f;

	static constexpr float  kDisappearTime				= 1.5f;						// è¡Ç¶ÇÈÇ‹Ç≈ÇÃéûä‘
	static constexpr VECTOR kLocalFirstMoveDir			= { 1.0f, 1.0f, 0.0f };		// èâä˙ÇÃà⁄ìÆï˚å¸
	static constexpr float  kInitialVelocity			= 100.0f;					// èâë¨
	static constexpr float  kDeceleration				= 2.0f;						// å∏ë¨ìx

	static constexpr float  kColliderRadius				= 0.9f;
	static constexpr float  kCollisionAreaRadius		= 6.0f;
	static constexpr VECTOR kCollisionAreaOffset		= { 0.0f, 0.0f, 0.0f };
	static constexpr float  kLandingTriggerRadius		= 0.9f;
	static constexpr VECTOR kLandingTriggerOffsetPos	= { 0.0f, -1.5f, 0.0f };
	static constexpr float  kRayLength					= 4.0f;

private:
	std::shared_ptr<Modeler> m_modeler;
	std::string m_time_scale_owner_name;

	VECTOR m_move_dir;
	float  m_alive_timer;

	float  m_move_speed;

	bool   m_prev_on_ground;
};
