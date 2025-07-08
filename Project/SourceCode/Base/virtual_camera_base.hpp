#pragma once
#include "obj_base.hpp"

#include "../Part/camera_body.hpp"
#include "../Part/camera_aim.hpp"
#include "../Part/camera_noise.hpp"

#include "../Data/Kind/scene_kind.hpp"
#include "../Data/Kind/virtual_camera_kind.hpp"

class VirtualCameraBase abstract : public ObjBase
{
public:
	VirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind);
	virtual ~VirtualCameraBase() = default;

	[[nodiscard]] int				GetPriority()		const { return m_priority; }
	[[nodiscard]] VirtualCameraKind GetCameraKind()		const { return m_camera_kind; }
	[[nodiscard]] bool				IsActiveCamera()	const { return m_is_active_camera; }

protected:
	std::shared_ptr<CameraBody>		m_body;
	std::shared_ptr<CameraAim>		m_aim;
	std::shared_ptr<CameraNoise>	m_noise;

	int								m_priority;				// 優先度
	bool							m_is_active_camera;		// 
	VirtualCameraKind				m_camera_kind;			// カメラの種類
	std::vector<SceneKind>			m_active_scene_kind;	// どのシーンでアクティブ化されるかを格納
};
