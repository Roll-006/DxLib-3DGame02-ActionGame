//#pragma once
//#include "../Interface/i_virtual_camera_controller.hpp"
//#include "virtual_camera.hpp"
//
//class CinemachineBrain;
//class Player;
//
//class GrabVirtualCameraController final : public IVirtualCameraController
//{
//public:
//	GrabVirtualCameraController(Player& player);
//	~GrabVirtualCameraController();
//
//	void Init();
//	void Update();
//	void LateUpdate();
//
//	void Activate()   override { m_is_active = true;  }
//	void Deactivate() override { m_is_active = false; }
//
//	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
//	[[nodiscard]] std::shared_ptr<VirtualCameraBase> GetHaveVirtualCamera(const std::string& name) const override;
//	[[nodiscard]] std::vector<std::shared_ptr<VirtualCameraBase>> GetHaveAllVirtualCamera()  const override;
//	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
//	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }
//
//private:
//	void SetupCamera();
//
//	void CalcAimTransform();
//
//private:
//	static constexpr VECTOR kFollowOffset		= {  9.0f, 6.0f, -50.0f };
//	static constexpr VECTOR kTrackedObjOffset	= { -2.0f, 2.0f,   0.0f };
//
//private:
//	VirtualCameraControllerKind		m_virtual_camera_controller_kind;
//	int								m_controller_handle;
//	bool							m_is_active;
//
//	Player&							m_player;
//	std::shared_ptr<Subject<GrabVirtualCameraController>> m_subject;
//
//	std::shared_ptr<VirtualCamera>	m_camera;
//	std::shared_ptr<Transform>		m_aim_transform;
//};
