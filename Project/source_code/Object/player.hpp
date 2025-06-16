#pragma once
#include "../Base/chara_base.hpp"
#include "../Data/Kind/player_kind.hpp"

#include "assault_rifle.hpp"
#include "camera.hpp"

class Player final : public CharaBase
{
public:
	Player(std::shared_ptr<Camera> camera);
	~Player() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void OnCollide(const PhysicalObjBase& check_hit_obj) override;
	void OnGravity() override;

	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler()  const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<Animator>	GetAnimator() const { return m_animator; }

	#pragma region コマンド
	void Run();
	void Squat();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void ReadyGun();
	void Shot();
	void TurnAround();
	#pragma endregion

private:
	void Move();
	void InitMove();

	/// @brief 水平方向の速度ベクトルを計算
	void CalcHorizontalVelocity();
	/// @brief 垂直方向の速度ベクトルを計算
	void CalcVerticalVelocity();
	/// @brief 移動速度を計算
	void CalcMoveSpeed(const float input_slope);

	/// @brief 移動方向を計算
	void CalcMoveDir(const VECTOR& velocity);
	/// @brief 向きを計算
	void CalcLookDir();
	/// @brief 向きを補正する
	void CorrectLookDir();

	// パッド入力での速度ベクトルを取得
	[[nodiscard]] VECTOR GetVelocityFromPad(VECTOR& velocity);

	/// @brief しゃがみ処理によりカプセルを縮める
	void ShrinkCapsule();

	/// @brief ボーンの位置を修正する
	void CorrectBonePos();

	/// @brief トランスフォーム情報を更新する
	void UpdateTransform();

	/// @brief 速度ベクトルをコライダーおよびトリガーに適用する;
	void ApplyVelocityToCollider();

	void LoadAnim() override;
	void ChangeAnimState() override;

	/// @brief カメラのforwardをもとにした進行方向ベクトルを取得する
	/// @return カメラのforwardのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveForward();

	[[nodiscard]] bool IsApplyTransform();

private:
	enum class MoveDir
	{
		kForward,
		kBackward,
		kLeft,
		kRight,
	};

private:
	static constexpr float kSquatWalkSpeed						= 1.0f;
	static constexpr float kSlowWalkSpeed						= 1.5f;
	static constexpr float kWalkSpeed							= 2.0f;
	static constexpr float kRunSpeed							= 7.0f;
	static constexpr float kAcceleration						= 2.0f;		// 加速度(減速度も共通)

	static constexpr float kMoveDirCorrectionSpeed				= 0.065f;	// 移動方向の補正速度
	static constexpr float kLookDirCorrectionAngle				= 0.1f;		// 見る方向を回転させる角度
	static constexpr float kLookDirCorrectionAngleForADS		= 0.26f;	// スコープを覗く際の、見る方向を回転させる角度
	static constexpr float kConfirmMoveDirThreshold				= 0.08f;	// 目的のdirに到達したと判定する閾値
	static constexpr float kConfirmLookDirThresholdAngle		= 10.0f;	// 目的のdirに到達したと判定する閾値
	static constexpr float kConfirmLookDirThresholdAngleForADS	= 20.0f;	// スコープを覗く際の、目的のdirに到達したと判定する閾値

	static constexpr float kIdelAnimPlayThreshold				= 0.35f;	// アイドルアニメーションを再生すると判定する閾値

	static constexpr int   kWalkStickSlopeLimit					= 15000;	// 歩き状態とするスティック傾きの上限
	static constexpr int   kTurnAroundStickAngle				= 30.0f;	// 振り向きを行うスティックの入力角度

	static constexpr float kCapsuleRadius						= 34.0f;
	static constexpr float kCapsuleLengthForStand				= 178.0f;
	static constexpr float kCapsuleLengthForSquat				= 130.0f;
	static constexpr float kCapsuleLengthShrinkSpeed			= 130.0f;
	static constexpr float kLandingTriggerRadius				= 23.0f;
	
	static constexpr float kAimDownSightsSpeed					= 800.0f;	// スコープをのぞき込む速度

	std::shared_ptr<Camera> m_camera;

	std::unordered_map<TimeKind, VECTOR> m_move_dir;			// 移動方向(TODO : 長さが1未満である時がある場合があるため命名を変更すべき)
	std::unordered_map<TimeKind, VECTOR> m_look_dir;			// 向いている方向
	
	std::unordered_map<TimeKind, PlayerAnimKind> m_anim_kind;	// アニメーションの状態を判定

	float m_move_speed;
	float m_non_move_time;										// 入力をしていない時間

	std::shared_ptr<Capsule> m_capsule_collider;
	float m_capsule_length;										// 押し戻し用のカプセルの長さ

	bool m_is_move;
	bool m_is_run;
	bool m_is_squat;
	bool m_is_ready_gun;										// 銃を構える
	bool m_is_turn_around;										// Y軸180°回転させるかを判定
	bool m_is_correct_look_dir;									// 見る方向を補正するかを判定

	int  m_turn_around_count;									// 連続で振り向くことを触れぐための振り向きカウント

	std::array<bool, 4> m_is_input_move;
};
