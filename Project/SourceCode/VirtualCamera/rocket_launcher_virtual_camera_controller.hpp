#pragma once
#include "../Interface/i_observer.hpp"
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"

class CameraManager;
class Player;

class RocketLauncherVirtualCameraController final : public IObserver, public IVirtualCameraController
{
public:
	RocketLauncherVirtualCameraController(const std::shared_ptr<Player> player);
	~RocketLauncherVirtualCameraController();

	void Init();
	void Update();
	void LateUpdate();

	void Activate()   override { m_is_active = true;  }
	void Deactivate() override { m_is_active = false; }

	void OnNotify(const IEvent& event) override;

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	void CalcAimTransformForRotCamera();
	void CalcAimTransformForZoomOutCamera();

private:
	std::shared_ptr<Player> m_player;

	std::shared_ptr<VirtualCamera>	m_rot_angle_camera;
	std::shared_ptr<VirtualCamera>	m_zoom_out_camera;

	std::shared_ptr<Transform>		m_rot_camera_aim_transform;
	std::shared_ptr<Transform>		m_zoom_out_camera_aim_transform;

	VirtualCameraControllerKind		m_virtual_camera_controller_kind;

	bool m_is_active;
};
