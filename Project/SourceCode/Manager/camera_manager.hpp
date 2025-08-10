#pragma once
#include "../Base/singleton_base.hpp"

#include "../Object/main_camera.hpp"
#include "../VirtualCamera/rot_control_virtual_camera.hpp"
#include "../VirtualCamera/rocket_launcher_virtual_camera_controller.hpp"

#include "../Input/input_checker.hpp"
#include "../GameTime/game_time_manager.hpp"

class Player;

namespace virtual_camera_concepts
{
	/// @brief バーチャルカメラ型である
	template<typename T>
	concept VirtualCameraT			= std::is_base_of_v<VirtualCameraBase, T>;

	/// @brief バーチャルカメラコントローラー型である
	template<typename T>
	concept VirtualCameraController = std::is_base_of_v<IVirtualCameraController, T>;
}

class CameraManager final : public SingletonBase<CameraManager>
{
public:
	void Update();
	void LateUpdate();

	/// @brief メインカメラを設定する
	void SetMainCamera(const std::shared_ptr<MainCamera> main_camera);

	/// @brief ブレンドにかける時間を設定する
	void SetBlendTime(const float blend_time);

	/// @brief 優先順位をソートする
	template<virtual_camera_concepts::VirtualCameraT VirtualCameraT>
	void SortPriority(const std::shared_ptr<VirtualCameraT> virtual_camera)
	{
		if (m_virtual_cameras.count(virtual_camera->GetObjHandle()))
		{
			for (const auto& v_camera : m_virtual_cameras)
			{
				m_priority[v_camera.second->GetObjHandle()] = v_camera.second->GetPriority();
			}
			m_priority = algorithm::Sort(m_priority, SortKind::kDescending);
		}
	}


	#pragma region 登録 / 解除
	/// @brief バーチャルカメラを登録する
	template<virtual_camera_concepts::VirtualCameraT VirtualCameraT>
	void AddVirtualCamera(const std::shared_ptr<VirtualCameraT> virtual_camera)
	{
		if (!m_virtual_cameras.count(virtual_camera->GetObjHandle()))
		{
			m_virtual_cameras[virtual_camera->GetObjHandle()] = virtual_camera;

			SortPriority(virtual_camera);
		}
	}
	
	/// @brief バーチャルカメラの登録を解除する
	void RemoveVirtualCamera(const int obj_handle);

	/// @brief バーチャルカメラコントローラーを登録する
	template<virtual_camera_concepts::VirtualCameraController ControllerT>
	void AddVirtualCameraController(std::shared_ptr<ControllerT> virtual_camera_controller)
	{
		if (std::find(m_virtual_camera_controllers.begin(), m_virtual_camera_controllers.end(), virtual_camera_controller) == m_virtual_camera_controllers.end())
		{
			m_virtual_camera_controllers.emplace_back(virtual_camera_controller);
		}
	}

	/// @brief バーチャルカメラコントローラーの登録を解除する
	template<virtual_camera_concepts::VirtualCameraController ControllerT>
	void RemoveVirtualCameraController(std::shared_ptr<ControllerT> virtual_camera_controller)
	{
		if (std::find(m_virtual_camera_controllers.begin(), m_virtual_camera_controllers.end(), virtual_camera_controller) != m_virtual_camera_controllers.end())
		{
			erase(m_virtual_camera_controllers, virtual_camera_controller);
		}
	}
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
	void SetBlendTransform();

	/// @brief ブレンド結果行列トランスフォームを計算
	void CalcBlendResuletTransform();
	#pragma endregion


private:
	static constexpr float kNear		= 10.0f;
	static constexpr float kFar			= 4000.0f;
	static constexpr float kFOV			= 25.0f;

	std::shared_ptr<MainCamera>									m_main_camera;			// バーチャルカメラを適用させるメインカメラ
	std::unordered_map<int, std::shared_ptr<VirtualCameraBase>>	m_virtual_cameras;		// 登録されているバーチャルカメラ
	std::unordered_map<int, int>								m_priority;				// 優先順位<オブジェクトハンドル, 優先度>

	std::shared_ptr<Transform>			m_blend_origin_transform;						// ブレンドの起点とするトランスフォーム
	std::shared_ptr<Transform>			m_blend_target_transform;						// ブレンドのターゲットとするトランスフォーム
	std::shared_ptr<Transform>			m_blend_origin_result_transform;				// ブレンドの起点に利用するブレンド結果トランスフォーム
	std::shared_ptr<Transform>			m_blend_result_transform;						// ブレンド結果トランスフォーム

	std::unordered_map<TimeKind, int>	m_origin_virtual_camera_handle;					// ブレンドの起点とするバーチャルカメラのハンドル
	std::unordered_map<TimeKind, int>	m_target_virtual_camera_handle;					// ブレンドのターゲットとするバーチャルカメラのハンドル

	float m_blend_time;
	float m_blend_timer;																// ブレンドにかける時間を計測
	float m_blend_coefficient;															// ブレンド係数
	bool  m_is_blending;																// ブレンド中かを判定
	bool  m_is_invert_horizontal;														// 操作時に左右反転を行うかを判定
	bool  m_is_invert_vertical;															// 操作時に上下反転を行うかを判定

	std::vector<std::shared_ptr<IVirtualCameraController>> m_virtual_camera_controllers;




	bool test_is_add1;
	bool test_is_add2;
	int handle1;
	int handle2;
	int handle3;
	std::shared_ptr<Transform> transform1;
	std::shared_ptr<Transform> transform2;



	friend class SingletonBase<CameraManager>;
};
