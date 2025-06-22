#pragma once
#include "../Base/chara_base.hpp"
#include "../Manager/physics_manager.hpp"

class Enemy final : public CharaBase
{
public:
	Enemy();
	~Enemy() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;

private:
	void LoadAnim() override;
	void ChangeAnimState() override;

	/// @brief トランスフォーム情報を更新する
	void UpdateTransform();

private:
	static constexpr float kModelScale				= 0.25f;

	static constexpr float kCapsuleRadius			= 8.0f;
	static constexpr float kLandingTriggerRadius	= 6.0f;

private:
	VECTOR m_dir;
};