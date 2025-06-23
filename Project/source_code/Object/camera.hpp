#pragma once
#include "../Manager/obj_manager.hpp"
#include "../Manager/input_checker.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

#include "../Part/modeler.hpp"

class Camera final : public PhysicalObjBase
{
public:
	Camera();
	~Camera() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairData& hit_collider_pair) override;
	

	#pragma region アタッチ・デタッチ
	/// @brief 追跡対象をアタッチする(トランスフォーム情報で追跡)
	void AttachTarget(const std::shared_ptr<ObjBase> obj);
	/// @brief 追跡対象をアタッチする(トランスフォーム情報で追跡)
	void AttachTarget(const std::string& obj_name);
	/// @brief 追跡対象をアタッチする(トランスフォーム＋ボーン情報で追跡)
	/// @param bone_path ボーンのパス
	void AttachTarget(const std::shared_ptr<ObjBase> obj, const std::shared_ptr<Modeler> modeler, const std::string& bone_path, const bool is_track_height_only);
	/// @brief 追跡対象をアタッチする(トランスフォーム＋ボーン情報で追跡)
	/// @param bone_path ボーンのパス
	void AttachTarget(const std::string& obj_name, const std::shared_ptr<Modeler> modeler, const std::string& bone_path, const bool is_track_height_only);
	/// @brief 追跡対象をデタッチする
	void DetachTarget();
	#pragma endregion


	#pragma region ターゲットとの距離
	/// @brief ターゲットとの距離を設定する
	void SetDistanceToTarget(const float distance) { m_distance_to_target = distance; }

	/// @brief ターゲットに接近する
	void Approach(const float min_distance, const float move_speed);
	/// @brief ターゲットから離れる
	void Depart(const float max_distance, const float move_speed);
	#pragma endregion


	#pragma region コマンド
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void InitAngle();
	#pragma endregion


	/// @brief ボーンを追尾する際にボーンの揺れまで追尾する
	/// @brief ボーンを追尾していない場合は効果なし
	void TrackBoneWobbly() { m_is_track_height_only = true; }
	/// @brief ターゲットの原点から対象のボーンまでの高さのみを抽出した位置を追尾
	/// @brief ボーンを追尾していない場合は効果なし
	void TrackBoneHeightOnly() { m_is_track_height_only = false; }

	/// @brief ヨー角(Y軸回転)の視点をリセット
	void InitYaw();

	/// @brief 視点リセット中かどうかを判定
	[[nodiscard]] bool IsInitAngle() const { return m_is_init_angle; }

	/// @brief 追跡対象と同じ方向を見ているかを判定
	/// @bried Y成分は考慮しない
	[[nodiscard]] bool IsLookSameDirTarget() const { return m_is_look_same_dir_target; }

private:
	void Move();
	void InitMove();

	void CalcAngle();
	//void CalcDistance();

	/// @brief 視点リセット時の角度を計算する
	void CalcInitAngle();

	/// @brief 見る座標を取得
	[[nodiscard]] VECTOR GetLookPos();

	/// @brief 見る方向を設定
	void SetLookDir();

	/// @brief 追跡対象と同じ方向を見ているかを判定
	void JudgeLookSameDirTarget();

	/// @brief 操作時の反転処理を適用する
	void ApplyInvert();

	void CalcDirFromPad();
	void CalcDirFromMouse();

private:
	enum class InputDir
	{
		kUp,
		kDown,
		kLeft,
		kRight,
	};

public:
	static constexpr float  kNormalDistance			= 42.0f;

private:
	static constexpr float  kNear					= 1.0f;
	static constexpr float  kFar					= 3000.0f;
	static constexpr float  kFOV					= 60.0f;

	static constexpr VECTOR kLookCorrectPos			= VECTOR(19.0f, 5.0f, 0.0f);

	//static constexpr float  kMaxDistanceToTarget	= 42.0f;
	//static constexpr float  kMinDistanceToTarget	= 42.0f;

	static constexpr float  kMaxVerticalAngle		= 60.0f;
	static constexpr float  kMinVerticalAngle		= -60.0f;
	static constexpr float  kInitAngleSpeed			= 10.0f;
	static constexpr float  kInitYawSpeed			= 60.0f;
	static constexpr float  kInitAngleEndThreshold	= 0.01f;
	static constexpr float  kInitYawEndThreshold	= 0.02f;

	static constexpr float  kMoveSpeedWithButton	= 2.0f;
	static constexpr float  kMoveSpeedWithStick		= 2.5f;
	static constexpr float  kMoveSpeedWithMouse		= 0.1f;

	static constexpr float  kInitAngleTolerance		= 0.01f;		// 視点リセットが完了したと判定させる許容値

	std::shared_ptr<Transform> m_target_transform;
	std::shared_ptr<Modeler>   m_target_modeler;
	std::string				   m_target_bone;

	float m_distance_to_target;		// 追跡対象までの距離

	float m_init_angle_speed;		// 視点リセットの速度
	float m_init_end_threshold;		// 視点リセットの終了判定を行う閾値

	bool m_is_invert_horizontal;	// 操作時に左右反転を行うかを判定
	bool m_is_invert_vertical;		// 操作時に上下反転を行うかを判定
	bool m_is_init_angle;			// 視点リセットを行うかを判定
	bool m_is_look_same_dir_target;	// 追跡対象と同じ向きを見ているかを判定(Y成分は考慮しない)
	bool m_is_track_height_only;	// 高さのみを追尾するかを判定

	VECTOR m_dir;
	VECTOR m_velocity;

	std::unordered_map<TimeKind, VECTOR> m_angle;
	std::array<bool, 4> m_is_input;
};
