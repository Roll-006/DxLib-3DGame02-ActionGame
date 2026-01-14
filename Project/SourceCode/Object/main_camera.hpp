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
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	void ApplyMatrix(const MATRIX& matrix);
	void SetAimPos(const VECTOR& aim_pos) { m_aim_pos = aim_pos; }


	#pragma region Getter
	[[nodiscard]] const float	GetDeltaTime()  const override;
	#pragma endregion

private:
	#pragma region Event
	void CreateGrabCollider(const GrabEvent&	event);
	void CreateGrabCollider(const OnGrabEvent&	event);
	void DeleteGrabCollider(const ReleaseEvent& event);
	#pragma endregion


	/// @brief 見る方向を設定
	void UpdatePosture(const VECTOR& target);

	/// @brief レイキャストが発生
	void OnRayCast(const VECTOR& intersection);

	/// @brief レイキャスト発生(掴み時)
	void OnRayCastGrabCutscene(const VECTOR& intersection);

	void CalcRayCastPos();
	void CalcVisionTriggerPos();
	void CalcGrabColliderPosAndRadius();

private:
	static constexpr float kMeleeTargetDistance		= 400.0f;
	static constexpr float kMeleeCandidateDistance	= 150.0f;
	static constexpr float kMeleeTargetFOV			= 35.0f;
	static constexpr float kMeleeCandidateFOV		= 40.0f;

	VECTOR	m_aim_pos;
	float	m_collider_radius;
	bool	m_is_active_grab_collider;
	int		m_grabbed_obj_handle;

	inline static float grab_ray_length = VSize({10, 50, 0});
};
