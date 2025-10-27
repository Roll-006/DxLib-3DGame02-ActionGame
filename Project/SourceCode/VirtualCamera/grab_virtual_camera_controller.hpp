#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"
#include "../Event/grab_event.hpp"
#include "../Event/on_grab_event.hpp"

class CinemachineBrain;

class GrabVirtualCameraController final : public IVirtualCameraController
{
public:
	GrabVirtualCameraController();
	~GrabVirtualCameraController();

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
	#pragma region Event
	void SetGrabberModelHandle(const GrabEvent&		event);
	void SetGrabbedModelHandle(const OnGrabEvent&	event);
	#pragma endregion


	void SetupCamera();

	void CalcAimTransform();

private:
	static constexpr VECTOR kFollowOffset		= { 0.0f, -10.0f, -50.0f };
	static constexpr VECTOR kTrackedObjOffset	= { 5.0f,   0.0f,   0.0f };

private:
	VirtualCameraControllerKind		m_virtual_camera_controller_kind;
	int								m_controller_handle;
	bool							m_is_active;

	std::shared_ptr<Modeler>		m_grabber_modeler;		// 掴む側のモデルハンドル
	std::shared_ptr<Modeler>		m_grabbed_modeler;		// 掴まれる側のモデルハンドル

	std::shared_ptr<VirtualCamera>	m_camera;
	std::shared_ptr<Transform>		m_aim_transform;
};
