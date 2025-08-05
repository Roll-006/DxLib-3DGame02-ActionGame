#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_poolable.hpp"

#include "../Part/modeler.hpp"

class GunBase;

class ShellCasing final : public PhysicalObjBase, public IPoolable
{
public:
	ShellCasing();
	~ShellCasing() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	/// @brief –òä°‚ğ”ro‚·‚é
	void Eject(GunBase& gun);

	[[nodiscard]] bool IsReturnPool() override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	void Move();

	void CalcColliderPos();

private:
	static constexpr float  kDisappearTime				= 3.0f;						// Á‚¦‚é‚Ü‚Å‚ÌŠÔ
	static constexpr VECTOR kLocalFirstMoveDir			= { 1.0f, 1.0f, 0.0f };		// ‰Šú‚ÌˆÚ“®•ûŒü
	static constexpr float  kInitialVelocity			= 1.7f;						// ‰‘¬
	static constexpr float  kDeceleration				= 1.5f;						// Œ¸‘¬“x

	static constexpr float  kColliderRadius				= 0.9f;
	static constexpr float  kLandingTriggerRadius		= 0.9f;
	static constexpr VECTOR kLandingTriggerOffsetPos	= { 0.0f, -1.5f, 0.0f };

private:
	std::shared_ptr<Modeler> m_modeler;

	VECTOR m_move_dir;
	float  m_alive_timer;

	float  m_move_speed;
};
