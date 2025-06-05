#pragma once
#include "../Base/chara_base.hpp"
#include "../Data/Kind/player_kind.hpp"

#include "assault_rifle.hpp"
#include "camera.hpp"

class Player final : public CharaBase
{
public:
	Player(std::shared_ptr<Camera> camera);
	~Player();

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;
	void OnGravity() override;

	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler()  const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<Animator>	GetAnimator() const { return m_animator; }

	#pragma region コマンド
	void CalcMoveForward();
	void CalcMoveBackward();
	void CalcMoveLeft();
	void CalcMoveRight();
	void Run();
	#pragma endregion

private:
	void Move();

	void CalcHorizontalVelocity();
	void CalcVerticalVelocity();
	void CalcMoveSpeed(const float input_slope);
	void CalcDir(const VECTOR& velocity);

	// 入力方式に対応した速度ベクトルを取得
	[[nodiscard]] VECTOR GetVelocityFromPad	 (const VECTOR& forwrd, const VECTOR& right);
	[[nodiscard]] VECTOR GetVelocityFromKey(const VECTOR& forwrd, const VECTOR& right);

	/// @brief 加速処理
	void Acceleration(const float destination_speed);
	/// @brief 減速処理
	void Deceleration(const float destination_speed);

	void ConvertMouseVelocityToPadVelocity();

	void LoadAnim() override;
	void ChangeAnimState() override;

private:
	static constexpr float kSlowWalkSpeed		= 2.0f;
	static constexpr float kWalkSpeed			= 4.0f;
	static constexpr float kRunSpeed			= 10.0f;
	static constexpr float kAcceleration		= 4.0f;		// 加速度(減速度も共通)
	static constexpr float kDirCorrectionSpeed  = 0.07f;	// dirの補正速度
	static constexpr float kConfirmDirThreshold	= 0.1f;	// 目的のdirを即座に現在のdirに反映する閾値
	static constexpr int   kWalkStickSlopeLimit	= 15000;	// 歩き状態とするスティック傾きの上限

	std::shared_ptr<Camera> m_camera;

	std::unordered_map<TimeKind, VECTOR> m_dir;	// 向いている方向(向き補間のためタイムカインドを用意)
	VECTOR m_move_dir;							// 移動方向

	VECTOR m_velocity;
	float  m_move_speed;
	bool   m_is_move;
	bool   m_is_run;
};
