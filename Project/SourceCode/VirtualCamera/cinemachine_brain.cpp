#include "cinemachine_brain.hpp"
#include "../Object/player.hpp"

CinemachineBrain::CinemachineBrain() :
	m_main_camera					(nullptr),
	m_blend_origin_transform		(nullptr),
	m_blend_target_transform		(nullptr),
	m_blend_origin_result_transform	(nullptr),
	m_blend_result_transform		(nullptr),
	m_blend_time					(1.0f),
	m_blend_timer					(0.0f),
	m_blend_coefficient				(0.0f),
	m_is_blending					(false),
	m_is_invert_horizontal			(false),
	m_is_invert_vertical			(false)
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegToRad);
}

CinemachineBrain::~CinemachineBrain()
{

}

void CinemachineBrain::Update()
{
	// 非アクティブ化処理の遅延防止のため一度先行してトランスフォームを取得
	SetBlendTransform();

	for (const auto& camera_controller : m_virtual_camera_controllers)
	{
		camera_controller->Update();
	}

	for (const auto& camera : m_virtual_cameras)
	{
		camera.second->Update();
	}
}

void CinemachineBrain::LateUpdate()
{
	for (const auto& camera_controller : m_virtual_camera_controllers)
	{
		camera_controller->LateUpdate();
	}

	for (const auto& camera : m_virtual_cameras)
	{
		camera.second->LateUpdate();
	}

	BlendVirtualCamera();

	m_main_camera->LateUpdate();
}

void CinemachineBrain::Draw() const
{
	for (const auto& camera : m_virtual_cameras)
	{
		camera.second->Draw();
	}

	m_main_camera->Draw();
}

void CinemachineBrain::RemoveVirtualCamera(const int obj_handle)
{
	m_virtual_cameras.erase(obj_handle);

	const auto remove = std::remove_if(m_priority.begin(), m_priority.end(), [=](const std::pair<int, int>& pair)
	{
		return pair.first == obj_handle;
	});
	m_priority.erase(remove, m_priority.end());
}


#pragma region Setter
void CinemachineBrain::SetMainCamera(const std::shared_ptr<MainCamera> main_camera)
{
	if (!m_main_camera)
	{
		m_main_camera = main_camera;
	}
}

void CinemachineBrain::SetBlendTime(const float blend_time)
{
	float scale = 0.0f;
	if (blend_time != 0.0f)
	{
		scale = blend_time / m_blend_time;
	}

	m_blend_time = blend_time;
	m_blend_timer *= scale;
}
#pragma endregion


#pragma region Getter
std::shared_ptr<VirtualCameraBase> CinemachineBrain::GetVirtualCamera(const int obj_handle) const
{
	return m_virtual_cameras.count(obj_handle) ? m_virtual_cameras.at(obj_handle) : nullptr;
}

std::shared_ptr<VirtualCameraBase> CinemachineBrain::GetVirtualCamera(const std::string& obj_name) const
{
	for (const auto& camera : m_virtual_cameras)
	{
		if (camera.second->GetName() == obj_name)
		{
			return camera.second;
		}
	}
	return nullptr;
}

std::shared_ptr<IVirtualCameraController> CinemachineBrain::GetVirtualCameraController(const int controller_handle) const
{
	for (const auto& controller : m_virtual_camera_controllers)
	{
		if (controller->GetControllerHandle() == controller_handle)
		{
			return controller;
		}
	}
	return nullptr;
}

std::shared_ptr<IVirtualCameraController> CinemachineBrain::GetVirtualCameraController(const VirtualCameraControllerKind controller_kind) const
{
	for (const auto& controller : m_virtual_camera_controllers)
	{
		if (controller->GetVirtualCameraControllerKind() == controller_kind)
		{
			return controller;
		}
	}
	return nullptr;
}
#pragma endregion


#pragma region ブレンド関連処理
void CinemachineBrain::DeactivateVirtualCamera(const std::shared_ptr<VirtualCameraBase> origin_camera, const std::shared_ptr<VirtualCameraBase> target_camera)
{
	switch (target_camera->GetBlendActivationPolicyKind())
	{
	case BlendActivationPolicyKind::kDeactivateOriginCamera:
		if (origin_camera) { origin_camera->Deactivate(); }
		break;

	case BlendActivationPolicyKind::kDeactivateAllCamera:
		for (const auto& camera : m_virtual_cameras)
		{
			if (target_camera != camera.second)
			{
				camera.second->Deactivate();
			}
		}
		break;

	default:
		break;
	}
}

void CinemachineBrain::BlendVirtualCamera()
{
	// ブレンドの起点とターゲットを設定
	SetBlendTransform();

	// ブレンド結果を計算
	CalcBlendResuletTransform();

	// メインカメラへ適用
	m_main_camera->GetTransform()->SetMatrix(CoordinateKind::kWorld, m_blend_result_transform->GetMatrix(CoordinateKind::kWorld));
}

void CinemachineBrain::ChangeTargetVirtualCamera(const int obj_handle)
{
	m_target_virtual_camera_handle[TimeKind::kPrev]    = m_target_virtual_camera_handle[TimeKind::kCurrent];
	m_target_virtual_camera_handle[TimeKind::kCurrent] = obj_handle;

	if (m_target_virtual_camera_handle[TimeKind::kPrev] != m_target_virtual_camera_handle[TimeKind::kCurrent])
	{
		// ブレンド中に最優先カメラが切り替わった場合、それまでのブレンド結果をブレンドの起点とする
		if (m_blend_timer != m_blend_time)
		{
			m_blend_origin_result_transform = m_blend_result_transform;
		}

		m_blend_timer = 0.0f;
		m_is_blending = true;
	}
}

void CinemachineBrain::SetBlendTransform()
{
	// FIXME : ブレンドが開始した一瞬、別の地点が描画される現象発生中。[追記]不具合が消えた可能性あり。消えた理由はわからない。要検証
	// FIXME : originA➡targetBのブレンド中に、originB➡targetAに切り替わった場合、到達までの時間が早くなる不具合発生中
	
	bool is_seted_target_transform = false;
	bool is_seted_origin_transform = false;
	std::shared_ptr<VirtualCameraBase> origin_camera = nullptr;
	std::shared_ptr<VirtualCameraBase> target_camera = nullptr;

	for (const auto& pr : m_priority)
	{
		const auto camera = GetVirtualCamera(pr.first);

		// アクティブであるかつ、ターゲットがまだ設定されていない場合、ターゲットを設定する
		if (camera->IsActive() && !is_seted_target_transform)
		{
			target_camera = camera;
			m_blend_target_transform = target_camera->GetTransform();
			ChangeTargetVirtualCamera(pr.first);

			is_seted_target_transform = true;
		}
		else if (!is_seted_origin_transform)
		{
			// 以前までターゲットであったカメラを起点に移行
			m_origin_virtual_camera_handle[TimeKind::kCurrent] = m_target_virtual_camera_handle[TimeKind::kPrev];

			if (m_origin_virtual_camera_handle[TimeKind::kCurrent] == pr.first)
			{
				// ブレンド結果が格納されていればブレンド結果を起点とする
				// FIXME : originA➡targetBにブレンド中に、originB➡targetAに切り替わった場合、到達までの時間が早くなる不具合発生中
				if (m_blend_origin_result_transform != nullptr /*
					&& m_target_virtual_camera_handle[TimeKind::kPrev] != m_origin_virtual_camera_handle*/)
				{
					m_blend_origin_transform = m_blend_origin_result_transform;
				}
				// ブレンド結果がない場合はバーチャルカメラのトランスフォームを直接起点とする
				else
				{
					origin_camera = camera;
					m_blend_origin_transform = origin_camera->GetTransform();
				}

				is_seted_origin_transform = true;
			}
		}

		if (is_seted_target_transform && is_seted_origin_transform) { break; }
	}

	// ターゲットのブレンド方針に従ってターゲット以外のカメラのアクティブ状態を制御
	DeactivateVirtualCamera(origin_camera, target_camera);
}

void CinemachineBrain::CalcBlendResuletTransform()
{
	// バーチャルカメラが単独で存在していた場合、
	// もしくはブレンドが完了済みの場合は、ターゲット自身を追尾する
	if (m_blend_origin_transform == nullptr || !m_is_blending)
	{
		m_blend_result_transform = m_blend_target_transform;
		return;
	}

	// トランスフォーム間の補間
	const auto time_manager = GameTimeManager::GetInstance();
	math::Increase(m_blend_timer, time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera), m_blend_time, false);
	m_blend_coefficient			= m_blend_time != 0.0f ? math::GetUnitValue<float, float>(0.0f, m_blend_time, m_blend_timer) : 1.0f;
	auto blended_transform		= math::GetLerpTransform(*m_blend_origin_transform, *m_blend_target_transform, m_blend_coefficient, true, false, true);
	m_blend_result_transform	= std::make_shared<Transform>(blended_transform);

	// ブレンド完了判定
	if (m_blend_coefficient >= 1.0f)
	{
		m_blend_origin_result_transform = nullptr;
		m_is_blending					= false;
	}
}
#pragma endregion
