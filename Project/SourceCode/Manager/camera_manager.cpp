#include "camera_manager.hpp"

CameraManager::CameraManager() :
	m_main_camera			(nullptr),
	m_blend_time			(0.0f),
	m_is_blending			(false),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{

}

CameraManager::~CameraManager()
{

}

void CameraManager::Update()
{
	for (const auto& camera : m_virtual_camera)
	{
		camera.second->Update();
	}
}

void CameraManager::LateUpdate()
{
	for (const auto& camera : m_virtual_camera)
	{
		camera.second->LateUpdate();
	}
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
	m_virtual_camera.erase(obj_handle);
}

void CameraManager::BlendVirtualCamera()
{
	// カメラを優先度順にソート
	std::unordered_map<int, int> obj_handle;
	std::unordered_map<int, int> priority;
	for (int i = 0; i < m_virtual_camera.size(); ++i)
	{
		obj_handle[i] = m_virtual_camera.at(i)->GetObjHandle();
		priority[i]	  = m_virtual_camera.at(i)->GetPriority();
	}
	priority = algorithm::Sort(priority, SortKind::kDescending);

	// TODO : 仮
	m_main_camera->GetTransform()->SetMatrix(CoordinateKind::kWorld, GetVirtualCamera<RotControlVirtualCamera>(VirtualCameraKind::kRotControl)->GetTransform()->GetMatrix(CoordinateKind::kWorld));
}

void CameraManager::ApplyBlendResultMatrix()
{
	
}

//#pragma region アタッチ・デタッチ
//void MainCamera::AttachTarget(const std::shared_ptr<ObjBase> obj)
//{
//	m_target_transform = obj->GetTransform();
//}
//
//void MainCamera::AttachTarget(const std::string& obj_name)
//{
//	auto target_obj = ObjManager::GetInstance()->GetObj<ObjBase>(obj_name);
//	AttachTarget(target_obj);
//}
//
//void MainCamera::AttachTarget(const std::shared_ptr<ObjBase> obj, const int model_handle, const std::string& bone_path, const bool is_track_height_only)
//{
//	AttachTarget(obj);
//
//	m_target_model_handle = model_handle;
//	m_target_bone = bone_path;
//	m_is_track_height_only = is_track_height_only;
//}
//
//void MainCamera::AttachTarget(const std::string& obj_name, const int model_handle, const std::string& bone_path, const bool is_track_height_only)
//{
//	AttachTarget(obj_name);
//
//	m_target_model_handle = model_handle;
//	m_target_bone = bone_path;
//	m_is_track_height_only = is_track_height_only;
//}
//
//void MainCamera::DetachTarget()
//{
//	m_target_transform = nullptr;
//	m_target_model_handle = -1;
//	m_target_bone = "";
//	m_is_track_height_only = false;
//}
//#pragma endregion
//
//#pragma region ターゲットとの距離
//void MainCamera::Approach(const float min_distance, const float move_speed)
//{
//	m_distance_to_target -= move_speed;
//	if (std::abs(m_distance_to_target) < min_distance)
//	{
//		m_distance_to_target = min_distance;
//	}
//}
//
//void MainCamera::Depart(const float max_distance, const float move_speed)
//{
//	m_distance_to_target += move_speed;
//	if (std::abs(m_distance_to_target) > max_distance)
//	{
//		m_distance_to_target = max_distance;
//	}
//}
//#pragma endregion
