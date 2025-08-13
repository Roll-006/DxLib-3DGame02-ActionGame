#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"

#include "../Data/recoil_data.hpp"

class CameraManager;
class Player;
class GunBase;

class ControlVirtualCamerasController final : public IVirtualCameraController
{
public:
	ControlVirtualCamerasController(Player& player);
	~ControlVirtualCamerasController();

	void Init();
	void Update();
	void LateUpdate();

	/// @brief 銃が撃たれた際のリコイル処理
	void OnRecoil(const GunBase& gun);

	void Activate()   override { m_is_active = true; }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsRecoiling()		 const			{ return m_is_recoiling; }
	[[nodiscard]] bool IsReachedRecoilPeak() const			{ return m_is_reached_recoil_peak; }
	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }

private:
	void SetupForRotCamera();
	void SetupForAimCamera();

	void Move();

	void CalcMoveDirFromPad();
	void CalcMoveDirFromMouse();
	void CalcMoveDirFromCommand();

	void CalcAimPos();
	void CalcInputAngle();
	void CalcRecoilAngle();
	void CalcResultAngle();
	//void CalcInitAim();

	/// @brief 無加工のボーンをカメラが追尾するかを判定
	/// @return true : ボーンそのものをカメラが追尾, false : ボーンを同じ高さにある位置を追尾
	[[nodiscard]] bool IsTrackCameraOriginBone() const;

private:
	static constexpr VECTOR kFollowOffsetForRotCamera		= { 30.0f, 20.0f, -90.0f };
	static constexpr VECTOR kFollowOffsetForAimCamera		= { 30.0f, 20.0f, -90.0f };
	static constexpr VECTOR kTrackedObjOffsetForRotCamera	= { 10.0f, 10.0f,   0.0f };
	static constexpr VECTOR kTrackedObjOffsetForAimCamera	= { 10.0f, 10.0f,   0.0f };

	static constexpr VECTOR kBodyDampingForRotCamera		= { 0.015f, 0.015f, 0.0f };
	static constexpr VECTOR kBodyDampingForAimCamera		= { 0.0f,   0.0f,   0.0f };
	static constexpr float  kHorizontalDampingForRotCamera	= 0.05f;
	static constexpr float  kHorizontalDampingForAimCamera	= 0.0f;
	static constexpr float  kVerticalDampingForRotCamera	= 0.0f;
	static constexpr float  kVerticalDampingForAimCamera	= 0.0f;

	static constexpr float  kCameraAimOffsetBasicSpeed		= 40.0f;

	static constexpr float  kMaxVerticalInputAngle			= 60.0f;
	static constexpr float  kMinVerticalInputAngle			= -60.0f;
	static constexpr float	kMoveSpeedWithStick				= 2.5f;
	static constexpr float	kMoveSpeedWithMouse				= 0.1f;
	static constexpr float	kMoveSpeedWithButton			= 2.0f;

	static constexpr float  kRecoilEndThreshold				= 0.002f;

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
	std::unordered_map<TimeKind, VECTOR>	m_result_angle;						// リザルト角度

	RecoilData								m_recoil_data;
	bool									m_is_recoiling;
	bool									m_is_reached_recoil_peak;
	float									m_recoil_timer;

	//float									init_angle_speed	= 0.0f;			// 視点リセット速度
	//float									init_end_threshold	= 0.0f;			// 視点リセットを終了する閾値
	//bool									is_init_aiming		= false;		// 視点リセット中かを判定

};
