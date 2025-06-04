#pragma once
#include "../Manager/obj_manager.hpp"
#include "../Manager/input_checker.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class Camera final : public PhysicalObjBase
{
public:
	enum class TimeState
	{
		kCurrect,
		kNext,
	};

	Camera();
	~Camera();

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;
	void OnGravity() override;

	void AttachTarget(const std::shared_ptr<ObjBase> obj);
	void AttachTarget(const std::string& obj_name);
	void DetachTarget();

	/// @brief ターゲットとの距離を設定する
	void SetDistanceToTarget(const float distance) { m_distance_to_target = distance; }

	#pragma region コマンド
	void Approach();
	void Depart();
	#pragma endregion

private:
	void Move();

	/// @brief 視点リセット
	void InitAngle();

	void SetLookDir();

	/// @brief 操作時の反転処理を適用する
	[[nodiscard]] VECTOR ApplyInvert(VECTOR& velocity) const;

	void CalcAngleFromPad();
	void CalcAngleFromMouse();

private:
	static constexpr float kNear					= 10.0f;
	static constexpr float kFar						= 3000.0f;
	static constexpr float kFOV						= 60.0f;
	static constexpr float kMaxVerticalAngle		= 89.0f;
	static constexpr float kMinVerticalAngle		= -70.0f;
	static constexpr float kNormalDistance			= 500.0f;
	static constexpr float kSpeedWithPad			= 2.5f;
	static constexpr float kSpeedRateWithMouse		= 0.1f;
	static constexpr float kApproachSpeed			= 800.0f;
	static constexpr float kInitAngleTolerance		= 0.01f;		// 視点リセットが完了したと判定させる許容値

	std::shared_ptr<Transform> m_target_transform;
	//std::unordered_map<TimeState, VECTOR>	m_target_pos;

	float  m_move_speed;
	float  m_distance_to_target;
	bool   m_is_invert_horizontal;		// 操作時に左右反転を行うかを判定
	bool   m_is_invert_vertical;		// 操作時に上下反転を行うかを判定
	
	VECTOR m_velocity;
	VECTOR m_angle;
	//Quaternion m_quaternion;
};
