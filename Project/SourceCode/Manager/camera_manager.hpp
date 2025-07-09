#pragma once
#include "../Base/singleton_base.hpp"

#include "../Base/virtual_camera_base.hpp"
#include "../Object/main_camera.hpp"

namespace virtual_camera_concepts
{
	/// @brief バーチャルカメラ型である
	template<typename T>
	concept VirtualCameraT = std::is_base_of_v<VirtualCameraBase, T>;
}

class CameraManager final : public SingletonBase<CameraManager>
{
public:
	enum class BlendKind
	{
		kTime,		// 時間でブレンド
		kSpeed,		// 速度でブレンド
	};

public:
	void Update();
	void LateUpdate();

	/// @brief メインカメラを設定する
	void SetMainCamera(const std::shared_ptr<MainCamera> main_camera);

	/// @brief バーチャルカメラを登録する
	template<virtual_camera_concepts::VirtualCameraT VirtualCameraT>
	void AddVirtualCamera(const std::shared_ptr<VirtualCameraT> virtual_camera)
	{
		if (!m_virtual_camera.count(virtual_camera->GetObjHandle()))
		{
			m_virtual_camera[virtual_camera->GetObjHandle()] = virtual_camera;
		}
	}
	
	/// @brief バーチャルカメラの登録を解除する
	void RemoveVirtualCamera(const int obj_handle);
	

	#pragma region Getter
	[[nodiscard]] std::shared_ptr<MainCamera> GetMainCamera() const { return m_main_camera; }

	/// @brief バーチャルカメラを取得
	/// @param obj_handle オブジェクトハンドル
	[[nodiscard]] std::shared_ptr<VirtualCameraBase> GetVirtualCamera(const int obj_handle) const;

	/// @brief バーチャルカメラを取得
	/// @brief オブジェクト名カメラを判別する(同じ名前が複数ある場合はオブジェクトハンドルでの取得を推奨)
	/// @param obj_name オブジェクト名
	[[nodiscard]] std::shared_ptr<VirtualCameraBase> GetVirtualCamera(const std::string& obj_name) const;
	#pragma endregion


	/// @brief バーチャルカメラをブレンド中であるかを判定
	[[nodiscard]] bool IsBlending() const { return m_is_blending; }
	
// @brief 追跡対象をアタッチする(トランスフォーム情報で追跡)
//	void AttachTarget(const std::string& obj_name);
//	/// @brief 追跡対象をアタッチする(トランスフォーム＋ボーン情報で追跡)
//	/// @param bone_path ボーンのパス
//	void AttachTarget(const std::shared_ptr<ObjBase> obj, const int model_handle, const std::string& bone_path, const bool is_track_height_only);
//	/// @brief 追跡対象をアタッチする(トランスフォーム＋ボーン情報で追跡)
//	/// @param bone_path ボーンのパス
//	void AttachTarget(const std::string& obj_name, const int model_handle, const std::string& bone_path, const bool is_track_height_only);
//	/// @brief 追跡対象をデタッチする
//	void DetachTarget();
//#pragma endregion
//
//
//#pragma region ターゲットとの距離
//	/// @brief ターゲットとの距離を設定する
//	void SetDistanceToTarget(const float distance) { m_distance_to_target = distance; }
//
//	/// @brief ターゲットに接近する
//	void Approach(const float min_distance, const float move_speed);
//	/// @brief ターゲットから離れる
//	void Depart(const float max_distance, const float move_speed);
//#pragma endregion
//
//
//
//
//
//	/// @brief ボーンを追尾する際にボーンの揺れまで追尾する
//	/// @brief ボーンを追尾していない場合は効果なし
//	void TrackBoneWobbly() { m_is_track_height_only = true; }
//	/// @brief ターゲットの原点から対象のボーンまでの高さのみを抽出した位置を追尾
//	/// @brief ボーンを追尾していない場合は効果なし
//	void TrackBoneHeightOnly() { m_is_track_height_only = false; }
//
//	/// @brief ヨー角(Y軸回転)の視点をリセット
//	void InitYaw();
//
//	/// @brief 視点リセット中かどうかを判定
//	[[nodiscard]] bool IsInitAngle() const { return m_is_init_angle; }
//
//	/// @brief 追跡対象と同じ方向を見ているかを判定
//	/// @bried Y成分は考慮しない
//	[[nodiscard]] bool IsLookSameDirTarget() const { return m_is_look_same_dir_target; }

private:
	CameraManager();
	~CameraManager() override;
	
	/// @brief バーチャルカメラ間のブレンドを行う
	void BlendVirtualCamera();

	/// @brief バーチャルカメラのブレンド結果をメインカメラに受け渡す
	[[nodiscard]] MATRIX GiveBlendResultMatrix();

//	void Move();
//	void InitMove();
//
//	void CalcAngle();
//	void CalcPos();
//	void CalcRayPos();
//	//void CalcDistance();
//
//	/// @brief 視点リセット時の角度を計算する
//	void CalcInitAngle();
//
//	/// @brief 見る座標を取得
//	[[nodiscard]] VECTOR GetLookPos();
//
//	/// @brief 追跡対象と同じ方向を見ているかを判定
//	void JudgeLookSameDirTarget();
//
//	/// @brief 操作時の反転処理を適用する
//	void ApplyInvert();
//
//	void CalcDirFromPad();
//	void CalcDirFromMouse();
//
//private:
//	enum class InputDir
//	{
//		kUp,
//		kDown,
//		kLeft,
//		kRight,
//	};
//
//public:
//	static constexpr float  kNormalDistance = 42.0f;
//
//private:
//	static constexpr float  kNear = 1.0f;
//	static constexpr float  kFar = 5000.0f;
//	static constexpr float  kFOV = 45.0f;
//
//	static constexpr VECTOR kLookCorrectPos = { 19.0f, 5.0f, 0.0f };
//
//	//static constexpr float  kMaxDistanceToTarget	= 42.0f;
//	//static constexpr float  kMinDistanceToTarget	= 42.0f;
//
//	static constexpr float  kMaxVerticalAngle = 60.0f;
//	static constexpr float  kMinVerticalAngle = -60.0f;
//	static constexpr float  kInitAngleSpeed = 10.0f;
//	static constexpr float  kInitYawSpeed = 60.0f;
//	static constexpr float  kInitAngleEndThreshold = 0.01f;
//	static constexpr float  kInitYawEndThreshold = 0.3f;
//
//	static constexpr float  kMoveSpeedWithButton = 2.0f;
//	static constexpr float  kMoveSpeedWithStick = 2.5f;
//	static constexpr float  kMoveSpeedWithMouse = 0.1f;
//
//	static constexpr float  kInitAngleTolerance = 0.01f;		// 視点リセットが完了したと判定させる許容値
//
//	std::shared_ptr<Transform>	m_target_transform;
//	int							m_target_model_handle;
//	std::string					m_target_bone;
//
//	float m_distance_to_target;		// 追跡対象までの距離
//
//	float m_init_angle_speed;		// 視点リセットの速度
//	float m_init_end_threshold;		// 視点リセットの終了判定を行う閾値
//
//	bool m_is_invert_horizontal;	// 操作時に左右反転を行うかを判定
//	bool m_is_invert_vertical;		// 操作時に上下反転を行うかを判定
//	bool m_is_init_angle;			// 視点リセットを行うかを判定
//	bool m_is_look_same_dir_target;	// 追跡対象と同じ向きを見ているかを判定(Y成分は考慮しない)
//	bool m_is_track_height_only;	// 高さのみを追尾するかを判定
//
//	VECTOR m_dir;
//	VECTOR m_velocity;
//
//	std::unordered_map<TimeKind, VECTOR> m_angle;
//	std::array<bool, 4> m_is_input;
	
private:
	std::shared_ptr<MainCamera>									m_main_camera;
	std::unordered_map<int, std::shared_ptr<VirtualCameraBase>> m_virtual_camera;

	float m_blend_time;
	float m_blend_speed;
	bool  m_is_blending;

	friend SingletonBase<CameraManager>;
};
