#pragma once
#include "../Base/control_virtual_camera_base.hpp"

class RotControlCamera final : public ControlVirtualCameraBase
{
public:
	RotControlCamera();
	~RotControlCamera() override;

	#pragma region ƒRƒ}ƒ“ƒh
	void MoveUp()		override;
	void MoveDown()		override;
	void MoveLeft()		override;
	void MoveRight()	override;
	void InitAim()		override;
	#pragma endregion

private:
	static constexpr int kPriority = 10;
};