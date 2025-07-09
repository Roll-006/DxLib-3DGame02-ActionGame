#pragma once
#include "../Base/control_virtual_camera_base.hpp"

/// @brief 回転コントロールカメラ
class RotControlVirtualCamera final : public ControlVirtualCameraBase
{
public:
	RotControlVirtualCamera();
	~RotControlVirtualCamera() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()   override;
	void Draw() const	override;

	#pragma region コマンド
	void MoveUp()		override;
	void MoveDown()		override;
	void MoveLeft()		override;
	void MoveRight()	override;
	void InitAim()		override;
	void InitYawAim()   override;
	#pragma endregion

private:
	void Move()			override;

private:
	static constexpr int kPriority = 10;	// バーチャルカメラの優先度
};
