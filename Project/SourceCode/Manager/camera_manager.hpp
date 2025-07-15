#pragma once
#include "../Base/singleton_base.hpp"

#include "../VirtualCamera/rot_control_virtual_camera.hpp"
#include "../VirtualCamera/scope_virtual_camera.hpp"
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
	[[nodiscard]] std::shared_ptr<VirtualCameraBase> GetVirtualCamera(const int obj_handle) const;
	/// @brief バーチャルカメラを取得
	/// @brief オブジェクト名でカメラを判別する(同じ名前が複数ある場合はオブジェクトハンドルでの取得を推奨)
	/// @param obj_name オブジェクト名
	[[nodiscard]] std::shared_ptr<VirtualCameraBase> GetVirtualCamera(const std::string& obj_name) const;
	/// @brief バーチャルカメラを取得
	/// @brief バーチャルカメラの種類でカメラを判別する(同じ種類が複数ある場合はオブジェクトハンドルでの取得を推奨)
	/// @param camra_kind バーチャルカメラの種類
	[[nodiscard]] std::shared_ptr<VirtualCameraBase> GetVirtualCamera(const VirtualCameraKind camra_kind) const;
	#pragma endregion


	/// @brief バーチャルカメラをブレンド中であるかを判定
	[[nodiscard]] bool IsBlending() const { return m_is_blending; }


private:
	CameraManager();
	~CameraManager() override;
	
	/// @brief バーチャルカメラ間のブレンドを行う
	void BlendVirtualCamera();

	/// @brief バーチャルカメラのブレンド結果をメインカメラに適用させる
	void ApplyBlendResultMatrix();
	
private:
	static constexpr float  kNear		= 1.0f;
	static constexpr float  kFar		= 3000.0f;
	static constexpr float  kFOV		= 45.0f;

	std::shared_ptr<MainCamera>									m_main_camera;		// バーチャルカメラを適用させるメインカメラ
	std::unordered_map<int, std::shared_ptr<VirtualCameraBase>> m_virtual_camera;	// 登録されているバーチャルカメラ
	std::unordered_map<TimeKind, Transform>						m_blend_transform;	// ブレンド対象のトランスフォーム

	float m_blend_time;					// ブレンドにかける時間
	bool  m_is_blending;				// ブレンド中かを判定
	bool  m_is_invert_horizontal;		// 操作時に左右反転を行うかを判定
	bool  m_is_invert_vertical;			// 操作時に上下反転を行うかを判定

	friend SingletonBase<CameraManager>;
};
