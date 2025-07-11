#pragma once
#include "../Base/singleton_base.hpp"

#include "../VirtualCamera/rot_control_virtual_camera.hpp"
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
	void Update();
	void LateUpdate();

	/// @brief メインカメラを設定する
	void SetMainCamera(const std::shared_ptr<MainCamera> main_camera);


	#pragma region 登録 / 解除
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
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<MainCamera> GetMainCamera() const { return m_main_camera; }

	/// @brief バーチャルカメラを取得
	/// @param obj_handle オブジェクトハンドル
	template<virtual_camera_concepts::VirtualCameraT VirtualCameraT>
	[[nodiscard]] std::shared_ptr<VirtualCameraT> GetVirtualCamera(const int obj_handle) const
	{
		return m_virtual_camera.count(obj_handle) ? std::dynamic_pointer_cast<VirtualCameraT>(m_virtual_camera.at(obj_handle)) : nullptr;
	}

	/// @brief バーチャルカメラを取得
	/// @brief オブジェクト名でカメラを判別する(同じ名前が複数ある場合はオブジェクトハンドルでの取得を推奨)
	/// @param obj_name オブジェクト名
	template<virtual_camera_concepts::VirtualCameraT VirtualCameraT>
	[[nodiscard]] std::shared_ptr<VirtualCameraT> GetVirtualCamera(const std::string& obj_name) const
	{
		for (const auto& camera : m_virtual_camera)
		{
			if (camera.second->GetName() == obj_name)
			{
				return std::dynamic_pointer_cast<VirtualCameraT>(camera.second);
			}
		}
		return nullptr;
	}

	/// @brief バーチャルカメラを取得
	/// @brief バーチャルカメラの種類でカメラを判別する(同じ種類が複数ある場合はオブジェクトハンドルでの取得を推奨)
	/// @param camra_kind バーチャルカメラの種類
	template<virtual_camera_concepts::VirtualCameraT VirtualCameraT>
	[[nodiscard]] std::shared_ptr<VirtualCameraT> GetVirtualCamera(const VirtualCameraKind camra_kind) const
	{
		for (const auto& camera : m_virtual_camera)
		{
			if (camera.second->GetCameraKind() == camra_kind)
			{
				return std::dynamic_pointer_cast<VirtualCameraT>(camera.second);
			}
		}
		return nullptr;
	}
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
//	/// @brief ボーンを追尾する際にボーンの揺れまで追尾する
//	/// @brief ボーンを追尾していない場合は効果なし
//	void TrackBoneWobbly() { m_is_track_height_only = true; }
//	/// @brief ターゲットの原点から対象のボーンまでの高さのみを抽出した位置を追尾
//	/// @brief ボーンを追尾していない場合は効果なし
//	void TrackBoneHeightOnly() { m_is_track_height_only = false; }
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

	/// @brief バーチャルカメラのブレンド結果をメインカメラに適用させる
	void ApplyBlendResultMatrix();

//	void Move();
//	void InitMove();
//
//	void CalcAngle();
//	void CalcPos();
//	void CalcRayPos();
//	void CalcDistance();
//
//	@brief 視点リセット時の角度を計算する
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
//public:
//	static constexpr float  kNormalDistance = 42.0f;
//
//private:
//
//	static constexpr VECTOR kLookCorrectPos = { 19.0f, 5.0f, 0.0f };
//
//	//static constexpr float  kMaxDistanceToTarget	= 42.0f;
//	//static constexpr float  kMinDistanceToTarget	= 42.0f;
//


//
//	static constexpr float  kInitAngleTolerance = 0.01f;		// 視点リセットが完了したと判定させる許容値
//
//	std::shared_ptr<Transform>	m_target_transform;
//	int							m_target_model_handle;
//	std::string					m_target_bone;
//
//	float m_distance_to_target;		// 追跡対象までの距離
//	bool m_is_look_same_dir_target;	// 追跡対象と同じ向きを見ているかを判定(Y成分は考慮しない)
//	bool m_is_track_height_only;	// 高さのみを追尾するかを判定
	
private:
	static constexpr float  kNear	= 1.0f;
	static constexpr float  kFar	= 3000.0f;
	static constexpr float  kFOV	= 45.0f;

	std::shared_ptr<MainCamera>									m_main_camera;
	std::unordered_map<int, std::shared_ptr<VirtualCameraBase>> m_virtual_camera;

	float m_blend_time;					// ブレンドにかける時間
	bool  m_is_blending;				// ブレンド中かを判定
	bool  m_is_invert_horizontal;		// 操作時に左右反転を行うかを判定
	bool  m_is_invert_vertical;			// 操作時に上下反転を行うかを判定

	friend SingletonBase<CameraManager>;
};
