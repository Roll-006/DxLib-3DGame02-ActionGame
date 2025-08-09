#pragma once
#include "virtual_camera.hpp"
#include "../Interface/i_observer.hpp"

#include "../Manager/camera_manager.hpp"

class RocketLauncherVirtualCameraController final : public IObserver
{
public:
	RocketLauncherVirtualCameraController();
	~RocketLauncherVirtualCameraController();

	void Init();
	void Update();
	void LateUpdate();

	void OnNotify(const IEvent& event) override;

private:
	std::shared_ptr<VirtualCamera> m_rot_angle_camera;
	std::shared_ptr<VirtualCamera> m_zoom_out_camera;
};
