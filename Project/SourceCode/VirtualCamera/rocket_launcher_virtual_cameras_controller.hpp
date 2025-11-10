#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"
#include "../Base/obj_base.hpp"

class CinemachineBrain;
class UIDrawer;
class Player;

class RocketLauncherVirtualCamerasController final : public IVirtualCameraController
{
public:
	RocketLauncherVirtualCamerasController(Player& player);
	~RocketLauncherVirtualCamerasController();

	void Init();
	void Update()		override;
	void LateUpdate()	override;

	void Activate()   override { m_is_active = true; }
	void Deactivate() override { m_is_active = false; }

	void SetRocketBomb(const std::shared_ptr<ObjBase>& rocket_bomb) { m_rocket_bomb = rocket_bomb; }

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetHaveVirtualCamera(const std::string& name) const override;
	[[nodiscard]] std::vector<std::shared_ptr<VirtualCamera>> GetHaveAllVirtualCamera()  const override;
	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsEndExitRot()		 const { return m_rot_camera_angle.y == -DX_TWO_PI_F; }
	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }

private:
	void SetupForEnterRotCamera();
	void SetupForZoomInCamera();
	void SetupForZoomOutCamera();
	void SetupForExitRotCamera();

	void CalcAimTransformForEnterRotCamera();
	void CalcAimTransformForZoomInCamera();
	void CalcAimTransformForZoomOutCamera();
	void CalcAimTransformForExitRotCamera();

	void Exit();

private:
	static constexpr VECTOR kFirstFollowOffsetForEnterRotCamera		= {  9.0f, 6.0f, -50.0f };
	static constexpr VECTOR kFirstFollowOffsetForZoomInCamera		= {  9.0f, 6.0f, -50.0f };
	static constexpr VECTOR kFirstFollowOffsetForZoomOutCamera		= {  2.0f, 0.0f,   0.0f };
	static constexpr VECTOR kDestinationFollowOffsetForZoomInCamera = {  9.0f, 6.0f, -34.0f };
	static constexpr VECTOR kTrackedObjOffsetForEnterRotCamera		= { -2.0f, 2.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForZoomInCamera		= { -2.0f, 2.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForZoomOutCamera		= {  2.0f, 0.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForExitRotCamera		= {  0.0f, 0.0f,   0.0f };
	static constexpr VECTOR kFirstAngleForEnterRotCamera			= { 0.0f, 0.0f * math::kDegToRad, 0.0f };

	static constexpr float  kEnterRotAcceleration	= 2.5f * 1.1;
	static constexpr float  kExitRotAcceleration	= 2.0f * 2.1f;

	static constexpr float  kZoomOutSpeed		= 0.09f;
	static constexpr float  kZoomInDamping		= 0.2f;
	static constexpr float  kZoomOutTime		= 1.5f;
	static constexpr float  kExitRotLeaveSpeed	= 70.0f;

private:
	VirtualCameraControllerKind		m_virtual_camera_controller_kind;
	int								m_controller_handle;
	bool							m_is_active;

	Player& m_player;
	std::shared_ptr<ObjBase>		m_rocket_bomb;

	std::shared_ptr<VirtualCamera>	m_enter_rot_camera;
	std::shared_ptr<VirtualCamera>	m_zoom_in_camera;
	std::shared_ptr<VirtualCamera>	m_zoom_out_camera;
	std::shared_ptr<VirtualCamera>	m_exit_rot_camera;

	std::shared_ptr<Transform>		m_rot_camera_aim_transform;
	std::shared_ptr<Transform>		m_zoom_camera_aim_transform;

	VECTOR	m_follow_offset_for_zoom_in;
	VECTOR	m_follow_offset_for_zoom_out;
	VECTOR	m_follow_offset_for_exit_rot;
	VECTOR  m_rot_camera_angle;
	float	m_zoom_in_wait_timer;
	float   m_zoom_out_timer;

	float   m_test_timer; // ‰¼
};
