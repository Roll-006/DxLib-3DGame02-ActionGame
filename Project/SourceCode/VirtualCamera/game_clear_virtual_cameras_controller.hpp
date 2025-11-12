#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"

#include "../Event/victory_pose_event.hpp"

class CinemachineBrain;

class GameClearVirtualCamerasController final : public IVirtualCameraController
{
public:
	GameClearVirtualCamerasController(const int model_handle, const std::shared_ptr<Transform>& boss_transform);
	~GameClearVirtualCamerasController();

	void Init();
	void Update()		override;
	void LateUpdate()	override;

	void Activate()   override { m_is_active = true;  }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetHaveVirtualCamera(const std::string& name) const override;
	[[nodiscard]] std::vector<std::shared_ptr<VirtualCamera>> GetHaveAllVirtualCamera()  const override;
	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }

private:
	void SetVictoryPoseCamera(const VictoryPoseEvent& event);

	void SetupDeadBossCamera();
	void SetupVictoryPoseCamera();

	void CalcAimTransformForDeadBossCamera();
	void CalcAimTranasormForVictoryPoseCamera();
	void CalcFollowOffset();

private:
	static constexpr VECTOR kFollowOffset			= { 0.0f, 50.0f, -180.0f };
	static constexpr VECTOR kTrackedObjOffset		= { 0.0f,  0.0f,    0.0f };
	static constexpr float  kZoomInInitialVelocity	= 1.4f;
	static constexpr float  kZoomInDeceleration		= 1.0f;
	static constexpr float  kZoomInWaitTime			= 0.0f;

private:
	VirtualCameraControllerKind		m_virtual_camera_controller_kind;
	int								m_controller_handle;
	bool							m_is_active;

	std::shared_ptr<VirtualCamera>	m_dead_boss_camera;
	std::shared_ptr<VirtualCamera>	m_victory_pose_camera;

	std::shared_ptr<Transform>		m_aim_transform;
	VECTOR							m_follow_offset;
	VECTOR							m_follow_offset_dir;

	float							m_zoom_in_wait_time;
	float							m_zoom_in_speed;

	std::shared_ptr<Modeler>		m_player_modeler;
	int								m_boss_model_handle;
	std::shared_ptr<Transform>		m_player_transform;
	std::shared_ptr<Transform>		m_boss_transform;
};
