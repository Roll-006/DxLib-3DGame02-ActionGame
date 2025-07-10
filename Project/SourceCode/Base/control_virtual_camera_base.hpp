#pragma once
#include "virtual_camera_base.hpp"

class ControlVirtualCameraBase abstract : public VirtualCameraBase
{
public:
	ControlVirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind);
	virtual ~ControlVirtualCameraBase() = default;

	#pragma region コマンド
	virtual void MoveUp()		abstract;
	virtual void MoveDown()		abstract;
	virtual void MoveLeft()		abstract;
	virtual void MoveRight()	abstract;
	virtual void InitAim()		abstract;
	virtual void InitYawAim()   abstract;
	#pragma endregion

	/// @brief 視点リセット中かを取得
	[[nodiscard]] bool IsInitAiming() const { return m_is_init_aiming; }

protected:
	virtual void Move()					abstract;
	virtual void CalcMoveDirFromPad()	abstract;
	virtual void CalcMoveDirFromMouse()	abstract;

protected:
	enum class InputDir
	{
		kUp,
		kDown,
		kLeft,
		kRight,
	};

protected:
	VECTOR									m_move_dir;					// 移動方向
	VECTOR									m_velocity;					// 速度ベクトル
	std::unordered_map<TimeKind, VECTOR>	m_input_angle;				// 入力角度
	float									m_init_angle_speed;			// 視点リセット速度
	float									m_init_end_threshold;		// 視点リセットを終了する閾値
	std::array<bool, 4>						m_is_input;					// 入力判定
	bool									m_is_init_aiming;			// 視点リセット中かを判定
};
	