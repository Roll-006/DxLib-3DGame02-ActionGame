#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"

class CameraManager;
class Player;

class RocketLauncherVirtualCameraController final : public IVirtualCameraController
{
public:
	RocketLauncherVirtualCameraController(Player& player);
	~RocketLauncherVirtualCameraController();

	void Init();
	void Update();
	void LateUpdate();

	void Activate()   override { m_is_active = true;  }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	void CalcAimTransformForRotCamera();
	void CalcAimTransformForZoomOutCamera();

private:
	static constexpr VECTOR kFollowOffsetForRot			= {  0.0f, 0.0f, -50.0f };
	static constexpr VECTOR kTrackedObjOffsetForRot		= { 10.0f, 0.0f,   0.0f };
	static constexpr VECTOR kFollowOffsetForZoomOut		= {  0.0f, 0.0f, -50.0f };
	static constexpr VECTOR kTrackedObjOffsetForZoomOut	= { 10.0f, 0.0f,   0.0f };
	static constexpr VECTOR kFirstRotCameraAngle		= { 0.0f, -60.0f * math::kDegToRad, 0.0f };
	static constexpr float  kZoomOutkAcceleration		= 2.8f;
	static constexpr float  kRotAcceleration			= 0.7f;

	Player& m_player;

	std::shared_ptr<VirtualCamera>	m_rot_camera;
	std::shared_ptr<VirtualCamera>	m_zoom_out_camera;
	std::shared_ptr<Transform>		m_rot_camera_aim_transform;
	std::shared_ptr<Transform>		m_zoom_out_camera_aim_transform;

	VirtualCameraControllerKind		m_virtual_camera_controller_kind;

	VECTOR	m_follow_offset_for_zoom_out;
	VECTOR  m_rot_camera_angle;
	bool	m_is_active;
};
