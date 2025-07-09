#pragma once
#include "virtual_camera_base.hpp"

class ControlVirtualCameraBase abstract : public VirtualCameraBase
{
public:
	ControlVirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind);
	virtual ~ControlVirtualCameraBase() = default;

	virtual void MoveUp()		abstract;
	virtual void MoveDown()		abstract;
	virtual void MoveLeft()		abstract;
	virtual void MoveRight()	abstract;
	virtual void InitAim()		abstract;

	/// @brief 視点リセット中かを取得
	[[nodiscard]] bool IsInitAiming() const { return m_is_init_aiming; }

protected:
	std::unordered_map<TimeKind, VECTOR>	m_angle;			// 入力角度
	std::array<bool, 4>						m_is_input;			// 入力判定
	bool									m_is_init_aiming;	// 視点リセット中かを判定
};
