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


	#pragma region Event
	void CreateGrabCollider(const GrabEvent&	event);
	void DeleteGrabCollider(const ReleaseEvent& event);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime()  const override;
	#pragma endregion

private:
	/// @brief 見る方向を設定
	void SetAim();

	/// @brief レイキャストトリガーの位置を計算
	void CalcRayCastPos();
	void CalcVisionTriggerPos();
	void CalcGrabColliderPosAndRadius();

private:
	static constexpr float kMeleeDistance	= 150.0f;
	static constexpr float kMeleeFOV		= 60.0f;

	VECTOR	m_aim_pos;
	float	m_collider_radius;
	bool	m_is_active_grab_collider;
};
