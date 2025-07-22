#pragma once
#include <queue>
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
		if (!m_virtual_cameras.count(virtual_camera->GetObjHandle()))
		{
			m_virtual_cameras[virtual_camera->GetObjHandle()] = virtual_camera;
			
			// 優先順位も同時に格納
			for (const auto& v_camera : m_virtual_cameras)
			{
				m_priority[v_camera.second->GetObjHandle()] = v_camera.second->GetPriority();
			}
			m_priority = algorithm::Sort(m_priority, SortKind::kDescending);
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
	

	#pragma region ブレンド関連処理
	/// @brief バーチャルカメラ間のブレンドを行う
	void BlendVirtualCamera();

	/// @brief ターゲットとなるバーチャルカメラを変更する
	void ChangeTargetVirtualCamera(const int obj_handle);

	/// @brief ブレンド対象となるトランスフォームを設定
	void SetBlendTransform(std::queue<int>& sorted_camera_handles);

	/// @brief ブレンド結果行列トランスフォームを計算
	void CalcBlendResuletTransform();
	#pragma endregion


private:
	static constexpr float kNear		= 10.0f;
	static constexpr float kFar			= 4000.0f;
	static constexpr float kFOV			= 25.0f;
	static constexpr float kBlendTime	= 5.0f;

	std::shared_ptr<MainCamera>									m_main_camera;			// バーチャルカメラを適用させるメインカメラ
	std::unordered_map<int, std::shared_ptr<VirtualCameraBase>>	m_virtual_cameras;		// 登録されているバーチャルカメラ
	std::unordered_map<int, int>								m_priority;				// 優先順位<オブジェクトハンドル, 優先度>

	std::shared_ptr<Transform> m_blend_origin_transform;			// ブレンドの起点とするトランスフォーム
	std::shared_ptr<Transform> m_blend_target_transform;			// ブレンドのターゲットとするトランスフォーム
	std::shared_ptr<Transform> m_blend_origin_result_transform;		// ブレンドの起点に利用するブレンド結果トランスフォーム
	std::shared_ptr<Transform> m_blend_result_transform;			// ブレンド結果トランスフォーム


	int   m_target_virtual_camera_handle;	// ブレンドのターゲットとしているバーチャルカメラのハンドル
	float m_blend_timer;					// ブレンドにかける時間を計測
	float m_blend_coefficient;				// ブレンド係数
	bool  m_is_blending;					// ブレンド中かを判定
	bool  m_is_invert_horizontal;			// 操作時に左右反転を行うかを判定
	bool  m_is_invert_vertical;				// 操作時に上下反転を行うかを判定

	bool test_is_add1;
	bool test_is_add2;
	int handle1;
	int handle2;
	int handle3;

	friend SingletonBase<CameraManager>;
};
