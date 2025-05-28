#pragma once
#include "../Part/modeler.hpp"
#include "../Part/animator.hpp"

#include "../Object/camera.hpp"

#include "../Data/Kind/player_kind.hpp"

class Player final : public PhysicalObjBase
{
public:
	Player(std::shared_ptr<Camera> camera);
	~Player();

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;
	void OnGravity() override;

	void ChangeAnimState();

	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler()  const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<Animator>	GetAnimator() const { return m_animator; }

private:
	void Move();

	void CalcHorizontalVelocity();
	void CalcVerticalVelocity();
	void CalcMoveSpeed(const float input_slope);

	/// @brief 加速処理
	void Acceleration(const float destination_speed);
	/// @brief 減速処理
	void Deceleration(const float destination_speed);

private:
	static constexpr float kWalkSpeed			= 10.0f;
	static constexpr float kJogSpeed			= 30.0f;
	static constexpr float kRunSpeed			= 60.0f;
	static constexpr float kAcceleration		= 5.0f;		// 加速度(減速度も共通)
	static constexpr int   kWalkStickSlopeLimit	= 15000;	// 歩き状態とするスティック傾きの上限
	static constexpr int   kJogStickSlopeLimit	= 20000;	// ジョギング状態とするスティック傾きの上限

	std::shared_ptr<Modeler>  m_modeler;
	std::shared_ptr<Animator> m_animator;

	std::shared_ptr<Camera>	  m_camera;

	VECTOR m_dir;
	VECTOR m_velocity;
	float  m_move_speed;
	bool   m_is_move;
	bool   m_is_run;
};
