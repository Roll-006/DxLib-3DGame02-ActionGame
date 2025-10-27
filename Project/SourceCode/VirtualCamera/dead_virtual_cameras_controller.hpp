#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"

#include "virtual_camera.hpp"
#include "../Event/event_system.hpp"

class CinemachineBrain;

class DeadVirtualCamerasController final : public IVirtualCameraController
{
public:
	DeadVirtualCamerasController();
	~DeadVirtualCamerasController();

	void Init();
	void Update()		override;
	void LateUpdate()	override;

	void Activate()   override { m_is_active = true; }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetHaveVirtualCamera(const std::string& name) const override;
	[[nodiscard]] std::vector<std::shared_ptr<VirtualCamera>> GetHaveAllVirtualCamera()  const override;
	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }

private:
	#pragma region Event
	void ActiveDeadCamera(const DeadPlayerEvent& event);
	#pragma endregion


	void SetupCamera();

	void CalcAimTransform();

private:
	static constexpr VECTOR kFollowOffset		= { 0.0f, 0.0f, -100.0f };
	static constexpr VECTOR kTrackedObjOffset	= { 0.0f, 0.0f, 0.0f };

private:
	VirtualCameraControllerKind				m_virtual_camera_controller_kind;
	int										m_controller_handle;
	bool									m_is_active;

	std::shared_ptr<VirtualCamera>			m_dead_camera;
	std::shared_ptr<Transform>				m_aim_transform;
	int										m_model_handle;
};
