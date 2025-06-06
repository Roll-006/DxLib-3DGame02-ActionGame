#pragma once
#include "../Manager/obj_manager.hpp"
#include "../Manager/input_checker.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class Camera final : public PhysicalObjBase
{
public:
	enum class InputDir
	{
		kUp,
		kDown,
		kLeft,
		kRight,
	};

public:
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
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void InitAngle();
	void Approach();
	void Depart();
	#pragma endregion

private:
	void Move();
	void CalcAngle();

	void SetLookDir();

	/// @brief 操作時の反転処理を適用する
	void ApplyInvert();

	void CalcDirFromPad();
	void CalcDirFromMouse();

private:
	static constexpr float kNear					= 10.0f;
	static constexpr float kFar						= 3000.0f;
	static constexpr float kFOV						= 60.0f;

	static constexpr float kMaxVerticalAngle		= 89.0f;
	static constexpr float kMinVerticalAngle		= -70.0f;
	static constexpr float kNormalDistance			= 500.0f;

	static constexpr float kMoveSpeedWithButton		= 2.0f;
	static constexpr float kMoveSpeedWithStick		= 2.5f;
	static constexpr float kMoveSpeedWithMouse		= 0.1f;
	static constexpr float kApproachSpeed			= 800.0f;

	static constexpr float kInitAngleTolerance		= 0.01f;		// 視点リセットが完了したと判定させる許容値

	std::shared_ptr<Transform> m_target_transform;

	float  m_distance_to_target;
	bool   m_is_invert_horizontal;		// 操作時に左右反転を行うかを判定
	bool   m_is_invert_vertical;		// 操作時に上下反転を行うかを判定
	
	VECTOR m_dir;
	VECTOR m_velocity;
	VECTOR m_angle;
	std::array<bool, 4> m_is_input;
};
