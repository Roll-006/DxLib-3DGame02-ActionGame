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
	void Run();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void ReadyGun();
	#pragma endregion

private:
	void Move();

	/// @brief 水平方向の速度ベクトルを計算
	void CalcHorizontalVelocity();
	/// @brief 垂直方向の速度ベクトルを計算
	void CalcVerticalVelocity();
	/// @brief 移動速度を計算
	void CalcMoveSpeed(const float input_slope);
	/// @brief 向きを計算
	void CalcDir(const VECTOR& velocity);

	// パッド入力での速度ベクトルを取得
	[[nodiscard]] VECTOR GetVelocityFromPad(VECTOR& velocity);

	/// @brief 加速処理
	void Acceleration(const float destination_speed);
	/// @brief 減速処理
	void Deceleration(const float destination_speed);

	void LoadAnim() override;
	void ChangeAnimState() override;

private:
	static constexpr float kSlowWalkSpeed			= 2.0f;
	static constexpr float kWalkSpeed				= 4.0f;
	static constexpr float kRunSpeed				= 10.0f;
	static constexpr float kAcceleration			= 4.0f;		// 加速度(減速度も共通)

	static constexpr float kDirCorrectionSpeed		= 0.065f;	// dirの補正速度
	static constexpr float kConfirmDirThreshold		= 0.08f;	// 目的のdirを即座に現在のdirに反映する閾値

	static constexpr int   kWalkStickSlopeLimit		= 15000;	// 歩き状態とするスティック傾きの上限

	static constexpr float kColliderCapsuleRadius	= 34.0f;
	static constexpr float kColliderCapsuleLength	= 178.0f;
	static constexpr float kLandingTriggerRadius	= 23.0f;

	std::shared_ptr<Camera> m_camera;

	std::unordered_map<TimeKind, VECTOR> m_move_dir;			// 移動方向(TODO : 長さが1未満である時がある場合があるため命名を変更すべき)
	std::unordered_map<TimeKind, VECTOR> m_dir;					// 向いている方向

	std::unordered_map<TimeKind, PlayerAnimKind> m_anim_kind;	// アニメーションの状態を判定

	float m_move_speed;
	bool  m_is_move;
	bool  m_is_run;
	bool  m_is_ready_gun;	// 銃を構える
};
