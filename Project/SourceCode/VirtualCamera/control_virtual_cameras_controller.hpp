#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"

#include "virtual_camera.hpp"
#include "../Data/recoil_data.hpp"
#include "../Event/event_system.hpp"

class CinemachineBrain;
class Player;
class GunBase;

class ControlVirtualCamerasController final : public IVirtualCameraController
{
public:
	ControlVirtualCamerasController(Player& player);
	~ControlVirtualCamerasController();

	void Init();
	void Update()		override;
	void LateUpdate()	override;

	/// @brief 銃が撃たれた際のリコイル処理
	void OnRecoil(const GunBase& gun);

	void Activate()   override { m_is_active = true; }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VECTOR GetVelocity()		 const			{ return m_velocity; }
	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetHaveVirtualCamera(const std::string& name) const override;
	[[nodiscard]] std::vector<std::shared_ptr<VirtualCamera>> GetHaveAllVirtualCamera()  const override;
	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsRecoiling()		 const			{ return m_is_recoiling; }
	[[nodiscard]] bool IsReachedRecoilPeak() const			{ return m_is_reached_recoil_peak; }
	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }

private:
	#pragma region Event
	void EndGrabCutscene(const EndCutsceneEvent& event);
	#pragma endregion


	void InitAngle();

	void SetupForRotCamera();
	void SetupForAimCamera();

	void Move();

	void CalcMoveDirFromPad();
	void CalcMoveDirFromMouse();
	void CalcMoveDirFromCommand();

	void CalcOffsetFromRotCamera();
	void CalcOffsetFromAimCamera();

	void CalcAimPos();
	void CalcInputAngle();
	void CalcRecoilAngle();
	void CalcResultAngle();
	//void CalcInitAim();

	/// @brief 無加工のボーンをカメラが追尾するかを判定
	/// @return true : ボーンそのものをカメラが追尾, false : ボーンを同じ高さにある位置を追尾
	[[nodiscard]] bool IsTrackCameraOriginFrame() const;

private:
	static constexpr VECTOR kFollowOffsetForRotCamera				= { 30.0f, 20.0f, -90.0f };
	static constexpr VECTOR kFollowOffsetForRotCameraCrouch			= { 30.0f, 20.0f, -70.0f };
	static constexpr VECTOR kFollowOffsetForAimCamera				= { 20.0f, 10.0f, -40.0f };
	static constexpr VECTOR kFollowOffsetForAimCameraKnife			= { 30.0f, 20.0f, -70.0f };
	static constexpr VECTOR kTrackedObjOffsetForRotCamera			= { 10.0f, 10.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForRotCameraCrouch		= { 10.0f, 10.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForAimCamera			= { 13.0f,  7.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForAimCameraKnife		= { 10.0f, 10.0f,   0.0f };

	static constexpr VECTOR kBodyDampingForRotCamera				= { 0.015f, 0.015f, 0.0f };
	static constexpr VECTOR kBodyDampingForAimCamera				= { 0.0f,   0.0f,   0.0f };
	static constexpr float  kHorizontalDampingForRotCamera			= 0.05f;
	static constexpr float  kHorizontalDampingForAimCamera			= 0.0f;
	static constexpr float  kVerticalDampingForRotCamera			= 0.0f;
	static constexpr float  kVerticalDampingForAimCamera			= 0.0f;

	static constexpr float  kCameraAimOffsetBasicSpeed				= 40.0f;

	static constexpr float  kMaxVerticalInputAngle					= 60.0f;
	static constexpr float  kMinVerticalInputAngle					= -60.0f;
	static constexpr float	kMoveSpeedWithStick						= 1.5f;
	static constexpr float	kMoveSpeedWithMouse						= 0.1f;
	static constexpr float	kMoveSpeedWithButton					= 2.0f;

	static constexpr float  kRecoilEndThreshold						= 0.002f;

private:
	VirtualCameraControllerKind				m_virtual_camera_controller_kind;
	int										m_controller_handle;
	bool									m_is_active;

	Player& m_player;

	std::shared_ptr<VirtualCamera>			m_rot_control_camera;				// 回転カメラ
	std::shared_ptr<VirtualCamera>			m_aim_control_camera;				// エイミング時カメラ

	std::shared_ptr<Transform>				m_aim_transform;					// カメラが起点とするトランスフォーム
	VECTOR									m_current_aim_pos;					// 現在の起点座標

	VECTOR									m_move_dir;							// 移動方向
	VECTOR									m_velocity;							// 速度ベクトル
	std::unordered_map<TimeKind, VECTOR>	m_input_angle;						// 入力角度
	std::unordered_map<TimeKind, VECTOR>	m_recoil_angle;						// リコイル角度
	VECTOR									m_result_angle;						// リザルト角度

	RecoilData								m_recoil_data;
	bool									m_is_recoiling;
	bool									m_is_reached_recoil_peak;
	float									m_recoil_timer;

	//float									init_angle_speed	= 0.0f;			// 視点リセット速度
	//float									init_end_threshold	= 0.0f;			// 視点リセットを終了する閾値
	//bool									is_init_aiming		= false;		// 視点リセット中かを判定

};
