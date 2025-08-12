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
	[[nodiscard]] int GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	void SetupForRotCamera();
	void SetupForZoomInCamera();
	void SetupForZoomOutCamera();

	void CalcAimTransformForRotCamera();
	void CalcAimTransformForZoomInCamera();
	void CalcAimTransformForZoomOutCamera();

private:
	static constexpr VECTOR kFirstFollowOffsetForRotCamera				= {  9.0f, 6.0f, -50.0f };
	static constexpr VECTOR kFirstFollowOffsetForZoomInCamera			= {  9.0f, 6.0f, -50.0f };
	static constexpr VECTOR kDestinationFollowOffsetForZoomInCamera		= {  9.0f, 6.0f, -34.0f };
	static constexpr VECTOR kTrackedObjOffsetForRotCamera				= { -2.0f, 2.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForZoomCamera				= { -2.0f, 2.0f,   0.0f };
	static constexpr VECTOR kFirstAngleForRotCamera						= { 0.0f, 0.0f * math::kDegToRad, 0.0f };

	static constexpr float  kRotAcceleration							= 0.7f;
	static constexpr float  kZoomOutDeceleration						= 12.0f;
	static constexpr float  kZoomOutMaxDeceleration						= 0.3f;
	static constexpr float  kZoomOutInitialVelocity						= 1.0f;
	static constexpr float  kZoomInDamping								= 0.5f;

private:
	VirtualCameraControllerKind		m_virtual_camera_controller_kind;
	int								m_controller_handle;
	bool							m_is_active;

	Player& m_player;
	std::shared_ptr<Subject<RocketLauncherVirtualCameraController>> m_subject;

	std::shared_ptr<VirtualCamera>	m_rot_camera;
	std::shared_ptr<VirtualCamera>	m_zoom_in_camera;
	std::shared_ptr<VirtualCamera>	m_zoom_out_camera;

	std::shared_ptr<Transform>		m_rot_camera_aim_transform;
	std::shared_ptr<Transform>		m_zoom_camera_aim_transform;

	VECTOR	m_follow_offset_for_zoom_in;
	VECTOR	m_follow_offset_for_zoom_out;
	VECTOR  m_rot_camera_angle;
	float	m_zoom_in_wait_timer;
	float   m_zoom_out_speed;
};
