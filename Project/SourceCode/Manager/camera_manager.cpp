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
	SetCameraNearFar		(kNear, kFar);
	SetupCamera_Perspective	(kFOV * math::kDegreesToRadian);
}

CameraManager::~CameraManager()
{

}

void CameraManager::Update()
{
	for (const auto& camera : m_virtual_cameras)
	{
		camera.second->Update();
	}

	BlendVirtualCamera();

	assert(m_main_camera != nullptr);
	m_main_camera->Update();
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
	// 仮の追加
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_1) == InputState::kSingle)
	{
		if (!test_is_add1)
		{
			const auto rot_camera = std::make_shared<RotControlVirtualCamera>(2);
			AddVirtualCamera(rot_camera);
			rot_camera->AttachTarget(std::make_shared<Transform>());

			handle2 = rot_camera->GetObjHandle();

			test_is_add1 = true;		
		}
	}
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_2) == InputState::kSingle)
	{
		if (!test_is_add2)
		{
			const auto rot_camera = std::make_shared<RotControlVirtualCamera>(3);
			AddVirtualCamera(rot_camera);
			const auto transform = std::make_shared<Transform>();
			transform->SetPos(CoordinateKind::kWorld, VGet(-500, 500, -500));
			rot_camera->AttachTarget(transform);

			handle3 = rot_camera->GetObjHandle();

			test_is_add2 = true;
		}
	}
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_3) == InputState::kSingle)
	{
		GetVirtualCamera(handle2)->Deactivate();
	}
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_4) == InputState::kSingle)
	{
		GetVirtualCamera(handle2)->Activate();
	}
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_5) == InputState::kSingle)
	{
		GetVirtualCamera(handle3)->Deactivate();
	}
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_6) == InputState::kSingle)
	{
		GetVirtualCamera(handle3)->Activate();
	}

	//m_target_virtual_camera_handle[TimeKind::kPrev] = m_target_virtual_camera_handle[TimeKind::kCurrent];

	// ブレンドの起点とターゲットを設定
	SetBlendTransform();

	// ブレンド結果を計算
	CalcBlendResuletTransform();

	DrawFormatString(0, 20, 0xffffff, "blend_timer         : %f", m_blend_timer);
	DrawFormatString(0, 40, 0xffffff, "is_blending         : %d", m_is_blending); 
	DrawFormatString(0, 60, 0xffffff, "m_blend_coefficient : %f", m_blend_coefficient);

	if(m_blend_result_transform)
	DrawSphere3D(m_blend_result_transform->GetPos(CoordinateKind::kWorld), 5, 8, 0xffffff, 0xffffff, TRUE);

	matrix::Draw(400, 0, m_blend_result_transform->GetMatrix(CoordinateKind::kWorld));

	int c = 0;
	for (const auto& pr : m_priority)
	{
		DrawFormatString(0, 80 + (c * 20), 0xffffff, "handle %3d : is_active %d", pr.first, GetVirtualCamera(pr.first)->IsActive());
		++c;
	}

	// メインカメラへ適用
	m_main_camera->GetTransform()->SetMatrix(CoordinateKind::kWorld, m_blend_result_transform->GetMatrix(CoordinateKind::kWorld));
}

void CameraManager::ChangeTargetVirtualCamera(const int obj_handle)
{
	m_target_virtual_camera_handle[TimeKind::kPrev]		= m_target_virtual_camera_handle[TimeKind::kCurrent];
	m_target_virtual_camera_handle[TimeKind::kCurrent]	= obj_handle;

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
	std::vector<bool> active_list;
	bool is_seted_target	= false;
	int  loop_count			= 0;;

	// 過去の状態を保存

	for (const auto& pr : m_priority)
	{
		active_list.emplace_back((GetVirtualCamera(pr.first))->IsActive());

		// アクティブであるかつ、ターゲットがまだ設定されていない場合、ターゲットを設定する
		if (active_list.at(loop_count) && !is_seted_target)
		{
			m_blend_target_transform = GetVirtualCamera(pr.first)->GetTransform();
			ChangeTargetVirtualCamera(pr.first);

			is_seted_target	= true;
		}
		else
		{
			// ブレンド結果が格納されていればブレンド結果を起点とする
			if (m_blend_origin_result_transform != nullptr)
			{
				m_blend_origin_transform = m_blend_origin_result_transform;
			}
			// ブレンド結果がない場合はバーチャルカメラのトランスフォームを直接起点とする
			else
			{
				m_blend_origin_transform = GetVirtualCamera(pr.first)->GetTransform();
			}
		}

		++loop_count;
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

	// ブレンド係数を計算
	math::Increase(m_blend_timer, FPS::GetDeltaTime(), kBlendTime);
	m_blend_coefficient = math::GetUnitValue<float, float>(0.0f, kBlendTime, m_blend_timer);

	// トランスフォーム間の補間
	m_blend_result_transform = math::GetLerpTransform(m_blend_origin_transform, m_blend_target_transform, m_blend_coefficient, true, false, true);

	// ブレンド完了判定
	if (m_blend_coefficient >= 1.0f)
	{
		m_blend_origin_result_transform = nullptr;
		m_is_blending					= false;
	}
}
#pragma endregion

