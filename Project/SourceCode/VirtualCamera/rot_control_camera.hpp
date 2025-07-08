#pragma once
#include "../Base/virtual_camera_base.hpp"

class RotControlCamera final : public VirtualCameraBase
{
public:
	RotControlCamera();
	~RotControlCamera() override;


	#pragma region ƒRƒ}ƒ“ƒh
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void InitAngle();
	#pragma endregion

private:
	static constexpr int kPriority = 10;
};