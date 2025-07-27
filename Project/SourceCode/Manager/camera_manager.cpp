#include "camera_manager.hpp"

CameraManager::CameraManager() :
	m_main_camera					(nullptr),
	m_blend_origin_transform		(nullptr),
	m_blend_target_transform		(nullptr),
	m_blend_origin_result_transform	(nullptr),
	m_blend_result_transform		(nullptr),
	m_blend_timer					(0.0f),
	m_blend_coefficient				(0.0f),
	m_is_blending					(false),
	m_is_invert_horizontal			(false),
	m_is_invert_vertical			(false)
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);
}

CameraManager::~CameraManager()
{

}

void CameraManager::Update()
{
	// TODO : 仮の実装。のちに削除。
	{
		//if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_1) == InputState::kSingle)
		{
			if (!test_is_add1)
			{
				const auto rot_camera = std::make_shared<RotControlVirtualCamera>(2);
				AddVirtualCamera(rot_camera);
				transform1 = std::make_shared<Transform>();
				rot_camera->AttachTarget(transform1);

				handle2 = rot_camera->GetObjHandle();
				GetVirtualCamera(handle2)->Deactivate();

				test_is_add1 = true;
			}
		}
		if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_2) == InputState::kSingle)
		{
			GetVirtualCamera(handle2)->Deactivate();
		}
		if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_3) == InputState::kSingle)
		{
			GetVirtualCamera(handle2)->Activate();
		}
		//if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_4) == InputState::kSingle)
		{
			if (!test_is_add2)
			{
				const auto rot_camera = std::make_shared<RotControlVirtualCamera>(3);
				AddVirtualCamera(rot_camera);
				transform2 = std::make_shared<Transform>();
				transform2->SetPos(CoordinateKind::kWorld, VGet(-500, 500, -500));
				rot_camera->AttachTarget(transform2);

				handle3 = rot_camera->GetObjHandle();
				GetVirtualCamera(handle3)->Deactivate();

				test_is_add2 = true;
			}
		}
		if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_5) == InputState::kSingle)
		{
			GetVirtualCamera(handle3)->Deactivate();
		}
		if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_6) == InputState::kSingle)
		{
			GetVirtualCamera(handle3)->Activate();
		}
	}

	for (const auto& camera : m_virtual_cameras)
	{
		camera.second->Update();
	}
}

void CameraManager::LateUpdate()
{
	for (const auto& camera : m_virtual_cameras)
	{
		camera.second->LateUpdate();
	}

	BlendVirtualCamera();

	m_main_camera->LateUpdate();
}

void CameraManager::SetMainCamera(const std::shared_ptr<MainCamera> main_camera)
{
	if (!m_main_camera)
	{
		m_main_camera = main_camera;
	}
}

void CameraManager::RemoveVirtualCamera(const int obj_handle)
{
	m_virtual_cameras.erase(obj_handle);
}


#pragma region Getter
std::shared_ptr<VirtualCameraBase> CameraManager::GetVirtualCamera(const int obj_handle) const
{
	return m_virtual_cameras.count(obj_handle) ? m_virtual_cameras.at(obj_handle) : nullptr;
}

std::shared_ptr<VirtualCameraBase> CameraManager::GetVirtualCamera(const std::string& obj_name) const
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

std::shared_ptr<VirtualCameraBase> CameraManager::GetVirtualCamera(const VirtualCameraKind camra_kind) const
{
	for (const auto& camera : m_virtual_cameras)
	{
		if (camera.second->GetCameraKind() == camra_kind)
		{
			return camera.second;
		}
	}
	return nullptr;
}
#pragma endregion


#pragma region ブレンド関連処理
void CameraManager::BlendVirtualCamera()
{
	// ブレンドの起点とターゲットを設定
	SetBlendTransform();

	// ブレンド結果を計算
	CalcBlendResuletTransform();

	// メインカメラへ適用
	m_main_camera->GetTransform()->SetMatrix(CoordinateKind::kWorld, m_blend_result_transform->GetMatrix(CoordinateKind::kWorld));

	if(m_blend_result_transform)matrix::Draw(  0,  40, m_blend_result_transform->GetMatrix(CoordinateKind::kWorld));
	if(m_blend_origin_transform)matrix::Draw(  0, 140, m_blend_origin_transform->GetMatrix(CoordinateKind::kWorld));
	if(m_blend_target_transform)matrix::Draw(700, 140, m_blend_target_transform->GetMatrix(CoordinateKind::kWorld));
}

void CameraManager::ChangeTargetVirtualCamera(const int obj_handle)
{
	m_target_virtual_camera_handle[TimeKind::kPrev]    = m_target_virtual_camera_handle[TimeKind::kCurrent];
	m_target_virtual_camera_handle[TimeKind::kCurrent] = obj_handle;

	if (m_target_virtual_camera_handle[TimeKind::kPrev] != m_target_virtual_camera_handle[TimeKind::kCurrent])
	{
		// ブレンド中に最優先カメラが切り替わった場合、それまでのブレンド結果をブレンドの起点とする
		if (m_blend_timer != kBlendTime)
		{
			m_blend_origin_result_transform = m_blend_result_transform;
		}

		m_blend_timer = 0.0f;
		m_is_blending = true;
	}
}

void CameraManager::SetBlendTransform()
{
	// FIXME : ブレンドが開始した一瞬、別の地点が描画される現象発生中

	bool is_seted_target_transform = false;
	bool is_seted_origin_transform = false;

	for (const auto& pr : m_priority)
	{
		// アクティブであるかつ、ターゲットがまだ設定されていない場合、ターゲットを設定する
		if (GetVirtualCamera(pr.first)->IsActive() && !is_seted_target_transform)
		{
			m_blend_target_transform = GetVirtualCamera(pr.first)->GetTransform();
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
					m_blend_origin_transform = GetVirtualCamera(pr.first)->GetTransform();
				}

				is_seted_origin_transform = true;
			}
		}

		if (is_seted_target_transform && is_seted_origin_transform) { break; }
	}
}

void CameraManager::CalcBlendResuletTransform()
{
	// バーチャルカメラが単独で存在していた場合、
	// もしくはブレンドが完了済みの場合は、ターゲット自身を追尾する
	if (m_blend_origin_transform == nullptr || !m_is_blending)
	{
		m_blend_result_transform = m_blend_target_transform;
		return;
	}

	// トランスフォーム間の補間
	math::Increase(m_blend_timer, FPS::GetDeltaTime(), kBlendTime);
	m_blend_coefficient			= math::GetUnitValue<float, float>(0.0f, kBlendTime, m_blend_timer);
	auto blended_transform		= math::GetLerpTransform(*m_blend_origin_transform, *m_blend_target_transform, m_blend_coefficient, true, false, true);
	m_blend_result_transform	= std::make_shared<Transform>(blended_transform);

	//DrawFormatString(600,  0, 0xffffff, "m_blend_timer       : %f", m_blend_timer);
	//DrawFormatString(600, 20, 0xffffff, "m_blend_coefficient : %f", m_blend_coefficient);

	// ブレンド完了判定
	if (m_blend_coefficient >= 1.0f)
	{
		m_blend_origin_result_transform = nullptr;
		m_is_blending					= false;
	}
}
#pragma endregion

