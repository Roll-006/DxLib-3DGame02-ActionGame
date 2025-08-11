#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"
#include "virtual_camera.hpp"

class CameraManager;
class Player;

class ControlVirtualCameraController final : public IVirtualCameraController
{
public:
	ControlVirtualCameraController(Player& player);
	~ControlVirtualCameraController();

	void Init();
	void Update();
	void LateUpdate();

	void Activate()   override { m_is_active = true; }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;

	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	void SetupForRotCamera();
	void SetupForAimCamera();

	void Move();

	void CalcMoveDirFromPad();
	void CalcMoveDirFromMouse();
	void CalcMoveDirFromCommand();

	void CalcInputAngle();
	void CalcAimPos();

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

private:
	VirtualCameraControllerKind				m_virtual_camera_controller_kind;
	bool	m_is_active;

	Player& m_player;

	std::shared_ptr<VirtualCamera>			m_rot_control_camera;				// 回転カメラ
	std::shared_ptr<VirtualCamera>			m_aim_control_camera;				// エイミング時カメラ

	std::shared_ptr<Transform>				m_aim_transform;					// カメラが起点とするトランスフォーム
	VECTOR									m_current_aim_pos;					// 現在の起点座標

	VECTOR									m_move_dir;							// 移動方向
	VECTOR									m_velocity;							// 速度ベクトル
	std::unordered_map<TimeKind, VECTOR>	m_input_angle;						// 入力角度

	//float									init_angle_speed	= 0.0f;			// 視点リセット速度
	//float									init_end_threshold	= 0.0f;			// 視点リセットを終了する閾値
	//bool									is_init_aiming		= false;		// 視点リセット中かを判定

};
