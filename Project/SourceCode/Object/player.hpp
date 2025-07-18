#pragma once
#include "../Base/chara_base.hpp"

#include "../Part/player_animator.hpp"

#include "assault_rifle.hpp"
#include "../Manager/camera_manager.hpp"

class PlayerStateController;

class Player final : public CharaBase
{
public:
	enum class MoveDir
	{
		kForward,
		kBackward,
		kLeft,
		kRight,
	};

public:
	Player();
	~Player() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;


	#pragma region State
	void CalcMoveDirFirstFrame();

	void Move();

	void CalcStopSpeed();
	void CalcCrouchSpeed();
	void CalcRunSpeed();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<PlayerStateController> GetStateController() const { return m_state; }
	#pragma endregion

private:
	void CalcMoveSpeed(const float input_slope);

	void CalcVelocity();
	void CalcMoveDir(const VECTOR& velocity);

	void MoveOld();

	/// @brief 移動方向を計算
	/// @brief 向きを計算
	void CalcLookDir();
	/// @brief 向きを補正する
	void CorrectLookDir();

	// パッド入力での速度ベクトルを取得
	[[nodiscard]] VECTOR GetVelocityFromPad();

	/// @brief カメラのforwardをもとにした進行方向ベクトルを取得する
	/// @return カメラのforwardのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveForward();

private:
	static constexpr float kModelScale							= 0.25f;

	static constexpr float kCrouchWalkSpeed						= 0.15f;
	static constexpr float kSlowWalkSpeed						= 0.2f;
	static constexpr float kWalkSpeed							= 0.5f;
	static constexpr float kRunSpeed							= 2.0f;
	static constexpr float kAcceleration						= 1.0f;		// 加速度(減速度も共通)

	static constexpr float kMoveDirCorrectionSpeed				= 0.065f;	// 移動方向の補正速度
	static constexpr float kLookDirCorrectionAngle				= 0.1f;		// 見る方向を回転させる角度
	static constexpr float kLookDirCorrectionAngleForADS		= 0.3f;		// スコープを覗く際の、見る方向を回転させる角度
	static constexpr float kConfirmMoveDirThreshold				= 0.08f;	// 目的のdirに到達したと判定する閾値
	static constexpr float kConfirmLookDirThresholdAngle		= 10.0f;	// 目的のdirに到達したと判定する閾値
	static constexpr float kConfirmLookDirThresholdAngleForADS	= 20.0f;	// スコープを覗く際の、目的のdirに到達したと判定する閾値

	static constexpr float kIdelAnimPlayThreshold				= 0.35f;	// アイドルアニメーションを再生すると判定する閾値

	static constexpr int   kWalkStickSlopeLimit					= 15000;	// 歩き状態とするスティック傾きの上限
	static constexpr float kTurnAroundStickAngle				= 30.0f;	// 振り向きを行うスティックの入力角度

	static constexpr float kCapsuleRadius						= 8.0f;
	static constexpr float kLandingTriggerRadius				= 6.0f;
	
	static constexpr float kADSSpeed							= 70.0f;	// スコープをのぞき込む速度

private:
	std::unordered_map<TimeKind, VECTOR> m_move_dir;			// 移動方向(TODO : 長さが1未満である時がある場合があるため命名を変更すべき)
	std::unordered_map<TimeKind, VECTOR> m_look_dir;			// 向いている方向
	
	PlayerAnimKind m_anim_kind;									// アニメーションの状態を判定
	std::shared_ptr<PlayerStateController> m_state;

	std::shared_ptr<BonePosCorrector> m_bone_pos_corrector;		// ボーン位置修正

	float m_move_speed;
	float m_non_move_time;										// 入力をしていない時間

	bool m_is_move;
	bool m_is_run;
	bool m_is_crouch;
	bool m_is_turn_around;										// Y軸180°回転させるかを判定
	bool m_is_turn_run;											// ダッシュ中に振り向くかを判定
	bool m_is_correct_look_dir;									// 見る方向を補正するかを判定

	int  m_turn_around_count;									// 連続で振り向くことを触れぐための振り向きカウント

	std::array<bool, 4> m_is_input_move;
};
