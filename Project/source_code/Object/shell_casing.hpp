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

	/// @brief ñÚ‰∞ÇîrèoÇ∑ÇÈ
	void Eject(GunBase& gun);

	[[nodiscard]] bool IsAlive() const;

private:
	static constexpr float kDisappearTime = 5.0f;

	float m_alive_timer;
	bool  m_is_alive;
};
