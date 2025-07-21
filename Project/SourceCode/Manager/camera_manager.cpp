#include "camera_manager.hpp"

CameraManager::CameraManager() :
	m_main_camera			(nullptr),
	m_blend_time			(0.0f),
	m_is_blending			(false),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	SetCameraNearFar		(kNear, kFar);
	SetupCamera_Perspective	(kFOV * math::kDegreesToRadian);

	m_result_transform[TimeKind::kPrev] = m_result_transform[TimeKind::kCurrent] = nullptr;

	test_is_add1 = false;
	test_is_add2 = false;
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

	//BlendVirtualCamera();

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
			transform->SetPos(CoordinateKind::kWorld, VGet(100, 100, -500));
			rot_camera->AttachTarget(transform);

			test_is_add2 = true;
		}
	}

	DrawSphere3D(VGet(100, 100, -500), 20, 8, 0xffffff, 0xffffff, FALSE);

	// キューにソート済みバーチャルカメラのハンドルを格納
	std::queue<int> sorted_camera_handles;
	int add_count = 0;
	for (const auto& pr : m_priority)
	{
		// PrevとCurrentの2種類のみしか不要なため3つ目の追加は許可しない
		if (add_count > 2) { break; }

		sorted_camera_handles.push(pr.first);
		++add_count;
	}

	SetBlendVirtualCamera(sorted_camera_handles);

	// ブレンド結果を格納
	auto result_matrix = MGetIdent();
	if (m_blend_transforms[TimeKind::kPrev])
	{
		auto current_transform = m_blend_transforms[TimeKind::kPrev];
		if (m_result_transform.at(TimeKind::kCurrent))
		{
			current_transform = m_result_transform.at(TimeKind::kCurrent);
		}

		m_result_transform.at(TimeKind::kCurrent) = math::GetInterpolatedTransform(
			m_blend_transforms[TimeKind::kPrev],
			m_blend_transforms[TimeKind::kCurrent],
			current_transform,
			kBlendTime);

		result_matrix = m_result_transform.at(TimeKind::kCurrent)->GetMatrix(CoordinateKind::kWorld);
	}
	else
	{
		result_matrix = m_blend_transforms[TimeKind::kCurrent]->GetMatrix(CoordinateKind::kWorld);
	}


	// メインカメラへ適用
	m_main_camera->GetTransform()->SetMatrix(CoordinateKind::kWorld, result_matrix);
}

void CameraManager::SetBlendVirtualCamera(std::queue<int>& sorted_camera_handles)
{
	for (int i = 0; i < 2; ++i)
	{
		if (sorted_camera_handles.empty()) { break; }

		const int handle = sorted_camera_handles.front();

		switch (i)
		{
		case 0:
			m_blend_transforms[TimeKind::kCurrent] = GetVirtualCamera(handle)->GetTransform();
			break;

		case 1:
			// ブレンド結果が格納されていればそれを採用
			if (m_result_transform.at(TimeKind::kPrev))
			{
				m_blend_transforms[TimeKind::kPrev] = m_result_transform.at(TimeKind::kPrev);
			}
			// ブレンド結果がない場合はバーチャルカメラのトランスフォームを直接格納
			else
			{
				m_blend_transforms[TimeKind::kPrev] = GetVirtualCamera(handle)->GetTransform();
			}
			break;
		}

		sorted_camera_handles.pop();
	}
}
