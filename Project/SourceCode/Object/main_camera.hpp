#pragma once
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"
#include "../Manager/effect_manager.hpp"

class CinemachineBrain;

class MainCamera final : public PhysicalObjBase
{
public:
	MainCamera();
	~MainCamera() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	void ApplyMatrix(const MATRIX& matrix);
	void SetAimPos(const VECTOR& aim_pos) { m_aim_pos = aim_pos; }

	[[nodiscard]] float	GetDeltaTime()  const override;

private:
	/// @brief 見る方向を設定
	void SetAim();

	/// @brief レイキャストトリガーの位置を計算
	void CalcRayCastPos();
	void CalcVisionTriggerPos();

private:
	static constexpr float kMeleeDistance	= 150.0f;
	static constexpr float kMeleeFOV		= 60.0f;

	VECTOR	m_aim_pos;
	VECTOR	m_origin_pos;	// レイキャス処理が実行される前の生の座標
	bool	m_is_changing_pos;
};
